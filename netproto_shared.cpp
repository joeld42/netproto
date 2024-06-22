#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Sleep function
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <windows.h>
#include <synchapi.h>
#else
#include <time.h>
#endif

#include "netproto_shared.h"

// Sleep for a given amount of seconds
// Used to limit client and server tick rate
void EchoSleep(double sec)
{
#if defined(__EMSCRIPTEN__)
    emscripten_sleep(sec * 1000);
#elif defined(_WIN32) || defined(_WIN64)
    Sleep(sec * 1000);
#else /* UNIX / OSX */
    long nanos = sec * 1e9;
    struct timespec t = { .tv_sec = nanos / 999999999, .tv_nsec = nanos % 999999999 };

    nanosleep(&t, &t);
#endif
}

static const char* log_type_strings[] = {
    "INFO",
    "ERROR",
    "DEBUG",
    "TRACE",
    "WARNING"
};

// Basic logging function
void Log(int type, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    printf("[%s] ", log_type_strings[type]);
    vprintf(fmt, args);
    printf("\n");

    va_end(args);
}

