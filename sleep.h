// sleep.h
#ifndef SLEEP_H
#define SLEEP_H

#ifdef _WIN32
    #include <windows.h>
    #define sleep(x) Sleep(1000 * (x))
#elif _POSIX_C_SOURCE >= 199309L
    #include <time.h>   // for nanosleep
    void sleep(int sec);  // Only declare the function here
#else
    #include <unistd.h> // for sleep
#endif

#endif // SLEEP_H