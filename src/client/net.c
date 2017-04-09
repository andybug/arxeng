#include <stdint.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#include <msgpack.h>
#include <zlog.h>
#include <zmq.h>

#include "../common/common.h"
#include "client.h"


struct net_main_state {
	/* logging */
	zlog_category_t *zcat;
	/* zmq handles */
	void *context;
	void *res;
	/* msgpack */
	msgpack_sbuffer sbuf;
	/* general */
	uint64_t client_id;
};

static void net_main_cleanup(void *_s)
{
	struct net_main_state *s = _s;

	zmq_close(s->res);
	zmq_ctx_destroy(s->context);

	msgpack_sbuffer_destroy(&s->sbuf);

	zlog_warn(s->zcat, "net_main thread terminated");
}

static void pack_header(struct net_main_state *s)
{
	msgpack_packer pk;

	msgpack_sbuffer_clear(&s->sbuf);
	msgpack_packer_init(&pk, &s->sbuf, msgpack_sbuffer_write);

	msgpack_pack_uint64(&pk, s->client_id);
}

void *net_main(void *_v)
{
	(void)_v;
	struct net_main_state s;
	int rc;

	s.client_id = 0x1234567890abcdef;

	/* configure logging */
	s.zcat = arx_log_category("net");
	zlog_info(s.zcat, "connecting to server");

	/* establish connection to server */
	s.context = zmq_ctx_new();
	s.res = zmq_socket(s.context, ZMQ_REQ);
	rc = zmq_connect(s.res, "tcp://localhost:5555");
	if (rc < 0) {
		zlog_error(s.zcat, "zmq_connect errno: %d", errno);
		exit(1);
	}

	/* msgpack buffer */
	msgpack_sbuffer_init(&s.sbuf);

	/* set cleanup func */
	pthread_cleanup_push(net_main_cleanup, &s);


	while (1) {
		char buffer[256];
		int size;

		pack_header(&s);
		zmq_send(s.res, s.sbuf.data, s.sbuf.size, 0);

		size = zmq_recv(s.res, buffer, 256, 0);
		if (size < 0) {
			zlog_debug(s.zcat, "zmq_recv errno: %d", errno);
			continue;
		}

		sleep(5);
	}

	pthread_cleanup_pop(1);

	return NULL;
}
