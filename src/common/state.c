#include <pthread.h>
#include <stdbool.h>

#include "common.h"


static pthread_mutex_t shutdown_mutex;
static bool shutdown = false;


void arx_state_init(void)
{
	pthread_mutex_init(&shutdown_mutex, NULL);
}

void arx_state_set_shutdown(bool _shutdown)
{
	pthread_mutex_lock(&shutdown_mutex);
	shutdown = _shutdown;
	pthread_mutex_unlock(&shutdown_mutex);
}

bool arx_state_get_shutdown(void)
{
	bool _shutdown;

	pthread_mutex_lock(&shutdown_mutex);
	_shutdown = shutdown;
	pthread_mutex_unlock(&shutdown_mutex);

	return _shutdown;
}
