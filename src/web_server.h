#pragma once

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <atomic>
#include "ble_explorer.h"

class WebServer {
public:
    WebServer(BLEExplorer& explorer, uint16_t port = 80);
    void begin(const char* ssid, const char* password);
    void sendEvent(const String& event, const String& data);

    // Call from loop() to process deferred BLE operations
    void loop();

private:
    void setupRoutes();
    void connectWiFi(const char* ssid, const char* password);

    // Route handlers
    void handleStatus(AsyncWebServerRequest* request);
    void handleScan(AsyncWebServerRequest* request);
    void handleDevices(AsyncWebServerRequest* request);
    void handleConnect(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total);
    void handleDisconnect(AsyncWebServerRequest* request);
    void handleServices(AsyncWebServerRequest* request);
    void handleSubscribe(AsyncWebServerRequest* request);

    BLEExplorer& m_explorer;
    AsyncWebServer m_server;
    AsyncWebSocket m_ws;

    // Pending BLE operations (run from loop() to avoid blocking async server)
    enum class PendingOp : int { NONE = 0, CONNECT = 1, DISCONNECT = 2, SUBSCRIBE = 3 };
    std::atomic<int> m_pendingOp{0};
    char m_pendingAddress[20] = {};  // MAC address, safe for cross-core
    String m_lastDebug;  // Last operation result for diagnostics

    // Discovered devices from last scan
    struct ScannedDevice {
        NimBLEAddress address;
        String name;
        int rssi;
        String services;
        String manufacturer;
    };
    std::vector<ScannedDevice> m_devices;

public:
    // Called from BLE scan callback to store devices with correct address type
    void addScannedDevice(const NimBLEAdvertisedDevice& device);
};
