#ifndef CONNECT_H
#define CONNECT_H

#include <stddef.h>
#include <wpa_ctrl.h>

int send_cmd(struct wpa_ctrl *ctrl, const char *cmd,
             char *reply, size_t reply_len);

int wifi_connect_peap(struct wpa_ctrl *ctrl,
                      const char *ssid,
                      const char *identity,
                      const char *password);

int wifi_wait_for_connect(struct wpa_ctrl *mon, int timeout_in_secs);

extern int verbose;

#endif