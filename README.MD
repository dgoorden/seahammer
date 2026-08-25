# sea_hammer

A WPA2-Enterprise (PEAP/MSCHAPv2) credential sprayer written in C.
Ported from [air-hammer](https://github.com/Wh1t3Rh1n0/air-hammer).

## Usage
```bash
./sea_hammer -i wlan0 -s <ssid> -u <username> -w <wordlist> [-v] [-l logfile]
```

## Requirements
- Linux
- wpa_supplicant
- gcc

## Disclaimer
For use only on networks you own or have explicit permission to test.