#include <stdio.h>
#include <stdlib.h>

#include <zlog.h>

#include "../common/common.h"


static zlog_category_t *zcat;


static void init_logging(void)
{
	if (!init_zlog(NULL))
		exit(1);

	zcat = zlog_get_category("server");
	if (!zcat) {
		fprintf(stderr, "zlog_get_category failed - 'server'\n");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init_logging();
	zlog_info(zcat, "arxeng-server starting...");

	return 0;
}
