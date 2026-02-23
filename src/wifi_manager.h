#pragma once

#include <Arduino.h>

// WiFi credential storage (NVS) and AP mode fallback
class WiFiManager {
public:
    // Try connecting with NVS credentials, then fallback defines, then AP mode.
    // Returns true if connected to STA, false if in AP mode.
    bool begin(const char* fallbackSsid = nullptr, const char* fallbackPassword = nullptr);

    // Save credentials to NVS
    void saveCredentials(const String& ssid, const String& password);

    // Clear stored credentials from NVS
    void clearCredentials();

    // Load credentials from NVS. Returns true if found.
    bool loadCredentials(String& ssid, String& password);

    // Start AP mode with captive portal
    void startAPMode();

    // Is the device currently in AP mode?
    bool isAPMode() const { return m_apMode; }

    // Check if BOOT button (GPIO0) is held during startup
    static bool isBootButtonHeld(unsigned long holdMs = 2000);

    // Handle DNS in AP mode (call from loop)
    void loop();

private:
    bool tryConnect(const String& ssid, const String& password, int maxRetries = 30);
    bool m_apMode = false;
};
