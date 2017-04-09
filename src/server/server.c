#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>

#include <zlog.h>

#include "../common/common.h"
#include "server.h"

static pthread_t net_thread;


static void sigint_handler(int sig)
{
	(void)sig;
}

static void set_handlers(void)
{
	struct sigaction s;

	/* use sigint_handler for SIGINT */
	s.sa_handler = sigint_handler;
	s.sa_flags = 0;
	sigemptyset(&s.sa_mask);
	sigaction(SIGINT, &s, NULL);
}

static void create_threads(void)
{
	sigset_t sigset, origset;

	/*
	 * make all spawned pthreads ignore SIGINT
	 * we only want the main thread to handle that
	 */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	pthread_sigmask(SIG_BLOCK, &sigset, &origset);

	/* spawn the threads */
	pthread_create(&net_thread, NULL, net_listen, NULL);

	/* reset the sigmask on the main thread */
	pthread_sigmask(SIG_SETMASK, &origset, NULL);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	zlog_category_t *zcat;

	if (!arx_log_init(NULL))
		exit(1);

	zcat = arx_log_category("server");
	zlog_info(zcat, "arxeng-server starting...");

	set_handlers();
	create_threads();

	pause();

	pthread_cancel(net_thread);
	pthread_join(net_thread, NULL);

	zlog_warn(zcat, "server stopped");
	zlog_fini();

	return 0;
}
