
typedef struct {
    char ssid[32];
    char bssid[18];
    int frequency;
    char username[32];
    char password[64];
    int security;
} wifi_connect;