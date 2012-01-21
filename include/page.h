#ifndef __PAGE_H_
#define __PAGE_H_

#include <limits.h>

struct s_page {
	char category[255];
	char directory[PATH_MAX];
	char permalink[500];
	char *content;
	int date;
};

#endif // __PAGE_H_
