#include <stdio.h>
#include <limits.h>

#include "category.h"
#include "string.h"
#include "files.h"
#include "config.h"
#include "core.h"

struct s_category categories[50];
struct s_category *ptr_cat = categories;
int categories_count = 0;

extern struct s_config_site config_site;

static struct s_category *add_category(char *dirname)
{
	char layout_file[PATH_MAX+1];

	if (dirname == NULL)
		return NULL;

	char name[strlen(dirname)];
	strcpy(name, dirname+1);

	strcpy(ptr_cat->name, name);
	strcpy(ptr_cat->path, dirname);

	sprintf(layout_file, "%s/_layout/%s.layout", config_site.source_directory,
						     ptr_cat->name);
	/* Check if layout exists */
	if (!is_file(layout_file))
		return NULL;

	categories_count++;
	return ptr_cat++;
}

int process_category(char *dirname)
{
	struct s_category *category;
	char new_dir[PATH_MAX + 1];

	category = add_category(dirname);
	if (category == NULL) {
		printf("Couldn't properly add category : %s\n", dirname);
		return -1;
	}


	sprintf(new_dir, "%s/%s", config_site.dest_directory, category->name);

	if (create_dir(new_dir) < 0) {
		return -1;
	}
	else {
		/* Category is created, now we can dig into it */
		return process_dir(dirname, category);
	}
}
