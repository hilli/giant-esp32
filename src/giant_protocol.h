#pragma once

#include <NimBLEDevice.h>

// GEV BLE Service and Characteristic UUIDs
static NimBLEUUID GEV_SERVICE_UUID("4D500001-4745-5630-3031-E50E24DCCA9E");
static NimBLEUUID GEV_WRITE_UUID("4D500002-4745-5630-3031-E50E24DCCA9E");
static NimBLEUUID GEV_NOTIFY_UUID("4D500003-4745-5630-3031-E50E24DCCA9E");

enum GiantCommand {
    CONNECT_GEV,
    DISCONNECT_GEV,
    READ_RIDING_DATA,
    READ_FACTORY_DATA,
    READ_TUNING_DATA,
    TRIGGER_LIGHT,
    TRIGGER_ASSIST_UP,
    TRIGGER_ASSIST_DOWN,
    TRIGGER_POWER,
    DIAGNOSTIC_ENERGY_PAK,
    DIAGNOSTIC_SYNC_DRIVE,
    READ_REMAINING_RANGE
};

struct GevRideData {
    float speed = 0;
    float crank = 0;
    float torque = 0;
    float watt = 0;
    float currentRidingDistance = 0;
    int rsoc = 0;
    int currentRidingTime = 0;
    int carr = 0;
    uint8_t errorCode = 0;
};

struct GevBikeData {
    int odo = 0;
    int epPercentageCapacity = 0;
    int epPercentageLife = 0;
    String rideControlUIFwVer;
    String syncDriveDuFwVersion;
    String epVersion;
    uint8_t duType = 0;
};

struct GevFactoryData {
    String frameNumber;
    String rcHwVersion;
    uint8_t rcType = 0;
};

class GiantBike {
public:
    GiantBike(NimBLEClient* client);

    bool init();
    bool sendCommand(GiantCommand cmd);
    bool sendRawCommand(const uint8_t* data, size_t len);
    void onNotify(const uint8_t* data, size_t len);

    GevRideData getRideData() const { return _rideData; }
    GevBikeData getBikeData() const { return _bikeData; }
    GevFactoryData getFactoryData() const { return _factoryData; }
    bool isInitialized() const { return _initialized; }

private:
    NimBLEClient* _client;
    NimBLERemoteCharacteristic* _writeChr = nullptr;
    NimBLERemoteCharacteristic* _notifyChr = nullptr;
    bool _initialized = false;

    GevRideData _rideData;
    GevBikeData _bikeData;
    GevFactoryData _factoryData;

    void aesEncrypt(const uint8_t* plaintext, uint8_t* ciphertext, uint8_t keyIndex);
    void aesDecrypt(const uint8_t* ciphertext, uint8_t* plaintext, uint8_t keyIndex);
    bool buildCommand(GiantCommand cmd, uint8_t* packet, size_t& packetLen);
    void parseResponse(const uint8_t* decrypted, size_t len);

    static void notifyCallbackStatic(NimBLERemoteCharacteristic* chr,
                                     const uint8_t* data, size_t length, bool isNotify);
    static GiantBike* s_instance;
};
