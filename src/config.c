#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "config.h"

struct s_config_site config_site;

int load_parameters(char *source, char *dest)
{
	if (check_source_dir(source) != 0) {
		printf("Wrong source value\n");
		return -1;
	}

	if (check_dest_dir(dest) != 0) {
		printf("Wrong dest value\n");
		return -1;
	}

	strcpy(config_site.source_directory, source);
	strcpy(config_site.dest_directory, dest);

	return 0;
}

static void get_config_path(char *filename)
{
	sprintf(filename, "%s/config", config_site.source_directory);
}

static int split_line(char *line, char *var, char *value)
{
	char *pch;

	if (line == NULL || var == NULL || value == NULL)
		return -1;

	if (line[strlen(line)-1] == '\n') {
		line[strlen(line)-1] = '\0';
	}

	pch = strtok(line,"=");
	if (pch == NULL)
		return -1;

	if (strcmp(pch, "") == 0)
		return -1;
	
	strcpy(var, pch);

	pch = strtok (NULL, "=");
	if (pch == NULL)
		return -1;

	strcpy(value, pch);
	
	return 0;
}

int load_site_config()
{
	FILE *fp;
	char config_path[255];
	char line[255];
	int i =0;

	get_config_path(config_path);

	fp = fopen(config_path, "r");
	if (fp == NULL) {
		printf("Error ! \n");
		return -1;
	}

	while (fgets(line, 255, fp) != NULL) {
		char var[255];
		char value[255];
		
		if (split_line(line, var, value) != 0)
			continue;
		
		strcpy(config_site.var[i].var, var);
		strcpy(config_site.var[i].value, value);
		i++;
	}

	strcpy(config_site.var[i].var, "");
	strcpy(config_site.var[i].value, "");
		
	fclose(fp);

	return 0;
}

