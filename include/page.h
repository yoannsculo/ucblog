#ifndef __PAGE_H_
#define __PAGE_H_

#include <limits.h>

#include "category.h"

struct s_page {
	struct s_category *category;
	char title[PATH_MAX+1];
	char source[PATH_MAX+1];

	char permalink[500]; /* page's url*/
	char *content; /* HTML content */

	int date;
};

struct s_page *add_page(char *filename, struct s_category *category);

#endif // __PAGE_H_
