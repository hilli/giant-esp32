# Giant E-Bike BLE Explorer & Charge Monitor

ESP32-S3 firmware to communicate with Giant e-bikes (Stormguard E+ 2 and similar models with Smart Gateway) over Bluetooth Low Energy.

**Status**: Phase 1 — BLE GATT Explorer (generic scanner/explorer, works with any BLE device)

Inspired by [JamieMagee/specialized-turbo](https://github.com/JamieMagee/specialized-turbo) and [Sepp62/LevoEsp32Ble](https://github.com/Sepp62/LevoEsp32Ble).

## Hardware

- **Board**: ESP32-S3-WROOM-1 (any ESP32-S3 dev board)
- **Target bike**: Giant Stormguard E+ 2 (2023) with Smart Gateway (Nordic nRF52832)

## Quick Start

### Prerequisites

```bash
# Install PlatformIO CLI
pip install platformio
```

### Build

```bash
pio run -e esp32s3
```

### Flash to ESP32-S3

1. Connect the ESP32-S3 via USB
2. If it's a new board, you may need to hold the **BOOT** button while pressing **RESET** to enter download mode
3. Flash the firmware:

```bash
pio run -e esp32s3 -t upload
```

PlatformIO auto-detects the serial port. If it doesn't, specify it:

```bash
pio run -e esp32s3 -t upload --upload-port /dev/cu.usbmodem*
```

### Monitor Serial Output

```bash
pio device monitor
```

Or build, flash, and monitor in one go:

```bash
pio run -e esp32s3 -t upload && pio device monitor
```

### Troubleshooting

- **Port not found**: Check `ls /dev/cu.usb*` — ESP32-S3 often shows as `/dev/cu.usbmodem*`
- **Upload fails**: Hold **BOOT**, press **RESET**, release **BOOT**, then retry upload
- **PlatformIO mirror blocked**: If `eu2.contabostorage.com` is blocked by your network, run:
  ```bash
  pio settings set enable_proxy_strict_ssl false
  ```

## Usage (Phase 1 — BLE Explorer)

1. Flash to ESP32-S3
2. Open Serial Monitor at 115200 baud
3. The device auto-scans for BLE devices on boot
4. Use Serial commands to interact:

| Command | Description |
|---------|-------------|
| `s` | Start a new BLE scan |
| `c AA:BB:CC:DD:EE:FF` | Connect to a device by address |
| `d` | Disconnect |
| `r` | Re-discover and read all services/characteristics |
| `n` | Subscribe to all notifications |
| `h` | Show help |

### Filtering

Edit `src/config.h` to set a name filter (e.g., `"Giant"`) or target address. When a filter is set, the explorer auto-connects to the first matching device.

### Output

The explorer prints detailed GATT information:

```
[SCAN] AA:BB:CC:DD:EE:FF | RSSI: -45 | Name: "Giant SG"
[BLE] Connected! MTU: 256
[SVC] UUID: 0x180f (Battery Service)
  [CHR] UUID: 0x2a19  Props: READ NOTIFY
  [CHR]   Value (1 bytes): 64  (100%)
[NTF] 0x2a19 (1 bytes): 64
```

## Project Phases

1. **BLE GATT Explorer** ✅ — Generic BLE scanner, connect, enumerate, subscribe
2. **Protocol Discovery** 🔜 — Run against Giant bike, document protocol in `docs/protocol.md`
3. **Battery Monitor** — Subscribe to battery, HTTP POST to NATS webhook on charge complete
4. **Polish** — WiFiManager, LED, deep sleep, OTA

## References

- [Giant CAN bus RE](https://github.com/bobmorane83/Giant-CanBus)
- [Specialized Turbo BLE protocol](https://github.com/JamieMagee/specialized-turbo/blob/main/docs/protocol.md)
- [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)
- [BLE Reverse Engineering guide](https://reverse-engineering-ble-devices.readthedocs.io/)

## License

MIT
