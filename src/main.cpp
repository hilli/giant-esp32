// Giant E-Bike BLE Explorer
// Phase 1: Generic BLE GATT scanner/explorer for ESP32-S3
//
// Usage:
//   1. Flash to ESP32-S3, open Serial Monitor at 115200
//   2. Scans for all BLE devices and prints info
//   3. Send 'c XX:XX:XX:XX:XX:XX' via Serial to connect to a device
//   4. Send 's' to start a new scan
//   5. Send 'd' to disconnect
//   6. Send 'r' to re-read all characteristics
//   7. Send 'n' to subscribe to all notifications

#include <Arduino.h>
#include "config.h"
#include "ble_explorer.h"

BLEExplorer explorer;

// Track the best candidate during scanning
NimBLEAddress targetAddress;
bool targetFound = false;

class MyCallbacks : public BLEExplorerCallbacks {
    void onDeviceFound(const NimBLEAdvertisedDevice& device) override {
        // Check if this device matches our filter
        String filter = BLE_TARGET_NAME_FILTER;
        String targetAddr = BLE_TARGET_ADDRESS;

        if (targetAddr.length() > 0) {
            if (device.getAddress().toString().c_str() == targetAddr) {
                targetAddress = device.getAddress();
                targetFound = true;
                Serial.printf("[MATCH] Target device found by address: %s\n",
                              targetAddr.c_str());
            }
        } else if (filter.length() > 0) {
            String name = device.getName().c_str();
            name.toLowerCase();
            String filterLower = filter;
            filterLower.toLowerCase();
            if (name.indexOf(filterLower) >= 0) {
                targetAddress = device.getAddress();
                targetFound = true;
                Serial.printf("[MATCH] Target device found by name: \"%s\"\n",
                              device.getName().c_str());
            }
        }
    }
};

MyCallbacks callbacks;

void printHelp() {
    Serial.println();
    Serial.println("=== Giant E-Bike BLE Explorer ===");
    Serial.println("Commands (via Serial):");
    Serial.println("  s           - Start BLE scan");
    Serial.println("  c <address> - Connect to device (e.g., c AA:BB:CC:DD:EE:FF)");
    Serial.println("  d           - Disconnect");
    Serial.println("  r           - Re-discover and read all services/characteristics");
    Serial.println("  n           - Subscribe to all notifications");
    Serial.println("  h           - Show this help");
    Serial.println();
}

void doConnect(const NimBLEAddress& addr) {
    if (explorer.isScanning()) {
        explorer.stopScan();
        delay(500);
    }

    if (explorer.connectToDevice(addr)) {
        explorer.discoverServices();
        if (BLE_AUTO_SUBSCRIBE) {
            explorer.subscribeToNotifications();
        }
        Serial.println("\n[BLE] Ready. Notifications will be printed as they arrive.");
        Serial.println("[BLE] Send 'h' for help.\n");
    }
}

void processSerialCommand() {
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() == 0) return;

    char c = cmd.charAt(0);

    switch (c) {
        case 's':
        case 'S':
            if (explorer.isConnected()) {
                Serial.println("[BLE] Disconnect first (send 'd')");
            } else {
                targetFound = false;
                explorer.startScan(BLE_SCAN_DURATION);
            }
            break;

        case 'c':
        case 'C': {
            String addrStr = cmd.substring(1);
            addrStr.trim();
            if (addrStr.length() < 17) {
                Serial.println("[ERR] Usage: c AA:BB:CC:DD:EE:FF");
                break;
            }
            NimBLEAddress addr(addrStr.c_str());
            doConnect(addr);
            break;
        }

        case 'd':
        case 'D':
            explorer.disconnect();
            Serial.println("[BLE] Disconnected");
            break;

        case 'r':
        case 'R':
            if (explorer.isConnected()) {
                explorer.discoverServices();
            } else {
                Serial.println("[ERR] Not connected");
            }
            break;

        case 'n':
        case 'N':
            if (explorer.isConnected()) {
                explorer.subscribeToNotifications();
            } else {
                Serial.println("[ERR] Not connected");
            }
            break;

        case 'h':
        case 'H':
            printHelp();
            break;

        default:
            Serial.printf("[ERR] Unknown command: '%c'. Send 'h' for help.\n", c);
            break;
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(2000); // Give Serial time to connect

    Serial.println();
    Serial.println("=========================================");
    Serial.println("  Giant E-Bike BLE Explorer v0.1");
    Serial.println("  ESP32-S3 + NimBLE");
    Serial.println("=========================================");
    Serial.println();

    explorer.setCallbacks(&callbacks);
    explorer.init();

    printHelp();

    // Auto-start scan
    Serial.println("[BLE] Starting initial scan...\n");
    explorer.startScan(BLE_SCAN_DURATION);
}

void loop() {
    processSerialCommand();

    // Auto-connect if target found during scan
    if (targetFound && !explorer.isConnected() && !explorer.isScanning() && BLE_AUTO_CONNECT) {
        targetFound = false;
        Serial.printf("\n[BLE] Auto-connecting to %s...\n", targetAddress.toString().c_str());
        doConnect(targetAddress);
    }

    delay(10);
}
