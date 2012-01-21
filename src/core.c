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

#include "category.h"

extern struct s_config_site config_site;

// Relative path
static int process_mkd(char *filename, struct s_category *category)
{
	struct s_page *page;
	
	page = add_page(filename, category);
	if (page == NULL)
		return -1;
	else
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
static int process_file(char *filename, struct s_category *category)
{
	//printf("Processing filename : %s\n", filename);
	if (is_markdown_file(filename))
		return process_mkd(filename, category);

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

// relative path
int process_dir(char *path, struct s_category *category)
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
				sprintf(new_dir, "%s/%s", config_site.dest_directory, path_name+1); // +1 is to delete _
				create_dir(new_dir);
				process_dir(path_name, category);
			} else {
				process_file(path_name, category);
			}
		}	
	}

	closedir(dir);

	return 0;
}

// relative path (_blog, _pages, ...)
static int process_dynamic_dir(char *dirname)
{
	char dir[PATH_MAX];

	if (dirname == NULL) {
		printf("Empty dirname\n");
		return -1;
	}

	if (!is_dynamic_element(dirname)) {
		printf("Bad category\n");
		return -1;
	}

	if (is_protected_directory(dirname))
		return 0;
	else
		return process_category(dirname);
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
	// TODO : usefull ?
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

int is_protected_directory(char *dirname)
{
	if (strcmp(dirname, "_layout") == 0)
		return 1;
	else
		return 0;
}





/* First step
 * - find and process each directory with the prefix "_"
 * - find and move regular files and directories
 */
static int process_tree()
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

	res = process_tree();
	res = generate_pages();
	// res = generate_redirections();
	// res = generate_sitemap();

	return res;
}

