#pragma once

#include <Arduino.h>
#include "giant_protocol.h"
#include "webhook.h"

class ChargeMonitor {
public:
    enum class State { UNKNOWN, NOT_CHARGING, CHARGING, CHARGED };

    ChargeMonitor(Webhook& webhook);

    // Call from main loop. Polls battery and detects state changes.
    void loop(GiantBike* bike);

    State getState() const { return _state; }
    int getBatteryPct() const { return _lastBatteryPct; }
    int getNotifyThreshold() const { return _notifyThreshold; }
    void setNotifyThreshold(int pct) { _notifyThreshold = pct; }
    bool isEnabled() const { return _enabled; }
    void setEnabled(bool en) { _enabled = en; }

    static const char* stateToString(State s);

private:
    void checkStateTransition(int batteryPct);
    void sendWebhookEvent(const char* event, int batteryPct);

    Webhook& _webhook;
    State _state = State::UNKNOWN;
    int _lastBatteryPct = -1;
    int _prevBatteryPct = -1;
    int _notifyThreshold = 100;  // notify when battery reaches this %
    bool _thresholdNotified = false;
    bool _enabled = true;
    unsigned long _lastPollMs = 0;
    unsigned long _pollIntervalMs = 30000;  // 30 seconds
    int _stableCount = 0;  // count of consecutive same-value readings
};
