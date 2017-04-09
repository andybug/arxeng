#include <stdio.h>
#include <stdlib.h>

#include <zlog.h>

#include "common.h"

bool arx_log_init(const char *conf)
{
	int err;
	const char *env = getenv("ARX_ZLOG_CONF");

	if (env)
		conf = env;

	err = zlog_init(conf);
	if (err) {
		fprintf(stderr, "zlog_init failed - '%s'\n", conf);
		return false;
	}

	return true;
}

zlog_category_t *arx_log_category(const char *cat)
{
	zlog_category_t *zcat = zlog_get_category(cat);
	if (!zcat) {
		fprintf(stderr, "zlog_get_category failed - '%s'\n", cat);
		exit(1);
	}

	return zcat;
}
