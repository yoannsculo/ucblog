#include <stdio.h>

#include "page.h"
#include "category.h"
#include "string.h"
#include "files.h"
#include "config.h"
#include "template.h"

struct s_page pages[500];
struct s_page *ptr_page = pages;
int pages_count = 0;

extern struct s_config_site config_site;

void print_page(struct s_page *page)
{
	printf("Page : \n");
	printf("\tTitle : %s\n", page->title);
	printf("\tCategory : %s\n", page->category->name);
	printf("\tFilename : %s\n", page->filename);
	printf("\tSource dir : %s\n", page->src_dir);
	printf("\tPermalink : %s\n", page->permalink);
}

static int get_syntax_from_extension(char *extension)
{
	if (extension == NULL)
		return SYNTAX_NONE;

	if (strcmp(extension, "mkd") == 0 ||
	    strcmp(extension, "markdown") == 0)
		return SYNTAX_MARKDOWN;

	return SYNTAX_NONE;
}

struct s_page *add_page(char *filename, struct s_category *category)
{
	char file[PATH_MAX+1];

	if (filename == NULL || category == NULL)
		return NULL;

	ptr_page->category = category;
	strcpy(ptr_page->title, "this is a title"); // TODO
	// strcpy(ptr_page->src_dir, filename);

	get_current_dir(filename, ptr_page->src_dir);

	if (get_extension(ptr_page->extension, filename) != 0)
		return NULL;

	ptr_page->syntax = get_syntax_from_extension(ptr_page->extension);
	get_short_filename(filename, file);
	remove_extension(file, ptr_page->filename);

	get_current_dir(filename+1, ptr_page->permalink); // +1 to delete _

	pages_count++;
	return ptr_page++;
}

static int load_content(struct s_page *page)
{
	char input[PATH_MAX+1];
	char output[PATH_MAX+1];

	// TODO : Could be done in one shot later, just in memory (no tmp file)

	sprintf(input,  "%s/%s/%s.%s", config_site.source_directory,
				    page->src_dir,
				    page->filename,
				    page->extension);

	sprintf(output, "%s/%s/content.html", config_site.dest_directory,
					      page->permalink);

	if (!is_file(input)) {
		printf("Couldn't find %s file\n", input);
		return -1;
	}

	convert_mkd_to_html(input, output);

	return 0;
}

int generate_page(struct s_page *page)
{
	char content[PATH_MAX+1];

	if (load_content(page) < 0) {
		printf("Couldn't load content of %s/%s/%s.%s\n", config_site.source_directory,
								 page->src_dir,
								 page->filename,
								 page->extension);
		return -1;
	}


	// generate_static_page(page); ?
	if (apply_template(page) < 0) {
		printf("Couldn't apply template\n");
		return -1;
	}

	print_page(page);

	// free_content(page);
	return 0;
}

int generate_pages()
{
	int i;

	ptr_page = pages; // let's rewind !

	for (i=0;i<pages_count;i++) {
		generate_page(ptr_page);
		ptr_page++;
	}
	return 0;
}
