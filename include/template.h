#ifndef __TEMPLATE_H_
#define __TEMPLATE_H_

#include "ctemplate.h"

struct s_template {
	char name[255];
	TMPL_varlist *varlist;
};


void test();
int load_template(struct s_template *tpl);

#endif
