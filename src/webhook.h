#pragma once

#include <Arduino.h>

class Webhook {
public:
    // Load webhook URL from NVS
    void init();

    // Send a webhook POST with JSON payload. Returns true on success.
    bool send(const String& jsonPayload);

    // Send a test ping
    bool sendTest();

    // URL management (stored in NVS)
    void setUrl(const String& url);
    String getUrl() const { return _url; }
    bool hasUrl() const { return _url.length() > 0; }
    void clearUrl();

private:
    String _url;
    static const int MAX_RETRIES = 3;
    static const int RETRY_DELAY_MS = 2000;
};
