#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>
#include <Preferences.h>
#include <DNSServer.h>

static const char* NVS_NAMESPACE = "wifi";
static const char* NVS_KEY_SSID = "ssid";
static const char* NVS_KEY_PASS = "pass";
static const char* AP_SSID = "Giant-ESP32-Setup";
static const uint16_t DNS_PORT = 53;

static DNSServer dnsServer;

bool WiFiManager::isBootButtonHeld(unsigned long holdMs) {
    pinMode(0, INPUT_PULLUP);
    if (digitalRead(0) == HIGH) return false;

    Serial.println("[WiFi] BOOT button detected, hold for AP mode...");
    unsigned long start = millis();
    while (digitalRead(0) == LOW) {
        if (millis() - start >= holdMs) {
            Serial.println("[WiFi] BOOT button held — forcing AP mode");
            return true;
        }
        delay(50);
    }
    return false;
}

bool WiFiManager::begin(const char* fallbackSsid, const char* fallbackPassword) {
    // Check for forced AP mode via BOOT button
    if (isBootButtonHeld()) {
        startAPMode();
        return false;
    }

    // Try NVS credentials first
    String ssid, password;
    if (loadCredentials(ssid, password)) {
        Serial.printf("[WiFi] Trying saved network: %s\n", ssid.c_str());
        if (tryConnect(ssid, password)) return true;
        Serial.println("[WiFi] Saved credentials failed");
    }

    // Try fallback (from credentials.h)
    if (fallbackSsid && strlen(fallbackSsid) > 0) {
        Serial.printf("[WiFi] Trying fallback network: %s\n", fallbackSsid);
        if (tryConnect(String(fallbackSsid), String(fallbackPassword ? fallbackPassword : ""))) return true;
        Serial.println("[WiFi] Fallback credentials failed");
    }

    // All attempts failed — start AP mode
    startAPMode();
    return false;
}

bool WiFiManager::tryConnect(const String& ssid, const String& password, int maxRetries) {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(MDNS_HOSTNAME);
    WiFi.begin(ssid.c_str(), password.c_str());

    Serial.printf("[WiFi] Connecting to %s", ssid.c_str());
    for (int i = 0; i < maxRetries; i++) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
            return true;
        }
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[WiFi] Connection timed out");
    WiFi.disconnect(true);
    return false;
}

void WiFiManager::saveCredentials(const String& ssid, const String& password) {
    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putString(NVS_KEY_SSID, ssid);
    prefs.putString(NVS_KEY_PASS, password);
    prefs.end();
    Serial.printf("[WiFi] Credentials saved for: %s\n", ssid.c_str());
}

bool WiFiManager::loadCredentials(String& ssid, String& password) {
    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, true);
    ssid = prefs.getString(NVS_KEY_SSID, "");
    password = prefs.getString(NVS_KEY_PASS, "");
    prefs.end();
    return ssid.length() > 0;
}

void WiFiManager::clearCredentials() {
    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.remove(NVS_KEY_SSID);
    prefs.remove(NVS_KEY_PASS);
    prefs.end();
    Serial.println("[WiFi] Stored credentials cleared");
}

void WiFiManager::startAPMode() {
    m_apMode = true;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID);
    delay(100);

    // Captive portal: redirect all DNS queries to our IP
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

    Serial.println("[WiFi] ========================================");
    Serial.printf("[WiFi] AP Mode: Connect to \"%s\"\n", AP_SSID);
    Serial.printf("[WiFi] Portal: http://%s/\n", WiFi.softAPIP().toString().c_str());
    Serial.println("[WiFi] ========================================");
}

void WiFiManager::loop() {
    if (m_apMode) {
        dnsServer.processNextRequest();
    }
}
