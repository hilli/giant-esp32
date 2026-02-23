# Giant E-Bike BLE Protocol Reference

> **Target**: Giant Stormguard E+ 2 (2023) with RideControl+  
> **Source**: Reverse-engineered from [RideControl+ APK](https://play.google.com/store/apps/details?id=com.giant.ridecontrolapp) v1.32.1.0

## 1. Overview

Giant e-bikes with the RideControl+ system communicate over BLE GATT using a custom encrypted protocol. All commands and responses are AES-128-ECB encrypted 18-byte packets.

## 2. BLE Service

| UUID | Role |
|------|------|
| `4D500001-4745-5630-3031-E50E24DCCA9E` | Service |
| `4D500002-4745-5630-3031-E50E24DCCA9E` | Write (commands) |
| `4D500003-4745-5630-3031-E50E24DCCA9E` | Notify (responses) |

## 3. Packet Format

```
[0x21 header][16 AES-encrypted bytes][CRC]
```

- **Total**: 18 bytes
- **CRC**: XOR of all 16 encrypted bytes
- **Encryption**: AES-128-ECB with 16 possible keys (key index in plaintext byte 1)

### Encryption Keys

16 hardcoded AES-128 keys extracted from the decompiled APK. Key index 0 is the default; the key index is embedded in plaintext[1].

### Two-Stage Decryption

Responses use a two-stage decryption:
1. Decrypt with key 0 to read byte[1] (key index)
2. If key index ≠ 0, re-decrypt with the correct key

## 4. Command Structure

Plaintext (16 bytes):

| Byte | Field |
|------|-------|
| 0 | Command ID |
| 1 | AES key index |
| 2-15 | Command-specific data (zero-padded) |

## 5. Commands

| Command | ID | Key | Description |
|---------|----|-----|-------------|
| CONNECT_GEV | 0x02 | 0 | Initialize GEV protocol |
| DISCONNECT_GEV | 0x21 | 0 | Disconnect GEV protocol |
| READ_FACTORY_DATA | 0x03 | 0 | Read frame number, RC type/HW version |
| READ_RIDING_DATA | 0x1B | 0 | Read live ride telemetry |
| READ_TUNING_DATA | 0x2C | 0 | Read tuning parameters |
| READ_REMAINING_RANGE | 0x1D | 0 | Read estimated remaining range |
| READ_BATTERY | 0x13 | 0 | Read battery % (ACTIVE_DATA_ENERGY_PAK_1) |
| DIAGNOSTIC_ENERGY_PAK | 0x17 | 0 | Battery diagnostics (capacity, life, FW) |
| DIAGNOSTIC_SYNC_DRIVE | 0x16 | 0 | Motor diagnostics (type, FW, ODO) |
| TRIGGER_LIGHT | 0x1C | 3 | Toggle light (data[2]=0x08) |
| TRIGGER_ASSIST_UP | 0x1C | 3 | Assist level up (data[2]=0x02) |
| TRIGGER_ASSIST_DOWN | 0x1C | 3 | Assist level down (data[2]=0x01) |
| TRIGGER_POWER | 0x1C | 3 | Power toggle (data[2]=0x00, data[3]=0x08) |

## 6. Response Parsing

Response type is in decrypted byte[0].

### 0x1B — Riding Data

| Byte(s) | Field | Conversion |
|---------|-------|------------|
| 2-3 | Speed | big-endian / 10.0 → km/h |
| 4-5 | Crank RPM | big-endian / 10.0 |
| 6-7 | Torque | big-endian / 100.0 → Nm |
| 8-9 | Watt | big-endian / 10.0 |
| 10 | RSOC (battery %) | 0-100 |
| 11-12 | Distance | big-endian / 10.0 → km |
| 13-14 | Ride time | big-endian → seconds |
| 15 | Error code | |

### 0x03 — Factory Data

| Byte(s) | Field |
|---------|-------|
| 2-12 | Frame number (ASCII) |
| 13 | RC type |
| 14-15 | RC HW version |

### 0x13 — Battery (ACTIVE_DATA_ENERGY_PAK_1)

| Byte(s) | Field | Conversion |
|---------|-------|------------|
| 2 | Battery capacity % | 0-100 |
| 3 | Battery life/health % | 0-100 |
| 4-5 | Last charged full capacity | big-endian / 10.0 → Wh |

### 0x0E — Energy Pak Passive Data 2

| Byte(s) | Field | Conversion |
|---------|-------|------------|
| 2-3 | Charge cycle count | big-endian short |
| 4-5 | Total charge times | big-endian short |
| 6 | Discharge state | 0 likely = charging |

### 0x17 — Diagnostic EnergyPak

| Byte(s) | Field |
|---------|-------|
| 2 | Capacity % |
| 3 | Life % |
| 4-6 | EP firmware version |

### 0x16 — Diagnostic SyncDrive

| Byte(s) | Field |
|---------|-------|
| 2 | DU type |
| 3-5 | DU firmware version |
| 6-9 | Odometer (big-endian) |

### 0x1C — Trigger Response

| Byte(s) | Field |
|---------|-------|
| 2 | Action |
| 3 | Status |

### 0x1D — Remaining Range

| Byte(s) | Field | Conversion |
|---------|-------|------------|
| 2-3 | Range | big-endian → km |

## 7. Bike Data Commands (from APK)

The APK uses a separate set of "bike data" commands (PASSIVE_DATA / ACTIVE_DATA) with numeric IDs:

| Name | ID | Description |
|------|-----|-------------|
| PASSIVE_DATA_RIDE_CONTROL_1-4 | 5-8 | RC firmware, usage stats |
| PASSIVE_DATA_SYNC_DRIVE_1-4 | 9-12 | Motor firmware, service data |
| PASSIVE_DATA_ENERGY_PAK_1-4 | 13-16 | Battery firmware, charge stats |
| ACTIVE_DATA_RIDE_CONTROL_1 | 17 | RC active data |
| ACTIVE_DATA_SYNC_DRIVE_1 | 18 | ODO, total usage hours |
| ACTIVE_DATA_ENERGY_PAK_1 | 19 | Battery %, health, capacity |

## 8. References

- [Giant CAN bus RE](https://github.com/bobmorane83/Giant-CanBus) — CAN bus level data
- [Specialized Turbo BLE Protocol](https://github.com/JamieMagee/specialized-turbo/blob/main/docs/protocol.md) — similar project for reference
- [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)
- [BLE Reverse Engineering guide](https://reverse-engineering-ble-devices.readthedocs.io/)
