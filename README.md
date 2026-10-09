# harbour-pm5

Proxmark control for Sailfish OS. Drives a Proxmark over USB (CDC-ACM) from the
phone and speaks the device wire protocol directly over the serial port — no
client binary. Adapts to the firmware: the ISO 14443-A reader uses the NG frame
format on new firmware and the legacy MIX format on old firmware.

## Features

- **LF 125 kHz** — EM4100 / HID Prox / IO Prox search; EM4100 → T5577 clone.
- **HF 13.56 MHz (ISO 14443-A)** — UID / ATQA / SAK / ATS, EMV application
  discovery, on-card transaction log where the card releases it, offline-PIN
  attempt (only when ≥ 2 PIN tries remain, so the card cannot be blocked).
- **MIFARE Classic** — default-key check and full dump.
- **Terminal** — custom APDUs and raw wire commands with readable decoding.
- **Emulation** — present a chosen UID as a 14443-A tag.
- **Export** — scan + log as TXT, CSV or JSON.

## Build

    mb2 --target <sailfish-target> build

## License

GPLv3. See `LICENSE`.
