#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#include <msgpack.h>
#include <zlog.h>
#include <zmq.h>

#include "../common/common.h"
#include "server.h"

#define LISTEN_BUF_SIZE 2048


struct net_listen_state {
	/* logging */
	zlog_category_t *zcat;
	/* zmq handles */
	void *context;
	void *rep;
};

static void net_listen_cleanup(void *_s)
{
	struct net_listen_state *s = _s;

	zmq_close(s->rep);
	zmq_ctx_destroy(s->context);

	zlog_warn(s->zcat, "listen thread terminated");
}

static bool unpack_header(struct net_listen_state *s, char *buf, size_t len)
{
	msgpack_unpacked result;
	msgpack_unpack_return ret;
	size_t offset = 0;

	msgpack_unpacked_init(&result);

	ret = msgpack_unpack_next(&result, buf, len, &offset);
	if (ret != MSGPACK_UNPACK_SUCCESS) {
		zlog_warn(s->zcat, "failed unpacking header");
		return false;
	}

	msgpack_object obj = result.data;
	if (obj.type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
		zlog_warn(s->zcat, "incorrect header format");
		return false;
	}

	zlog_info(s->zcat,
		  "message from client %llx",
		  (unsigned long long)obj.via.u64);

	return true;
}

void *net_listen(void *_v)
{
	(void)_v;
	struct net_listen_state s;
	char buf[LISTEN_BUF_SIZE];
	int size;
	int rc;

	/* configure logging */
	s.zcat = arx_log_category("net");
	zlog_info(s.zcat, "listening for connections");

	/* bind tcp port for as server */
	s.context = zmq_ctx_new();
	s.rep = zmq_socket(s.context, ZMQ_REP);
	rc = zmq_bind(s.rep, "tcp://*:5555");
	if (rc < 0) {
		zlog_error(s.zcat, "zmq_bind errno: %d", errno);
		exit(1);
	}

	/* set cleanup func */
	pthread_cleanup_push(net_listen_cleanup, &s);


	while (1) {
		size = zmq_recv(s.rep, buf, LISTEN_BUF_SIZE, 0);
		if (size < 0) {
			zlog_debug(s.zcat, "zmq_recv errno: %d", errno);
			continue;
		}

		unpack_header(&s, buf, (size_t)size);
		zmq_send(s.rep, "thank you, come again", 20, 0);
	}

	pthread_cleanup_pop(1);

	return NULL;
}
