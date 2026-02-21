# Giant E-Bike BLE Protocol Reference

> **Status**: Not yet documented — requires running BLE explorer against the bike  
> **Target**: Giant Stormguard E+ 2 (2023) with Smart Gateway

## 1. Overview

Giant e-bikes with the Smart Gateway module (Nordic nRF52832) communicate over BLE GATT.
The protocol is proprietary and undocumented. This document will be filled in as we
reverse-engineer the protocol by connecting to the bike with the BLE Explorer firmware.

## 2. BLE Discovery

### Advertising Data

TODO: Document after scanning the bike

| Field | Value |
|-------|-------|
| Device Name | ? |
| Company ID | ? |
| Manufacturer Data | ? |
| Advertised Services | ? |

### Detection Algorithm

```python
def is_giant_bike(device) -> bool:
    # TODO: Fill in after scanning
    pass
```

## 3. UUID Structure

TODO: Document after connecting to the bike

### Services

| Purpose | UUID |
|---------|------|
| ? | ? |

### Characteristics

| Service | Characteristic UUID | Properties | Purpose |
|---------|-------------------|------------|---------|
| ? | ? | ? | ? |

## 4. Authentication / Pairing

TODO: Determine if pairing is required and what method is used

## 5. Message Format

TODO: Analyze notification data to determine message structure

## 6. Data Fields

### Battery

| Channel | Name | Size | Conversion | Unit |
|---------|------|------|------------|------|
| ? | State of Charge | ? | ? | % |

## 7. References

- [Specialized Turbo BLE Protocol](https://github.com/JamieMagee/specialized-turbo/blob/main/docs/protocol.md) — similar project for reference
- [Giant CAN bus RE](https://github.com/bobmorane83/Giant-CanBus) — CAN bus level data
- [Giant Smart Gateway manual](https://usermanual.wiki/GIANT/SG10S-5242620.pdf)
