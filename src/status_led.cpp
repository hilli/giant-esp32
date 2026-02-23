#include "status_led.h"

// RMT-based WS2812 driver using ESP32 neopixelWrite()
// neopixelWrite(pin, r, g, b) is built into ESP32 Arduino core >= 2.x

void StatusLed::init() {
    pinMode(LED_PIN, OUTPUT);
    off();
    Serial.println("[LED] Status LED initialized on GPIO 48");
}

void StatusLed::setColor(uint8_t r, uint8_t g, uint8_t b) {
    neopixelWrite(LED_PIN, r, g, b);
}

void StatusLed::off() {
    setColor(0, 0, 0);
    _on = false;
}

void StatusLed::setState(LedState state) {
    _state = state;
    _lastMs = 0;  // reset animation
    _brightness = 0;
    _breathDir = 1;
}

void StatusLed::loop() {
    unsigned long now = millis();

    switch (_state) {
        case LedState::BOOT:
            // White pulse — brief indicator at startup
            if (now - _lastMs > 30) {
                _lastMs = now;
                _brightness += _breathDir * 5;
                if (_brightness >= 60) _breathDir = -1;
                if (_brightness <= 0) { _breathDir = 1; _brightness = 0; }
                setColor(_brightness, _brightness, _brightness);
            }
            break;

        case LedState::WIFI_AP:
            // Yellow breathe
            if (now - _lastMs > 30) {
                _lastMs = now;
                _brightness += _breathDir * 3;
                if (_brightness >= 50) _breathDir = -1;
                if (_brightness <= 0) { _breathDir = 1; _brightness = 0; }
                setColor(_brightness, _brightness / 2, 0);
            }
            break;

        case LedState::IDLE:
            // Dim steady blue
            setColor(0, 0, 8);
            break;

        case LedState::SCANNING:
            // Blue blink 2Hz
            if (now - _lastMs > 250) {
                _lastMs = now;
                _on = !_on;
                if (_on) setColor(0, 0, 40); else off();
            }
            break;

        case LedState::CONNECTED:
            // Solid green
            setColor(0, 30, 0);
            break;

        case LedState::RECORDING:
            // Pulsing red
            if (now - _lastMs > 25) {
                _lastMs = now;
                _brightness += _breathDir * 2;
                if (_brightness >= 50) _breathDir = -1;
                if (_brightness <= 4) { _breathDir = 1; _brightness = 4; }
                setColor(_brightness, 0, 0);
            }
            break;

        case LedState::ERROR:
            // Fast red blink 5Hz
            if (now - _lastMs > 100) {
                _lastMs = now;
                _on = !_on;
                if (_on) setColor(50, 0, 0); else off();
            }
            break;
    }
}
