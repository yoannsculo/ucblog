#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <limits.h>

struct s_var_entry {
	char var[255];
	char value[255];
};

struct s_config_site {
	char source_directory[PATH_MAX];
	char dest_directory[PATH_MAX];
	struct s_var_entry var[255];	
};


int load_site_config();
int load_parameters(char *source, char *dest);
void get_parameters(char *source, char *dest);

#endif
