#include <stdio.h>
#include <stdlib.h>

#include <zlog.h>

#include "common.h"

bool init_zlog(const char *conf)
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
