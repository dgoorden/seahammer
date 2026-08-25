#include "connect.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int send_cmd(struct wpa_ctrl *ctrl, const char *cmd,
             char *reply, size_t reply_len) {
    size_t len = reply_len - 1;
    int ret = wpa_ctrl_request(ctrl, cmd, strlen(cmd), reply, &len, NULL);
    reply[len] = '\0';
    return ret;
}

int wifi_connect_peap(struct wpa_ctrl *ctrl,
                            const char *ssid,
                            const char *identity,
                            const char *password) {
    char cmd[512];
    char reply[1024];

    send_cmd(ctrl, "REMOVE_NETWORK all", reply, sizeof(reply));
    if (verbose)
        printf("\nRemoved all networks: %s\n", reply);

    send_cmd(ctrl, "ADD_NETWORK", reply, sizeof(reply));
    if (strncmp(reply, "FAIL", 4) == 0) return -1;
    int net_id = atoi(reply);

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid \"%s\"", net_id, ssid);
    send_cmd(ctrl, cmd, reply, sizeof(reply));

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt WPA-EAP", net_id);
    send_cmd(ctrl, cmd, reply, sizeof(reply));

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d eap PEAP", net_id);
    send_cmd(ctrl, cmd, reply, sizeof(reply));

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d phase2 \"auth=MSCHAPV2\"", net_id);
    send_cmd(ctrl, cmd, reply, sizeof(reply));

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d identity \"%s\"", net_id, identity);
    send_cmd(ctrl, cmd, reply, sizeof(reply));

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d password \"%s\"", net_id, password);
    send_cmd(ctrl, cmd, reply, sizeof(reply));

    snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %d", net_id);
    send_cmd(ctrl, cmd, reply, sizeof(reply));
    if (strncmp(reply, "OK", 2) != 0) return -1;

    return net_id;
}

int wifi_wait_for_connect(struct wpa_ctrl *mon, int timeout_in_secs) {
    char event[1024];
    size_t event_len;
    int fd = wpa_ctrl_get_fd(mon);
    int associated = 0;
    int eap_success = 0;

    while (wpa_ctrl_pending(mon)) {
        event_len = sizeof(event) - 1;
        wpa_ctrl_recv(mon, event, &event_len);
        event[event_len] = '\0';
    }
    
    for (int i = 0; i < timeout_in_secs; i++) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        struct timeval tv = {1,0};

        if (select(fd + 1, &rfds, NULL, NULL, &tv) > 0) {
            while (wpa_ctrl_pending(mon)) {
                event_len = sizeof(event) - 1;
                wpa_ctrl_recv(mon, event, &event_len);
                event[event_len] = '\0';

                if (verbose) printf("Event: %s\n", event);

                if (strstr(event, "Associated with")) 
                    associated = 1;

                if (strstr(event, "CTRL-EVENT-EAP-SUCCESS"))
                    eap_success = 1;

                if (strstr(event, "CTRL-EVENT-CONNECTED"))
                    return 0;

                if (associated && !eap_success && (
                    strstr(event, "CTRL-EVENT-AUTH-REJECT") ||
                    strstr(event, "CTRL-EVENT-DISCONNECTED") ||
                    strstr(event, "SSID-TEMP-DISABLED")))
                    return -1;
            }
        }
    }
    return -1;

}