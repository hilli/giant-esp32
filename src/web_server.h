#pragma once

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <atomic>
#include "ble_explorer.h"
#include "giant_protocol.h"
#include "ride_logger.h"
#include "wifi_manager.h"
#include "webhook.h"
#include "charge_monitor.h"

class WebServer {
public:
    WebServer(BLEExplorer& explorer, uint16_t port = 80);
    void begin(const char* ssid, const char* password);
    void sendEvent(const String& event, const String& data);

    // Call from loop() to process deferred BLE operations
    void loop();

    // Access the WiFi manager
    WiFiManager& getWiFiManager() { return m_wifiManager; }

private:
    void setupRoutes();
    void setupPortalRoutes();

    // Route handlers
    void handleStatus(AsyncWebServerRequest* request);
    void handleScan(AsyncWebServerRequest* request);
    void handleDevices(AsyncWebServerRequest* request);
    void handleConnect(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total);
    void handleDisconnect(AsyncWebServerRequest* request);
    void handleServices(AsyncWebServerRequest* request);
    void handleSubscribe(AsyncWebServerRequest* request);
    void handleGiantStatus(AsyncWebServerRequest* request);
    void handleGiantCommand(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total);

    BLEExplorer& m_explorer;
    AsyncWebServer m_server;
    AsyncWebSocket m_ws;

    // Pending BLE operations (run from loop() to avoid blocking async server)
    enum class PendingOp : int { NONE = 0, CONNECT = 1, DISCONNECT = 2, SUBSCRIBE = 3, GIANT_CMD = 4 };
    std::atomic<int> m_pendingOp{0};
    char m_pendingAddress[20] = {};  // MAC address, safe for cross-core
    char m_pendingGiantCmd[32] = {};  // Giant command name, safe for cross-core
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

    // Giant bike integration
    void setGiantBike(GiantBike* bike) { m_giantBike = bike; }

    // Ride logger integration
    void setRideLogger(RideLogger* logger) { m_rideLogger = logger; }

    // Webhook & charge monitor integration
    void setWebhook(Webhook* hook) { m_webhook = hook; }
    void setChargeMonitor(ChargeMonitor* monitor) { m_chargeMonitor = monitor; }

private:
    GiantBike* m_giantBike = nullptr;
    RideLogger* m_rideLogger = nullptr;
    Webhook* m_webhook = nullptr;
    ChargeMonitor* m_chargeMonitor = nullptr;
    WiFiManager m_wifiManager;
};
