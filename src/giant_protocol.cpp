#include "giant_protocol.h"
#include "mbedtls/aes.h"
#include <cstring>

GiantBike* GiantBike::s_instance = nullptr;

// AES-128 key table (16 keys × 16 bytes) from decompiled RideControl+ APK
static const uint8_t AES_KEYS[16][16] = {
    {0x39,0xfa,0xd4,0xc3,0x93,0x42,0xae,0x41,0x42,0xa9,0xa7,0x77,0x89,0xa1,0x13,0xaf},
    {0x30,0xec,0x00,0xbd,0x96,0xf7,0x21,0x45,0xd8,0x46,0xb0,0x9a,0x87,0x29,0xa6,0x37},
    {0x6e,0x0d,0xe7,0xe3,0x04,0xae,0x67,0x2f,0xe4,0xa0,0xbc,0x3f,0xf5,0x04,0x4d,0x21},
    {0xb0,0xb9,0xc4,0x7a,0x62,0x67,0x67,0xd0,0x9d,0x40,0xe4,0x82,0xe2,0xd7,0x65,0xee},
    {0x5d,0x2c,0xb8,0xe0,0x04,0xb0,0x63,0x57,0xb0,0x75,0x92,0xf4,0xb2,0x61,0x84,0xc1},
    {0x0d,0x5e,0x2f,0x33,0x96,0x8a,0x63,0xee,0x5e,0xf1,0xfe,0x06,0x0e,0x29,0xce,0xf6},
    {0x58,0xed,0x11,0xd1,0xf8,0x82,0x82,0x22,0xe8,0x86,0x22,0x63,0x5b,0xc8,0x88,0xc1},
    {0x13,0xef,0x0a,0x98,0x51,0xff,0xf3,0x55,0x21,0xf2,0x06,0xc0,0xaa,0xd5,0xd6,0x06},
    {0x87,0x18,0xa0,0xef,0xea,0x5a,0xb7,0x35,0xec,0xbf,0x1d,0xa1,0xa2,0x39,0x19,0x8b},
    {0xa6,0x4c,0xd4,0x19,0x7a,0xe3,0x99,0x4c,0x19,0x1e,0xcc,0x98,0x26,0xb9,0x70,0x8d},
    {0xfa,0xac,0x80,0x64,0x4b,0xf8,0x46,0xdd,0xdf,0x7c,0xd0,0xfa,0x19,0x85,0xac,0x0b},
    {0x28,0x98,0xf9,0x81,0x44,0xb6,0xc3,0x09,0x64,0x06,0x7e,0xbf,0x27,0x15,0x6b,0x2b},
    {0x17,0xcb,0x16,0x36,0x14,0xab,0x6a,0xa3,0xe8,0x4d,0x26,0x87,0x4c,0x0f,0xd3,0x47},
    {0x2a,0xf5,0x57,0x69,0xae,0x8a,0xc8,0x0d,0x3b,0x45,0xad,0xaf,0x35,0xed,0xaa,0x06},
    {0xe7,0xc2,0x2e,0x96,0xb0,0x74,0x71,0x9c,0xcf,0x19,0x16,0x1c,0x69,0x41,0x79,0xf0},
    {0x96,0xb5,0xf6,0x8a,0xab,0xdf,0xe4,0xb8,0x7d,0x6e,0x65,0x67,0x51,0xcd,0xf3,0x9e}
};

GiantBike::GiantBike(NimBLEClient* client) : _client(client) {
    s_instance = this;
}

bool GiantBike::init() {
    if (!_client || !_client->isConnected()) {
        Serial.println("[GEV] Client not connected");
        return false;
    }

    NimBLERemoteService* svc = _client->getService(GEV_SERVICE_UUID);
    if (!svc) {
        Serial.println("[GEV] Service not found");
        return false;
    }

    _writeChr = svc->getCharacteristic(GEV_WRITE_UUID);
    if (!_writeChr) {
        Serial.println("[GEV] Write characteristic not found");
        return false;
    }

    _notifyChr = svc->getCharacteristic(GEV_NOTIFY_UUID);
    if (!_notifyChr) {
        Serial.println("[GEV] Notify characteristic not found");
        return false;
    }

    if (!_notifyChr->subscribe(true, notifyCallbackStatic)) {
        Serial.println("[GEV] Failed to subscribe to notifications");
        return false;
    }

    _initialized = true;
    Serial.println("[GEV] Protocol initialized");
    return true;
}

void GiantBike::notifyCallbackStatic(NimBLERemoteCharacteristic* chr,
                                     const uint8_t* data, size_t length, bool isNotify) {
    if (s_instance) {
        s_instance->onNotify(data, length);
    }
}

// AES-128-ECB encrypt a single 16-byte block
void GiantBike::aesEncrypt(const uint8_t* plaintext, uint8_t* ciphertext, uint8_t keyIndex) {
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_enc(&ctx, AES_KEYS[keyIndex & 0x0F], 128);
    mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, plaintext, ciphertext);
    mbedtls_aes_free(&ctx);
}

// AES-128-ECB decrypt a single 16-byte block
void GiantBike::aesDecrypt(const uint8_t* ciphertext, uint8_t* plaintext, uint8_t keyIndex) {
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_dec(&ctx, AES_KEYS[keyIndex & 0x0F], 128);
    mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, ciphertext, plaintext);
    mbedtls_aes_free(&ctx);
}

bool GiantBike::buildCommand(GiantCommand cmd, uint8_t* packet, size_t& packetLen) {
    uint8_t plaintext[16];
    memset(plaintext, 0, sizeof(plaintext));

    uint8_t cmdId = 0;
    uint8_t keyIndex = 0;

    switch (cmd) {
        case CONNECT_GEV:
            cmdId = 0x02; keyIndex = 0;
            break;
        case DISCONNECT_GEV:
            cmdId = 0x21; keyIndex = 0;
            break;
        case READ_FACTORY_DATA:
            cmdId = 0x03; keyIndex = 0;
            break;
        case READ_RIDING_DATA:
            cmdId = 0x1B; keyIndex = 0;
            break;
        case READ_TUNING_DATA:
            cmdId = 0x2C; keyIndex = 0;
            break;
        case READ_REMAINING_RANGE:
            cmdId = 0x1D; keyIndex = 0;
            break;
        case TRIGGER_LIGHT:
            cmdId = 0x1C; keyIndex = 3;
            plaintext[2] = 0x08;
            break;
        case TRIGGER_ASSIST_UP:
            cmdId = 0x1C; keyIndex = 3;
            plaintext[2] = 0x02;
            break;
        case TRIGGER_ASSIST_DOWN:
            cmdId = 0x1C; keyIndex = 3;
            plaintext[2] = 0x01;
            break;
        case TRIGGER_POWER:
            cmdId = 0x1C; keyIndex = 3;
            plaintext[2] = 0x00;
            plaintext[3] = 0x08;
            break;
        case DIAGNOSTIC_ENERGY_PAK:
            cmdId = 0x17; keyIndex = 0;
            break;
        case DIAGNOSTIC_SYNC_DRIVE:
            cmdId = 0x16; keyIndex = 0;
            break;
        case READ_BATTERY:
            cmdId = 0x13; keyIndex = 0;  // ACTIVE_DATA_ENERGY_PAK_1
            break;
        default:
            return false;
    }

    plaintext[0] = cmdId;
    plaintext[1] = keyIndex;

    // Encrypt the 16-byte plaintext block
    uint8_t encrypted[16];
    aesEncrypt(plaintext, encrypted, keyIndex);

    // Build packet: [0x21 header][16 encrypted bytes][CRC]
    packet[0] = 0x21;
    memcpy(&packet[1], encrypted, 16);

    // CRC = XOR of all 16 encrypted bytes
    uint8_t crc = 0;
    for (int i = 0; i < 16; i++) {
        crc ^= encrypted[i];
    }
    packet[17] = crc;
    packetLen = 18;

    return true;
}

bool GiantBike::sendCommand(GiantCommand cmd) {
    if (!_initialized || !_writeChr) {
        Serial.println("[GEV] Not initialized");
        return false;
    }

    uint8_t packet[18];
    size_t packetLen = 0;

    if (!buildCommand(cmd, packet, packetLen)) {
        Serial.println("[GEV] Failed to build command");
        return false;
    }

    return _writeChr->writeValue(packet, packetLen, false);
}

bool GiantBike::sendRawCommand(const uint8_t* data, size_t len) {
    if (!_initialized || !_writeChr) {
        Serial.println("[GEV] Not initialized");
        return false;
    }
    return _writeChr->writeValue(data, len, false);
}

void GiantBike::onNotify(const uint8_t* data, size_t len) {
    if (len < 18) {
        Serial.printf("[GEV] Short response: %d bytes\n", len);
        return;
    }

    // Verify CRC (XOR of the 16 encrypted bytes)
    uint8_t crc = 0;
    for (int i = 1; i <= 16; i++) {
        crc ^= data[i];
    }
    if (crc != data[17]) {
        Serial.printf("[GEV] CRC mismatch: got 0x%02X, expected 0x%02X\n", data[17], crc);
        return;
    }

    // Decrypt the 16-byte payload (key index is in decrypted byte[1])
    // First pass: try with key 0 to read the key index
    uint8_t decrypted[16];
    aesDecrypt(&data[1], decrypted, 0);

    // If key index is non-zero, re-decrypt with the correct key
    uint8_t keyIndex = decrypted[1];
    if (keyIndex != 0 && keyIndex < 16) {
        aesDecrypt(&data[1], decrypted, keyIndex);
    }

    parseResponse(decrypted, 16);
}

void GiantBike::parseResponse(const uint8_t* d, size_t len) {
    uint8_t responseType = d[0];

    switch (responseType) {
        case 0x02: // CONNECT response
            Serial.printf("[GEV] Connect response, status: 0x%02X\n", d[2]);
            break;

        case 0x1B: { // Riding data
            // Speed: bytes 2-3, big-endian, /10.0
            _rideData.speed = ((d[2] << 8) | d[3]) / 10.0f;
            // Crank RPM: bytes 4-5, big-endian, /10.0
            _rideData.crank = ((d[4] << 8) | d[5]) / 10.0f;
            // Torque: bytes 6-7, big-endian, /100.0
            _rideData.torque = ((d[6] << 8) | d[7]) / 100.0f;
            // Watt: bytes 8-9, big-endian
            _rideData.watt = ((d[8] << 8) | d[9]) / 10.0f;
            // RSOC (battery %): byte 10
            _rideData.rsoc = d[10];
            // Current riding distance: bytes 11-12, big-endian, /10.0
            _rideData.currentRidingDistance = ((d[11] << 8) | d[12]) / 10.0f;
            // Current riding time: bytes 13-14, big-endian (seconds)
            _rideData.currentRidingTime = (d[13] << 8) | d[14];
            // Error code: byte 15
            _rideData.errorCode = d[15];

            Serial.printf("[GEV] Ride: %.1fkm/h, crank=%.1f, torque=%.2f, watt=%.1f, batt=%d%%\n",
                          _rideData.speed, _rideData.crank, _rideData.torque,
                          _rideData.watt, _rideData.rsoc);
            break;
        }

        case 0x03: { // Factory data
            // Frame number from bytes 2-13 as ASCII string
            char frame[12];
            memcpy(frame, &d[2], 11);
            frame[11] = '\0';
            _factoryData.frameNumber = String(frame);
            _factoryData.rcType = d[13];
            // RC HW version from bytes 14-15
            _factoryData.rcHwVersion = String(d[14]) + "." + String(d[15]);

            Serial.printf("[GEV] Factory: frame=%s, rcType=%d, hwVer=%s\n",
                          _factoryData.frameNumber.c_str(),
                          _factoryData.rcType,
                          _factoryData.rcHwVersion.c_str());
            break;
        }

        case 0x17: { // Diagnostic EnergyPak
            _bikeData.epPercentageCapacity = d[2];
            _bikeData.epPercentageLife = d[3];
            // EP firmware version from bytes 4-6
            _bikeData.epVersion = String(d[4]) + "." + String(d[5]) + "." + String(d[6]);

            Serial.printf("[GEV] EnergyPak: cap=%d%%, life=%d%%, ver=%s\n",
                          _bikeData.epPercentageCapacity,
                          _bikeData.epPercentageLife,
                          _bikeData.epVersion.c_str());
            break;
        }

        case 0x16: { // Diagnostic SyncDrive
            _bikeData.duType = d[2];
            // DU firmware version from bytes 3-5
            _bikeData.syncDriveDuFwVersion = String(d[3]) + "." + String(d[4]) + "." + String(d[5]);
            // Odometer from bytes 6-9, big-endian
            _bikeData.odo = (d[6] << 24) | (d[7] << 16) | (d[8] << 8) | d[9];

            Serial.printf("[GEV] SyncDrive: type=%d, fw=%s, odo=%d\n",
                          _bikeData.duType,
                          _bikeData.syncDriveDuFwVersion.c_str(),
                          _bikeData.odo);
            break;
        }

        case 0x2C: // Tuning data
            Serial.printf("[GEV] Tuning data received (raw): ");
            for (int i = 0; i < 16; i++) Serial.printf("%02X ", d[i]);
            Serial.println();
            break;

        case 0x1C: // Trigger response (light/assist/power)
            Serial.printf("[GEV] Trigger response: action=0x%02X status=0x%02X\n", d[2], d[3]);
            break;

        case 0x1D: { // Remaining range
            _rideData.carr = (d[2] << 8) | d[3];
            Serial.printf("[GEV] Remaining range: %d km\n", _rideData.carr);
            break;
        }

        case 0x13: { // ACTIVE_DATA_ENERGY_PAK_1 (readBattery response)
            _bikeData.epPercentageCapacity = d[2] & 0xFF;
            _bikeData.epPercentageLife = d[3] & 0xFF;
            float lastChargedCap = ((d[4] << 8) | d[5]) / 10.0f;
            Serial.printf("[GEV] Battery: cap=%d%%, life=%d%%, lastCharged=%.1fWh\n",
                          _bikeData.epPercentageCapacity, _bikeData.epPercentageLife, lastChargedCap);
            break;
        }

        case 0x0E: { // PASSIVE_DATA_ENERGY_PAK_2
            _bikeData.epChargeCycles = (d[2] << 8) | d[3];
            _bikeData.epChargeTimes = (d[4] << 8) | d[5];
            _bikeData.epPercentageDischarge = d[6];
            Serial.printf("[GEV] EnergyPak2: cycles=%d, charges=%d, discharge=%d\n",
                          _bikeData.epChargeCycles, _bikeData.epChargeTimes,
                          _bikeData.epPercentageDischarge);
            break;
        }

        case 0x21: // Disconnect response
            Serial.println("[GEV] Disconnect acknowledged");
            break;

        default:
            Serial.printf("[GEV] Unknown response type 0x%02X: ", responseType);
            for (int i = 0; i < 16; i++) Serial.printf("%02X ", d[i]);
            Serial.println();
            break;
    }
}
