#include <stdio.h>
#include <time.h>
#include "logging.h"
#include <stdarg.h>


static FILE *log_file = NULL;

void log_init(const char *log_file_path) {
    log_file = fopen(log_file_path, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error opening log file: %s\n", log_file_path);
    }
}

void log_write(const char *format, ...) {
    time_t now = time(NULL);
    char time_stamp[32];
    strftime(time_stamp, sizeof(time_stamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    if (log_file != NULL) {
        fprintf(log_file, "[%s] ", time_stamp);
        va_list args;
        va_start(args, format);
        vfprintf(log_file, format, args);
        va_end(args);
        fprintf(log_file, "\n");
        fflush(log_file);
    }
}

void log_close(void) {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}