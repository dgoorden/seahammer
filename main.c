#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <wpa_ctrl.h>
#include <unistd.h>
#include "src/wifi/connect.h"
#include "src/passwords/read_passwords.h"
#include "src/passwords/check_password_lists.h"
#include "src/config/logging.h"

int verbose = 0;

#define RED_TEXT "\033[0;31m"
#define RED_BOLD_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[0;32m"
#define YELLOW_TEXT "\033[0;33m"
#define RESET_TEXT "\033[0m"
#define CYAN_TEXT "\033[0;36m"
#define CLEAR_SCREEN "\033[2J\033[H"
#define CURSOR_POSITION "\033[s"
#define CURSOR_HOME "\033[H"

int main(int argc, char *argv[]) {
    int exit_code = 0;
    char *wordlist = NULL;
    char *ssid = NULL;
    char *username = NULL;
    char *interface = NULL;

    printf("%sChecking for required arguments...%s\n\r", YELLOW_TEXT, RESET_TEXT);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            wordlist = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            ssid = argv[++i];
        } else if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            username = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            interface = argv[++i];
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            const char *log_file_path = argv[++i];
            log_init(log_file_path);
        } else {
            fprintf(stderr, "%sUnknown argument: %s%s\n", RED_TEXT, argv[i], RESET_TEXT);
            fprintf(stderr, "Usage: %s [-w wordlist] [-s ssid] [-u username] [-i interface] | optional: [-l log_file] [-v verbose console]\n", argv[0]);
            return 1;
        }
    }

    if (!wordlist || !ssid || !username || !interface) {
        fprintf(stderr, "%sMissing required arguments%s\n", RED_BOLD_TEXT, RESET_TEXT);
        fprintf(stderr, "Usage: %s [-w wordlist] [-s ssid] [-u username] [-i interface] | optional: [-l log_file] [-v verbose console]\n", argv[0]);
        fprintf(stderr, "Example: %s -w passwords.txt -s wifi-corp -u CONTOSO\\test -i wlan0 -v -l seahammer.log\n", argv[0]);
        return 1;
    }

    printf("Checking if user is root...\n");
    if (geteuid() != 0) {
        fprintf(stderr, "%sThis program must be run as root%s\n", RED_BOLD_TEXT, RESET_TEXT);
        return 1;
    } else {
        printf("%sRunning as root%s\n", GREEN_TEXT, RESET_TEXT);
    }

    if (file_exists(wordlist)) {
        if (verbose)
        printf("%sPassword file exists at %s%s\n", GREEN_TEXT, wordlist, RESET_TEXT);
    } else {
        fprintf(stderr, "%sPassword file does not exist%s\n", RED_TEXT, RESET_TEXT);
        return 1;
    }
    
    char wpa_socket[256];
    snprintf(wpa_socket, sizeof(wpa_socket), "/run/wpa_supplicant/%s", interface);

    struct wpa_ctrl *ctrl = wpa_ctrl_open(wpa_socket);
    if (!ctrl) {
        fprintf(stderr, "%sFailed to open wpa_supplicant socket%s\n", RED_TEXT, RESET_TEXT);
        exit_code = 1;
        goto cleanup;
    }

    struct wpa_ctrl *mon = wpa_ctrl_open(wpa_socket);
    if (!mon) {
        fprintf(stderr, "%sFailed to open wpa_supplicant socket for monitoring%s\n", RED_TEXT, RESET_TEXT);
        wpa_ctrl_close(ctrl);
        exit_code = 1;
        goto cleanup;
    }
    wpa_ctrl_attach(mon);
    printf("Clearing screen and starting seahammer...\n");
    sleep(1);
    printf(CLEAR_SCREEN);
    fflush(stdout);
    printf("\nStarting seahammer - brought to you by @dgoorden\n");
    printf("    github.com/dgoorden/seahammer\n");
    printf("A tool for brute-forcing WPA2 Enterprise networks using PEAP/MSCHAPv2\n");
    printf("ported from air-hammer-ng\n");
    printf("---------------------------------------------------------------\n");
    printf("Target SSID: %s\n", ssid);
    printf("Target username: %s\n", username);
    printf("Target interface: %s\n", interface);
    printf("Password file: %s\n", wordlist);
    printf("---------------------------------------------------------------\n\n");

    spray_passwords(ctrl, mon, wordlist, ssid, username);  

cleanup:
    if (mon) {
        wpa_ctrl_detach(mon);
        wpa_ctrl_close(mon);
    }

    if (ctrl) {
        wpa_ctrl_close(ctrl);
    }
    log_close();
    return exit_code;
}