#pragma once

// BLE scan duration in seconds (0 = scan forever)
#define BLE_SCAN_DURATION 10

// Target device filter — set to "" to show all devices, or a substring to filter by name
// Examples: "Giant", "SG", "RideControl", "Smart Gateway"
#define BLE_TARGET_NAME_FILTER ""

// Target device by address — set to "" to ignore, or a specific MAC address
// Example: "AA:BB:CC:DD:EE:FF"
#define BLE_TARGET_ADDRESS ""

// Auto-connect to the first device matching the filter
#define BLE_AUTO_CONNECT true

// Subscribe to all notify-capable characteristics after connecting
#define BLE_AUTO_SUBSCRIBE true

// Serial baud rate
#define SERIAL_BAUD 115200

// mDNS hostname — access at http://MDNS_HOSTNAME.local
#define MDNS_HOSTNAME "giant-esp32"

// Ride mode — AP with full web UI when saved WiFi is unreachable
#define RIDE_AP_SSID "Giant-ESP32-Ride"
#define RIDE_AUTO_RECORD true
