# Sea Hammer

A WPA2-Enterprise (PEAP/MSCHAPv2) credential sprayer written in C, ported from [air-hammer-ng](https://github.com/dgoorden/air-hammer-ng).

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)

## Disclaimer

Sea Hammer is intended for use only on networks you own or have explicit written permission to test. Unauthorised use is illegal. The author accepts no liability for misuse.

---

## Usage

```bash
sudo ./seahammer -i <interface> -s <ssid> -u <username> -w <wordlist> [-v] [-l logfile]
```

### Options

| Flag | Description |
|---|---|
| `-i` | Wireless interface (e.g. wlan0) |
| `-s` | Target SSID |
| `-u` | Username (e.g. DOMAIN\\user) |
| `-w` | Path to wordlist |
| `-v` | Verbose output (shows wpa_supplicant events) |
| `-l` | Log file path (optional) |

### Example

```bash
sudo ./seahammer -i wlan0 -s wifi-corp -u "CONTOSO\\test" -w rockyou.txt -l results.log
```

---

## Installation

### Pre-compiled binary

Pre-compiled binaries for x86_64 Linux are available on the [Releases](https://github.com/dgoorden/seahammer/releases) page.

```bash
wget https://github.com/dgoorden/seahammer/releases/download/v0.1/seahammer
chmod +x seahammer
sudo ./seahammer -i wlan0 -s <ssid> -u <username> -w <wordlist>
```

### Build from source

#### Requirements

- Linux
- gcc
- make
- wpa_supplicant

#### Steps

```bash
git clone https://github.com/dgoorden/seahammer.git
cd seahammer
make
```

---

## Dependencies

`wpa_ctrl.c` and `wpa_ctrl.h` are included in `lib/` and pre-patched to compile standalone without the full hostap source tree. No additional steps needed.

---