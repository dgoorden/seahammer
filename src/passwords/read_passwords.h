#ifndef READ_PASSWORDS_H
#define READ_PASSWORDS_H

#include <wpa_ctrl.h>

#define MAX_PASSWORDS 100000
#define MAX_PASSWORD_LENGTH 256

typedef struct {char passworxds[MAX_PASSWORDS][MAX_PASSWORD_LENGTH]; int count;} Wordlist;
int read_passwords(const char *path, Wordlist *out);
int spray_passwords(struct wpa_ctrl *ctrl, struct wpa_ctrl *mon, const char *path, const char *ssid, const char* identity);

#endif

