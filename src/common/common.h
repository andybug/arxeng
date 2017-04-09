#ifndef ARX_COMMON_H
#define ARX_COMMON_H

#include <stdbool.h>
#include <zlog.h>

/* logs.c */
extern bool arx_log_init(const char *conf);
extern zlog_category_t *arx_log_category(const char *cat);

/* state.c */
extern void arx_state_init(void);
extern void arx_state_set_shutdown(bool _shutdown);
extern bool arx_state_get_shutdown(void);

#endif
