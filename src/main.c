#include <stdio.h>
#include <unistd.h>

#include "files.h"
#include "core.h"

#include "template.h"


#include "config.h"

#define CMD_NAME "ucblog"

static void usage()
{
	printf("Usage: %s [OPTION]... SOURCE DEST\n", CMD_NAME);
}

int main(int argc, char *argv[])
{
	char *source;
	char *dest;
	int res = 0;

	if (argc != 3) {
		usage();
		return -1;
	}
	
	source = argv[1];
	dest   = argv[2];

	if (load_parameters(source, dest) != 0) {
		usage();
		return -1;
	}

	load_site_config();

	res = process_site();
	if (res != 0)
		printf("Site processing aborted.\n");
	else
		printf("Site sucessfully processed.\n");

	// TODO : deploy_site();

	return 0;
}

