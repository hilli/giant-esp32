# Copilot Instructions

## Build & Flash

```bash
pio run                                              # Build firmware
pio run -e esp32s3 -t upload --upload-port /dev/cu.usbserial-410  # Flash
pio device monitor --port /dev/cu.usbserial-410      # Serial monitor
```

Or use [Task](https://taskfile.dev/): `task build`, `task flash`, `task monitor`, `task deploy` (flash + monitor).

There are no unit tests or linters configured.

## Architecture

ESP32-S3 Arduino firmware with four modules orchestrated by `main.cpp`:

- **BLEExplorer** (`ble_explorer.*`) — BLE scanning, GATT service/characteristic discovery, notification subscriptions. Singleton pattern with static callbacks via `s_instance`.
- **GiantBike** (`giant_protocol.*`) — Giant RideControl+ GEV protocol: AES-128-ECB encrypted 18-byte packets over BLE. Handles command encoding, response decoding, and two-stage key decryption.
- **WiFiManager** (`wifi_manager.*`) — WiFi connection with NVS credential storage, AP mode fallback with captive portal, BOOT button (GPIO0) override.
- **WebServer** (`web_server.*`) — ESPAsyncWebServer REST API + WebSocket for live UI updates. Defers all BLE operations to `main loop()` via `std::atomic<int>` flags to avoid blocking the async HTTP task. Serves captive portal in AP mode.
- **RideLogger** (`ride_logger.*`) — Polling-based CSV ride data recorder on LittleFS (`/rides/` directory). Flushes every 10 samples.

The web UI is a single-page app embedded as a `PROGMEM` string literal in `web_ui.h`. The captive portal UI is in `portal_ui.h`.

### WiFi Connection Flow

```
Boot → Check BOOT button (GPIO0 held 2s → AP mode)
     → Try NVS stored credentials
     → Try credentials.h fallback
     → All failed → AP mode with captive portal
```

In AP mode, the device creates a `Giant-ESP32-Setup` network with a config portal. Once credentials are saved to NVS, the device reboots and connects. `credentials.h` is optional — only needed if NVS is empty and AP mode isn't used.

### Data Flow

```
BLE notify → GiantBike::handleNotification() → parsed data structs
                                                     ↓
main loop() → RideLogger::loop() polls data → CSV write
           → WebServer::loop() executes deferred BLE ops
           → WebSocket broadcasts live data to browser
```

HTTP handlers never call BLE directly — they set atomic flags that `WebServer::loop()` (called from the main Arduino loop) picks up and executes.

## Key Conventions

- **Serial log prefixes**: `[BLE]`, `[GEV]`, `[WiFi]`, `[LOG]` — use these consistently.
- **Member naming**: `m_` prefix in BLEExplorer/WebServer, `_` prefix in GiantProtocol.
- **NimBLE v2 API**: `getServices()`/`getCharacteristics()` return vectors by reference (not pointers). Use `createClient()` then `connect(address)` separately — never `createClient(address)`. Don't call `deleteClient()` on failed connections.
- **ESPAsyncWebServer routing**: Register specific routes (e.g., `/api/rides/download`) before general ones (e.g., `/api/rides`) — the server uses prefix matching.
- **ArduinoJson v7**: Use `JsonDocument` (not `StaticJsonDocument` or `DynamicJsonDocument`).
- **WiFi credentials**: Stored in NVS via `Preferences`. Fallback to `src/credentials.h` (gitignored, optional). Configurable via AP mode captive portal or `POST /api/wifi/config`.
- **mDNS hostname**: Defined as `MDNS_HOSTNAME` in `src/config.h`. Device accessible at `http://<hostname>.local`.
- **OTA updates**: Enabled via ArduinoOTA when in STA mode. Flash with `pio run -t upload --upload-port <ip_address>`.

## Gotchas

- The Arduino task watchdog is disabled in `setup()` because BLE connect blocks for 10+ seconds.
- Giant protocol uses two-stage decryption: first decrypt with key 0 to read the key index, then re-decrypt with the actual key if index ≠ 0.
- BLE address type (public/random) may need to be flipped on connection failure — the web server retries with the opposite type.
- `GiantBike` instances are deleted and recreated on each new Giant BLE connection.
