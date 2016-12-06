#include "log.h"

const char *LOG_ID = "riot-atom";

void log_init() {
    int log_opt = LOG_CONS | LOG_PID;
    int log_mask = LOG_MASK (LOG_ERR);

#ifdef DEBUG
    log_opt |= LOG_PERROR;
    log_mask = LOG_UPTO(LOG_DEBUG);
#endif

    openlog (LOG_ID, log_opt, LOG_USER);

    (void) setlogmask(log_mask);
}

void log_deinit() {
    closelog();
}
