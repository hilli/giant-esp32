#include "ride_logger.h"

bool RideLogger::init() {
    if (!LittleFS.begin(true)) {  // true = format on fail
        Serial.println("[LOG] LittleFS mount failed");
        return false;
    }
    if (!LittleFS.exists(RIDE_DIR)) {
        LittleFS.mkdir(RIDE_DIR);
    }
    Serial.printf("[LOG] Storage: %u/%u KB used\n",
                  (unsigned)(usedBytes() / 1024), (unsigned)(totalBytes() / 1024));
    return true;
}

void RideLogger::syncNTP() {
    configTzTime("UTC", "pool.ntp.org", "time.google.com");
    Serial.print("[NTP] Syncing time");
    struct tm t;
    int tries = 0;
    while (!getLocalTime(&t, 500) && tries++ < 10) {
        Serial.print(".");
        delay(500);
    }
    if (tries < 10) {
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
        Serial.printf("\n[NTP] Time: %s UTC\n", buf);
    } else {
        Serial.println("\n[NTP] Sync failed, using uptime");
    }
}

String RideLogger::makeFilename() {
    struct tm t;
    char buf[32];
    if (getLocalTime(&t, 100)) {
        strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &t);
    } else {
        snprintf(buf, sizeof(buf), "ride_%lu", millis() / 1000);
    }
    return String(RIDE_DIR) + "/" + String(buf) + ".csv";
}

bool RideLogger::start() {
    if (_logging) return true;

    _currentFile = makeFilename();
    _file = LittleFS.open(_currentFile, "w");
    if (!_file) {
        Serial.printf("[LOG] Failed to create %s\n", _currentFile.c_str());
        return false;
    }

    // CSV header
    _file.println("timestamp,speed_kmh,cadence_rpm,torque_nm,watts,battery_pct,distance_km,ride_time_s,range_km,error");
    _file.flush();
    _sampleCount = 0;
    _lastSampleMs = 0;
    _logging = true;
    _needsRidingDataRequest = false;
    Serial.printf("[LOG] Recording started: %s\n", _currentFile.c_str());
    return true;
}

void RideLogger::stop() {
    if (!_logging) return;
    _file.close();
    _logging = false;
    Serial.printf("[LOG] Recording stopped: %d samples in %s\n", _sampleCount, _currentFile.c_str());
}

void RideLogger::loop(GiantBike* bike) {
    if (!_logging || !bike || !bike->isInitialized()) return;

    unsigned long now = millis();
    if (now - _lastSampleMs < _intervalMs) return;
    _lastSampleMs = now;

    // Request fresh riding data from bike, sample will be written on next interval
    // after the notify callback has updated the data
    if (_needsRidingDataRequest) {
        writeSample(bike->getRideData());
    }
    bike->sendCommand(READ_RIDING_DATA);
    _needsRidingDataRequest = true;
}

void RideLogger::writeSample(const GevRideData& d) {
    if (!_file) return;

    char ts[24];
    struct tm t;
    if (getLocalTime(&t, 10)) {
        strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", &t);
    } else {
        snprintf(ts, sizeof(ts), "%lu", millis() / 1000);
    }

    _file.printf("%s,%.1f,%.1f,%.2f,%.1f,%d,%.1f,%d,%d,%d\n",
                 ts, d.speed, d.crank, d.torque, d.watt,
                 d.rsoc, d.currentRidingDistance,
                 d.currentRidingTime, d.carr, d.errorCode);
    _sampleCount++;

    // Flush every 10 samples to balance performance vs data safety
    if (_sampleCount % 10 == 0) {
        _file.flush();
        Serial.printf("[LOG] %d samples, %.1f KB\n", _sampleCount, _file.size() / 1024.0f);
    }
}

std::vector<RideLogger::RideInfo> RideLogger::listRides() {
    std::vector<RideInfo> rides;
    File dir = LittleFS.open(RIDE_DIR);
    if (!dir || !dir.isDirectory()) return rides;

    File f = dir.openNextFile();
    while (f) {
        if (!f.isDirectory()) {
            RideInfo info;
            info.filename = f.name();
            info.size = f.size();
            // Count lines (minus header) for sample count
            info.samples = 0;
            while (f.available()) {
                if (f.read() == '\n') info.samples++;
            }
            if (info.samples > 0) info.samples--;  // subtract header
            rides.push_back(info);
        }
        f = dir.openNextFile();
    }
    return rides;
}

bool RideLogger::deleteRide(const String& filename) {
    String path = String(RIDE_DIR) + "/" + filename;
    if (!LittleFS.exists(path)) return false;
    return LittleFS.remove(path);
}
