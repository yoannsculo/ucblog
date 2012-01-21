#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "ctemplate.h"

#include "template.h"

#include "files.h"
#include "config.h"

//  enum
//    {
//      TOTAL_KEYWORDS = 23,
//      MIN_WORD_LENGTH = 1,
//      MAX_WORD_LENGTH = 10,
//      MIN_HASH_VALUE = 1,
//      MAX_HASH_VALUE = 37
//    };


#define TPL_DEFAULT	"default.tpl"
#define TPL_BLOG	"post.tpl"
#define TPL_LAYOUT_DIR	"layout"


static TMPL_loop *mainloop;
static TMPL_varlist *mainlist;


extern struct s_config_site config_site;


// static const char * const protected_wordlist[] =
// {
// 	"content",
// 
// };


static int is_protected_var(char *var)
{
	// TODO : list
	// content
	return 1;
}

static void site_add_var(char *var, char *content)
{
	if (!is_protected_var(var))
		return;

	mainlist = TMPL_add_var(mainlist, var, content, 0);
	// mainlist = TMPL_add_var(mainlist, "title", "coucou", 0);
}




// static void prepare_site()
// {
// 	// mainloop = TMPL_add_varlist(mainloop, TMPL_add_var(mainloop, "title", "Yoann blog", 0));
// 	// mainloop = TMPL_add_varlist(mainloop, TMPL_add_var(mainloop, "magic", "<p>Yaye !</p>", 0));
// 	// 
// 	// mainlist = TMPL_add_loop(mainlist, "arg", mainloop);
// 
// 	site_add_var("content", "<p><tmpl_var name=\"site_title\"></p>");
// 	site_add_var("site_title", "Yoann Sculo");
// 	site_add_var("site_author", "Yoann Sculo");
// 	site_add_var("site_mailto", "yoann.sculo@gmail.com");
// 
// 
// 	// site_add_var("title", "Yoann blog");
// 	// site_add_var("site_author", "Yoann Sculo");
// 	// site_add_var("site_mailto", "yoann.sculo@gmail.com");
// 
// 	// mainlist = TMPL_add_var(mainlist, "title", "Yoann blog", 0);
// 	// mainlist = TMPL_add_var(mainlist, "magic", "<p>Yaye !</p>", 0);
// }

static int free_buffer(char **buffer)
{
	free(*buffer);
	*buffer = NULL;
	return 0;
}


void load_variables_from_cfg(TMPL_varlist *tmpl_list, struct s_var_entry *var)
{
	int i = 0;

	while (strcmp(var[i].var, "") != 0 && strcmp(var[i].value, "") != 0) {
		site_add_var(var[i].var, var[i].value);
		i++;
	}
}

static int fill_with_content(char *content, char *layout, char *result)
{
	FILE *fp;
	char layout_path[255];

	fp = fopen(result, "w+");
	if (fp == NULL) {
		printf("Could not open file : %s\n", strerror(errno));
		return -1;
	}

	load_variables_from_cfg(mainlist, config_site.var);	
	//template_add_pages(mainlist, "blog");

	mainlist = TMPL_add_var(mainlist, "content", content, 0);
	sprintf(layout_path, "./sources/_layout/%s.layout", layout);
	TMPL_write(layout_path, 0, 0, mainlist, fp, stderr);
	TMPL_free_varlist(mainlist);
	mainlist = NULL;
	fclose(fp);

	return 0;
}

// apply template on a file
int apply_template(char *template, char *filename)
{
	char *buffer = NULL;
	char temp_file[255]; // TODO : utiliser 2 temp_files
	char current_dir[255];
	char short_filename[255];

	long size;
	
	if (!is_file(filename)) {
		printf("%s is not a file\n", filename);
		return -1;
	}

	get_short_filename(filename, short_filename);
	if (strcmp(short_filename, "content.html"))
		return 1;

	size = load_file_content(filename, &buffer);
	if (size == 0) {
		return -1;
	}

	printf("Processing HTML file : %s\n", filename);

	get_current_dir(filename, current_dir);
	sprintf(temp_file, "%s/%s.html", current_dir, template);

	fill_with_content(buffer, template, temp_file);
	free_buffer(&buffer);
	
	size = load_file_content(temp_file, &buffer);
	if (size == 0) {
		return -1;
	}

	sprintf(temp_file, "%s/index.html", current_dir);
	fill_with_content(buffer, "default", temp_file);
	free_buffer(&buffer);

	sprintf(temp_file, "%s/content.html", current_dir);
	if (remove(temp_file) != 0) {
		printf("error !\n");
	}

	sprintf(temp_file, "%s/%s.html", current_dir, template);	
	if (remove(temp_file) != 0) {
		printf("error !\n");
	}

	return 0;
}


