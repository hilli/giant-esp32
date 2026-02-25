#pragma once

#include <Arduino.h>

// ESP32-S3 onboard RGB LED (WS2812) — GPIO 48 on DevKitC-1, GPIO 38 on some variants
#define LED_PIN 38

enum class LedState {
    BOOT,       // white pulse
    WIFI_AP,    // yellow breathe
    RIDE_MODE,  // alternating green/blue blink
    IDLE,       // dim blue
    SCANNING,   // blue blink
    CONNECTED,  // solid green
    RECORDING,  // pulsing red
    ERROR       // fast red blink
};

class StatusLed {
public:
    void init();
    void setState(LedState state);
    LedState getState() const { return _state; }
    void loop();  // call from main loop for animations

private:
    LedState _state = LedState::BOOT;
    unsigned long _lastMs = 0;
    bool _on = false;
    uint8_t _brightness = 0;
    int8_t _breathDir = 1;

    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void off();
};
