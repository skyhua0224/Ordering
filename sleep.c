#include "sleep.h"

#ifdef _POSIX_C_SOURCE >= 199309L
void sleep(int sec) {
    struct timespec ts;
    ts.tv_sec = sec;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);
}
#endif