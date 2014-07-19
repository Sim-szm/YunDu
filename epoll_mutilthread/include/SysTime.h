#ifndef _SYSTIME_H
#define _SYSTIME_H
#include <sys/timerfd.h>
static inline unsigned long GetSystemMs() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec*1000 + now.tv_nsec/(1000*1000);
}
#endif
