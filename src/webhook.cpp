#include "webhook.h"
#include <HTTPClient.h>
#include <Preferences.h>

static const char* NVS_NAMESPACE = "webhook";
static const char* NVS_KEY_URL = "url";

void Webhook::init() {
    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, true);
    _url = prefs.getString(NVS_KEY_URL, "");
    prefs.end();
    if (_url.length() > 0) {
        Serial.printf("[HOOK] Webhook configured: %s\n", _url.c_str());
    }
}

void Webhook::setUrl(const String& url) {
    _url = url;
    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putString(NVS_KEY_URL, url);
    prefs.end();
    Serial.printf("[HOOK] Webhook URL saved: %s\n", url.c_str());
}

void Webhook::clearUrl() {
    _url = "";
    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.remove(NVS_KEY_URL);
    prefs.end();
    Serial.println("[HOOK] Webhook URL cleared");
}

bool Webhook::send(const String& jsonPayload) {
    if (_url.length() == 0) return false;

    HTTPClient http;
    for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
        http.begin(_url);
        http.addHeader("Content-Type", "application/json");
        int code = http.POST(jsonPayload);
        http.end();

        if (code >= 200 && code < 300) {
            Serial.printf("[HOOK] Sent OK (HTTP %d)\n", code);
            return true;
        }
        Serial.printf("[HOOK] Attempt %d failed (HTTP %d)\n", attempt + 1, code);
        if (attempt < MAX_RETRIES - 1) delay(RETRY_DELAY_MS * (attempt + 1));
    }
    Serial.println("[HOOK] All retries failed");
    return false;
}

bool Webhook::sendTest() {
    return send("{\"event\":\"test\",\"message\":\"Giant ESP32 webhook test\"}");
}
