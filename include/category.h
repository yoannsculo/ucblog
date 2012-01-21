#ifndef ___CATEGORY_H_
#define ___CATEGORY_H_

#include <limits.h>

struct s_category {
	char name[PATH_MAX];
	char path[PATH_MAX];
	// int status; // like private / public ?
};

#endif
