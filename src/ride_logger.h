#pragma once

#include <LittleFS.h>
#include <time.h>
#include "giant_protocol.h"

class RideLogger {
public:
    static constexpr const char* RIDE_DIR = "/rides";
    static constexpr unsigned long DEFAULT_INTERVAL_MS = 2000;

    bool init();
    bool start();
    void stop();
    bool isLogging() const { return _logging; }

    // Call from loop() — polls ride data and writes sample if interval elapsed
    void loop(GiantBike* bike);

    // Ride file management
    struct RideInfo {
        String filename;
        size_t size;
        int samples;
    };
    std::vector<RideInfo> listRides();
    bool deleteRide(const String& filename);
    String currentRideFile() const { return _currentFile; }

    // Storage stats
    size_t totalBytes() const { return LittleFS.totalBytes(); }
    size_t usedBytes() const { return LittleFS.usedBytes(); }

    void setSampleInterval(unsigned long ms) { _intervalMs = ms; }

    // Sync time via NTP (call after WiFi connected)
    static void syncNTP();

private:
    bool _logging = false;
    String _currentFile;
    File _file;
    unsigned long _intervalMs = DEFAULT_INTERVAL_MS;
    unsigned long _lastSampleMs = 0;
    int _sampleCount = 0;
    bool _needsRidingDataRequest = false;

    String makeFilename();
    void writeSample(const GevRideData& data);
};
