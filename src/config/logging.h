#ifndef LOGGING_H
#define LOGGING_H

void log_init(const char *log_file_path);
void log_write(const char *format, ...);
void log_close(void);

#endif