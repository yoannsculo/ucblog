#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include "core.h"
#include "files.h"
#include "markup.h"
#include "template.h"

#include "config.h"

// static char source_directory[PATH_MAX];
// static char dest_directory[PATH_MAX];

extern struct s_config_site config_site;

// TODO : maybe use input / output parameters here ?
static int generate_mkd_to_html(char *mkd_filename)
{
	char input[PATH_MAX];
	char output[PATH_MAX];
	char input_dir[PATH_MAX];
	
	if (get_current_dir(mkd_filename, input_dir) != 0)
		return -1;

	// if (!strcmp(input_dir, "blog") || !strcmp(input_dir, "pages")) {
	// 	// We avoid depth 0 directories
	// 	return -1;
	// }	    

	sprintf(input,  "%s/%s", config_site.source_directory, mkd_filename);
	sprintf(output, "%s/%s/content.html", config_site.dest_directory, input_dir);

	// printf("convert mdk %s -> to html %s\n", input, output);

	// TODO : éviter taille zero

	return convert_mkd_to_html(input, output);
}

// Relative path
static int process_mkd(char *filename)
{
	if (generate_mkd_to_html(filename) != 0)
		return -1;
	
	printf("Processing MKD file : %s\n", filename);
	
	return 0;
}

// Relative path
static int process_image(char *filename)
{
	char dir[PATH_MAX];
	
	char source[PATH_MAX];
	char dest[PATH_MAX];

	if (get_current_dir(filename, dir) != 0)
		return -1;

	sprintf(source,  "%s/%s", config_site.source_directory, filename);
	sprintf(dest, "%s/%s", config_site.dest_directory, dir);

	if (cp_file(source, dest) == 0) {
		printf("Processing IMG file : %s\n", filename);
		return 0;
	} else {
		return -1;
	}
}

// Relative path
static int process_file(char *filename)
{
	//printf("Processing filename : %s\n", filename);
	if (is_markdown_file(filename))
		return process_mkd(filename);

	if (is_image_file(filename))
		return process_image(filename);
	

	// if (is_document_file(filename)) {
	// 	return process_document(filename);
	// }
	
	// is (is_music_file(filename)) {
	// 	return process_music(filename);
	// }


	// TODO : add_header_footer
	//
	
	return -1;
}

// TODO : rename into search_for_html
// static int apply_template(char *template, char *path)
static int parse_folder(char *template, char *path)
{
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	struct stat entryInfo;

	char path_name[PATH_MAX + 1];
	char absolute_path[PATH_MAX + 1];
	
	sprintf(absolute_path, "%s/%s", config_site.dest_directory, path);
	
	if ((dir = opendir(absolute_path)) == NULL) {
		printf("Couldn't open %s : %s\n", absolute_path, strerror(errno));
		return -1;
	}
	
	// printf("looking into %s\n", absolute_path);

	while ((readdir_r(dir, &entry, &result) == 0) && result != 0) {
		if (strcmp(entry.d_name, ".")  == 0 ||
		    strcmp(entry.d_name, "..") == 0) {
			continue;
		}
	
		strncpy(path_name, path, PATH_MAX);
		strncat(path_name, "/", PATH_MAX);
		strncat(path_name, entry.d_name, PATH_MAX);
		
		sprintf(absolute_path, "%s/%s", config_site.dest_directory, path_name);	
		
		if (lstat(absolute_path, &entryInfo) == 0) {
			if (S_ISDIR (entryInfo.st_mode)) {
				//printf("%s is a dir\n", path_name);			
				parse_folder(template, path_name);
			} else {
				char short_filename[255];
				get_short_filename(path_name, short_filename);
				// printf("template : %s - file : %s - %s\n", template, short_filename, absolute_path);
				
				if (is_html_file(path_name)) {
					apply_template(template, absolute_path);
					// printf("%sProcessing HTML file : %s\n", ( res >= 0 ? "" : "FAILED " ), path_name);
				}
			}
		}


		// sprintf(absolute_path, "%s/%s", source_directory, path_name);	
	}

	closedir(dir);

	return 0;
}

// relative path
static int process_dir(char *path)
{
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	struct stat entryInfo;
	
	char path_name[PATH_MAX + 1];
	char absolute_path[PATH_MAX + 1];
	char new_dir[PATH_MAX + 1];

	sprintf(absolute_path, "%s/%s", config_site.source_directory, path);	
	
	if ((dir = opendir(absolute_path)) == NULL) {
		printf("Couldn't open %s : %s\n", path, strerror(errno));
		return -1;
	}

	while ((readdir_r(dir, &entry, &result) == 0) && result != 0) {
		if (strcmp(entry.d_name, ".")  == 0 ||
		    strcmp(entry.d_name, "..") == 0) {
			continue;
		}
		
		strncpy(path_name, path, PATH_MAX);
		strncat(path_name, "/", PATH_MAX);
		strncat(path_name, entry.d_name, PATH_MAX);

		sprintf(absolute_path, "%s/%s", config_site.source_directory, path_name);	

		if (lstat(absolute_path, &entryInfo) == 0) {
			if (S_ISDIR (entryInfo.st_mode)) {
				sprintf(new_dir, "%s/%s", config_site.dest_directory, path_name);
				create_dir(new_dir);
				process_dir(path_name);
			} else {
				process_file(path_name);
			}
		}	
	}

	closedir(dir);

	return 0;
}

/* TODO : rename into category */
// relative path (_blog, _pages, ...)
static int process_dynamic_dir(char *dirname)
{
	if (dirname == NULL)
		return -1;

	// mkd -> html content (just raw data)
	process_dir(dirname);

	// include html content into dynamic html pages
	parse_folder("blog", dirname); // TODO : change blog template to dynamic
	
	// TODO : add dir cleanup function (delete blog.html)

	return 0;
}

// Relative path
static int process_regular_dir(char *dirname)
{
	char source_dir[PATH_MAX + 1];
	char dest_dir[PATH_MAX + 1];
	
	sprintf(source_dir, "%s/%s", config_site.source_directory, dirname);
	sprintf(dest_dir, "%s/%s", config_site.dest_directory, dirname);

	return cp_dir(source_dir, dest_dir);
}

// Relative path
static int process_regular_file(char *filename)
{
	// TODO
	return 1;
}

// Relative path
static int process_dynamic_file(char *filename)
{
	// TODO
	return 1;
}

int is_dynamic_element(char *name)
{
	if (name == NULL)
		return 0;

	if (*name == '_')
		return 1;
	else
		return 0;
}

static int process_root_dir()
{
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	struct stat entryInfo;
	char *root_dir;
	char absolute_path[PATH_MAX + 1];
	
	root_dir = config_site.source_directory;

	if ((dir = opendir(root_dir)) == NULL) {
		printf("Couldn't open %s : %s\n", root_dir, strerror(errno));
		return -1;
	}

	while ((readdir_r(dir, &entry, &result) == 0) && result != 0) {
		if (strcmp(entry.d_name, ".")  == 0 ||
		    strcmp(entry.d_name, "..") == 0) {
			continue;
		}
		
		sprintf(absolute_path, "%s/%s", root_dir, entry.d_name);

		if (lstat(absolute_path, &entryInfo) == 0) {
			if (S_ISDIR (entryInfo.st_mode)) {
				if (is_dynamic_element(entry.d_name))
					process_dynamic_dir(entry.d_name);
				else
					process_regular_dir(entry.d_name);
			} else {
				if (is_dynamic_element(entry.d_name))
					process_dynamic_file(entry.d_name);
				else
					process_regular_file(entry.d_name);
			}
		}	
	}

	closedir(dir);

	return 0;
}

static int prepare_site()
{
	// TODO : clean dest folder	
	return 0;
}

int process_site()
{
	int res = 0;

	res = prepare_site();
	if (res != 0)
		return -1;

	res = process_root_dir();

	return res;
}

