#ifndef OS_COMPAT_H
#define OS_COMPAT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define os_zalloc(s)            calloc(1, (s))
#define os_malloc(s)            malloc(s)
#define os_free(p)              free(p)
#define os_memcpy(d, s, n)      memcpy(d, s, n)
#define os_memset(d, c, n)      memset(d, c, n)
#define os_memcmp(a, b, n)      memcmp(a, b, n)
#define os_strlen(s)            strlen(s)
#define os_strncmp(a, b, n)     strncmp(a, b, n)
#define os_strchr(s, c)         strchr(s, c)
#define os_strdup(s)            strdup(s)
static inline size_t os_strlcpy(char *dst, const char *src, size_t n) {
    size_t len = strlen(src);
    if (n > 0) {
        size_t copy = len < n - 1 ? len : n - 1;
        memcpy(dst, src, copy);
        dst[copy] = '\0';
    }
    return len;
}
#define os_snprintf             snprintf
#define os_snprintf_error(s,r)  ((r) < 0 || (size_t)(r) >= (s))
#define os_sleep(s, us)         usleep((s) * 1000000 + (us))

struct os_reltime {
    long sec;
    long usec;
};

static inline int os_get_reltime(struct os_reltime *t) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    t->sec  = ts.tv_sec;
    t->usec = ts.tv_nsec / 1000;
    return 0;
}

static inline int os_reltime_expired(struct os_reltime *now,
                                     struct os_reltime *start,
                                     long timeout_secs) {
    return (now->sec - start->sec) >= timeout_secs;
}

#endif