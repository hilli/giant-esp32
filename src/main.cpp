// Giant E-Bike BLE Explorer
// Phase 1: Generic BLE GATT scanner/explorer for ESP32-S3
// Phase 2: Web UI for full browser-based control
//
// Usage:
//   1. Flash to ESP32-S3, open Serial Monitor at 115200
//   2. ESP32 connects to WiFi and starts a web server
//   3. Open the IP address shown in Serial Monitor in a browser
//   4. Serial commands still work: s=scan, c=connect, d=disconnect, r=read, n=notify, h=help

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "config.h"
#include "ble_explorer.h"
#include "web_server.h"
#include "giant_protocol.h"
#include "ride_logger.h"
#include "webhook.h"
#include "charge_monitor.h"
#include "status_led.h"

// Optional: compile-time WiFi fallback from credentials.h
#if __has_include("credentials.h")
#include "credentials.h"
#endif
#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

BLEExplorer explorer;
WebServer webServer(explorer);
GiantBike* giantBike = nullptr;
RideLogger rideLogger;
Webhook webhook;
ChargeMonitor chargeMonitor(webhook);
StatusLed statusLed;

// Track the best candidate during scanning
NimBLEAddress targetAddress;
bool targetFound = false;

class MyCallbacks : public BLEExplorerCallbacks {
    void onDeviceFound(const NimBLEAdvertisedDevice& device) override {
        // Store device with correct address type for later connection
        webServer.addScannedDevice(device);

        // Send to WebSocket
        String json = "{\"address\":\"" + String(device.getAddress().toString().c_str()) + "\"";
        json += ",\"name\":\"" + String(device.getName().c_str()) + "\"";
        json += ",\"rssi\":" + String(device.getRSSI());
        if (device.haveServiceUUID()) {
            json += ",\"services\":\"";
            for (int i = 0; i < device.getServiceUUIDCount(); i++) {
                if (i > 0) json += " ";
                json += device.getServiceUUID(i).toString().c_str();
            }
            json += "\"";
        }
        json += "}";
        webServer.sendEvent("scan_result", json);

        // Check if this device matches our filter
        String filter = BLE_TARGET_NAME_FILTER;
        String targetAddr = BLE_TARGET_ADDRESS;

        if (targetAddr.length() > 0) {
            if (String(device.getAddress().toString().c_str()) == targetAddr) {
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

    void onConnected(NimBLEClient* client) override {
        statusLed.setState(LedState::CONNECTED);
        webServer.sendEvent("connected", "{\"address\":\"" +
            String(client->getPeerAddress().toString().c_str()) + "\"}");

        // Check for Giant GEV service and initialize protocol
        NimBLERemoteService* gevSvc = client->getService(GEV_SERVICE_UUID);
        if (gevSvc) {
            Serial.println("[GEV] Giant E-Bike service found, initializing protocol...");
            if (giantBike) {
                webServer.setGiantBike(nullptr);
                delete giantBike;
            }
            giantBike = new GiantBike(client);
            if (giantBike->init()) {
                webServer.setGiantBike(giantBike);
                Serial.println("[GEV] Protocol ready via web UI");
            } else {
                Serial.println("[GEV] Protocol init failed");
                delete giantBike;
                giantBike = nullptr;
            }
        }
    }

    void onDisconnected(NimBLEClient* client, int reason) override {
        statusLed.setState(LedState::IDLE);
        if (giantBike) {
            webServer.setGiantBike(nullptr);
            delete giantBike;
            giantBike = nullptr;
            Serial.println("[GEV] Giant bike disconnected, cleaned up");
        }
        webServer.sendEvent("disconnected", "{\"reason\":" + String(reason) + "}");
    }

    void onNotification(NimBLERemoteCharacteristic* chr, uint8_t* data, size_t length) override {
        String hex;
        for (size_t i = 0; i < length; i++) {
            char buf[4];
            snprintf(buf, sizeof(buf), "%02x ", data[i]);
            hex += buf;
        }
        hex.trim();
        String json = "{\"uuid\":\"" + String(chr->getUUID().toString().c_str()) +
                       "\",\"hex\":\"" + hex +
                       "\",\"length\":" + String(length) + "}";
        webServer.sendEvent("notification", json);
    }

    void onScanEnd(uint32_t count) override {
        statusLed.setState(LedState::IDLE);
        webServer.sendEvent("scan_end", "{\"count\":" + String(count) + "}");
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
                statusLed.setState(LedState::SCANNING);
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
            NimBLEAddress addr(std::string(addrStr.c_str()), 0);
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

    // Disable task watchdog — BLE connect blocks for 10+ seconds
    esp_task_wdt_deinit();

    statusLed.init();
    statusLed.setState(LedState::BOOT);

    Serial.println();
    Serial.println("=========================================");
    Serial.println("  Giant E-Bike BLE Explorer v0.2");
    Serial.println("  ESP32-S3 + NimBLE + Web UI");
    Serial.println("=========================================");
    Serial.println();

    explorer.setCallbacks(&callbacks);
    explorer.init();

    // Start WiFi + Web Server
    webServer.begin(WIFI_SSID, WIFI_PASSWORD);

    // Only init subsystems when connected to WiFi
    if (!webServer.getWiFiManager().isAPMode()) {
        RideLogger::syncNTP();
        rideLogger.init();
        webServer.setRideLogger(&rideLogger);

        webhook.init();
        webServer.setWebhook(&webhook);
        webServer.setChargeMonitor(&chargeMonitor);

        printHelp();

        // Auto-start scan
        Serial.println("[BLE] Starting initial scan...\n");
        explorer.startScan(BLE_SCAN_DURATION);
        statusLed.setState(LedState::SCANNING);
    } else {
        Serial.println("[WiFi] In AP mode — configure WiFi via the captive portal");
        statusLed.setState(LedState::WIFI_AP);
    }
}

void loop() {
    processSerialCommand();
    webServer.loop();
    rideLogger.loop(giantBike);
    chargeMonitor.loop(giantBike);
    statusLed.loop();

    // Update LED for recording state
    if (rideLogger.isLogging() && statusLed.getState() != LedState::RECORDING) {
        statusLed.setState(LedState::RECORDING);
    } else if (!rideLogger.isLogging() && statusLed.getState() == LedState::RECORDING) {
        statusLed.setState(explorer.isConnected() ? LedState::CONNECTED : LedState::IDLE);
    }

    // Auto-connect if target found during scan
    if (targetFound && !explorer.isConnected() && !explorer.isScanning() && BLE_AUTO_CONNECT) {
        targetFound = false;
        Serial.printf("\n[BLE] Auto-connecting to %s...\n", targetAddress.toString().c_str());
        doConnect(targetAddress);
    }

    delay(10);
}
