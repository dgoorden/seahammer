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

### wpa_ctrl dependency
`wpa_ctrl.c` and `wpa_ctrl.h` are included in `lib/` and pre-patched to compile standalone without the full hostap source tree. No additional steps needed.