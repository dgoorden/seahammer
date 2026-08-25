## Building from source

### Requirements
- Linux
- gcc
- make
- wpa_supplicant

### Steps
```bash
git clone https://github.com/dgoorden/seahammer.git
cd seahammer
make
```
## Download pre-compiled binary

Pre-compiled binaries for x86_64 Linux are available on the [Releases](https://github.com/dgoorden/seahammer/releases) page.

> **Note:** The binary requires wpa_supplicant to be running and the wireless interface to be up. See [setup](#setup) below.

### Quick start
```bash
wget https://github.com/dgoorden/seahammer/releases/download/v0.1/seahammer
chmod +x seahammer
sudo ./seahammer -i wlan0 -s <ssid> -u <username> -w <wordlist>
```

## wpa_ctrl dependency
`wpa_ctrl.c` and `wpa_ctrl.h` are included in `lib/` and pre-patched to compile standalone without the full hostap source tree. No additional steps needed.

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)