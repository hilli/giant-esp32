#include "charge_monitor.h"

ChargeMonitor::ChargeMonitor(Webhook& webhook) : _webhook(webhook) {}

const char* ChargeMonitor::stateToString(State s) {
    switch (s) {
        case State::UNKNOWN: return "unknown";
        case State::NOT_CHARGING: return "not_charging";
        case State::CHARGING: return "charging";
        case State::CHARGED: return "charged";
        default: return "unknown";
    }
}

void ChargeMonitor::loop(GiantBike* bike) {
    if (!_enabled || !bike || !bike->isInitialized()) return;

    unsigned long now = millis();
    if (now - _lastPollMs < _pollIntervalMs) return;
    _lastPollMs = now;

    // Request battery data
    bike->sendCommand(READ_BATTERY);
    delay(500);  // Wait for notify response

    int batteryPct = bike->getBikeData().epPercentageCapacity;
    if (batteryPct < 0 || batteryPct > 100) return;  // Invalid reading

    _lastBatteryPct = batteryPct;
    checkStateTransition(batteryPct);
}

void ChargeMonitor::checkStateTransition(int batteryPct) {
    State prevState = _state;

    if (_prevBatteryPct < 0) {
        // First reading — can't determine direction yet
        _prevBatteryPct = batteryPct;
        return;
    }

    if (batteryPct > _prevBatteryPct) {
        // Battery increasing = charging
        _state = State::CHARGING;
        _stableCount = 0;
    } else if (batteryPct == _prevBatteryPct) {
        _stableCount++;
        // After 3 stable readings while in CHARGING state → CHARGED
        if (_state == State::CHARGING && batteryPct >= 99 && _stableCount >= 3) {
            _state = State::CHARGED;
        }
    } else {
        // Battery decreasing = not charging
        _state = State::NOT_CHARGING;
        _stableCount = 0;
        _thresholdNotified = false;
    }

    _prevBatteryPct = batteryPct;

    // Notify on state transitions
    if (_state != prevState) {
        Serial.printf("[CHG] State: %s → %s (batt=%d%%)\n",
                      stateToString(prevState), stateToString(_state), batteryPct);

        if (_state == State::CHARGING && prevState != State::CHARGING) {
            sendWebhookEvent("charging_started", batteryPct);
        } else if (_state == State::CHARGED) {
            sendWebhookEvent("charge_complete", batteryPct);
        } else if (_state == State::NOT_CHARGING && prevState == State::CHARGING) {
            sendWebhookEvent("charging_stopped", batteryPct);
        }
    }

    // Threshold notification (only once per charge cycle)
    if (_state == State::CHARGING && !_thresholdNotified &&
        batteryPct >= _notifyThreshold && _notifyThreshold < 100) {
        _thresholdNotified = true;
        sendWebhookEvent("threshold_reached", batteryPct);
    }
}

void ChargeMonitor::sendWebhookEvent(const char* event, int batteryPct) {
    String json = "{\"event\":\"" + String(event) + "\""
                  ",\"battery_pct\":" + String(batteryPct) +
                  ",\"state\":\"" + String(stateToString(_state)) + "\""
                  "}";
    _webhook.send(json);
}
