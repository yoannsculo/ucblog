#ifndef __PAGE_H_
#define __PAGE_H_

#include <limits.h>

#include "category.h"

enum
{
	SYNTAX_NONE = 0,
	SYNTAX_MARKDOWN
};

struct s_page {
	struct s_category *category;

	char title[PATH_MAX+1]; /* Full title in plain text */
	char src_dir[PATH_MAX+1];
	char filename[PATH_MAX+1]; /* Filename without extension */
	int syntax; /* if it is mkd, yaml, ... */
	char extension[50];

	char permalink[500];	/* page's url, similar as title but for SEO */
	char *content;		/* HTML content */

	int date;
};

struct s_page *add_page(char *filename, struct s_category *category);

#endif // __PAGE_H_
