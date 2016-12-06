#ifndef _LOG_H_
#define _LOG_H_

#include <syslog.h>

void log_init(void);
void log_deinit(void);

#define log_error(...) syslog (LOG_ERR, __VA_ARGS__);
#define log_warning(...) syslog (LOG_WARNING, __VA_ARGS__);
#define log_info(...) syslog (LOG_INFO, __VA_ARGS__);
#define log_debug(...) syslog (LOG_DEBUG, __VA_ARGS__);

#endif // _LOG_H_
