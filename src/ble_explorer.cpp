#include "ble_explorer.h"

BLEExplorer* BLEExplorer::s_instance = nullptr;

BLEExplorer::BLEExplorer() {
    s_instance = this;
}

void BLEExplorer::init() {
    NimBLEDevice::init("GiantExplorer");
    NimBLEDevice::setSecurityAuth(true, true, true); // bonding, MITM, SC
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_DISPLAY);
    m_scan = NimBLEDevice::getScan();
    m_scan->setScanCallbacks(&m_scanCallbacks);
    m_scan->setActiveScan(true);
    m_scan->setInterval(100);
    m_scan->setWindow(99);
}

void BLEExplorer::setCallbacks(BLEExplorerCallbacks* cb) {
    m_callbacks = cb;
}

void BLEExplorer::startScan(uint32_t durationSeconds) {
    Serial.printf("[BLE] Starting scan for %u seconds...\n", durationSeconds);
    m_scan->start(durationSeconds * 1000, false);
}

void BLEExplorer::stopScan() {
    m_scan->stop();
    Serial.println("[BLE] Scan stopped");
}

bool BLEExplorer::isScanning() const {
    return m_scan->isScanning();
}

bool BLEExplorer::connectToDevice(const NimBLEAddress& address) {
    Serial.printf("[BLE] Connecting to %s...\n", address.toString().c_str());

    // Reuse existing client or create a new one
    m_client = NimBLEDevice::getClientByPeerAddress(address);
    if (!m_client) {
        m_client = NimBLEDevice::getDisconnectedClient();
    }
    if (!m_client) {
        m_client = NimBLEDevice::createClient();
    }
    m_client->setClientCallbacks(&m_clientCallbacks);
    m_client->setConnectTimeout(10);

    if (!m_client->connect(address)) {
        Serial.println("[BLE] Connection failed");
        m_client = nullptr;
        return false;
    }

    Serial.printf("[BLE] Connected! MTU: %u\n", m_client->getMTU());
    m_connected = true;
    return true;
}

void BLEExplorer::disconnect() {
    if (m_client && m_connected) {
        m_client->disconnect();
    }
}

bool BLEExplorer::isConnected() const {
    return m_connected && m_client && m_client->isConnected();
}

bool BLEExplorer::discoverServices() {
    if (!isConnected()) return false;

    Serial.println("[BLE] Discovering services...");
    const auto& services = m_client->getServices(true);
    if (services.empty()) {
        Serial.println("[BLE] Service discovery failed");
        return false;
    }

    Serial.printf("[BLE] Found %u services\n", (unsigned)services.size());

    for (auto* svc : services) {
        Serial.printf("\n[SVC] UUID: %s\n", svc->getUUID().toString().c_str());
        if (m_callbacks) m_callbacks->onServiceDiscovered(svc);

        const auto& chars = svc->getCharacteristics(true);

        for (auto* chr : chars) {
            String props = "";
            if (chr->canRead())      props += "READ ";
            if (chr->canWrite())     props += "WRITE ";
            if (chr->canWriteNoResponse()) props += "WRITE_NR ";
            if (chr->canNotify())    props += "NOTIFY ";
            if (chr->canIndicate())  props += "INDICATE ";

            Serial.printf("  [CHR] UUID: %s  Props: %s\n",
                          chr->getUUID().toString().c_str(), props.c_str());

            // Try to read the value if readable
            if (chr->canRead()) {
                NimBLEAttValue val = chr->readValue();
                if (val.size() > 0) {
                    Serial.printf("  [CHR]   Value (%u bytes): ", (unsigned)val.size());
                    for (size_t i = 0; i < val.size(); i++) {
                        Serial.printf("%02x ", val.data()[i]);
                    }
                    // Also try to print as string if it looks printable
                    bool printable = true;
                    for (size_t i = 0; i < val.size(); i++) {
                        if (val.data()[i] < 0x20 || val.data()[i] > 0x7e) {
                            printable = false;
                            break;
                        }
                    }
                    if (printable && val.size() > 0) {
                        Serial.printf(" (\"%.*s\")", (int)val.size(), val.data());
                    }
                    Serial.println();
                }
            }

            // Enumerate descriptors
            const auto& descs = chr->getDescriptors(true);
            {
                for (auto* desc : descs) {
                    Serial.printf("    [DSC] UUID: %s\n", desc->getUUID().toString().c_str());
                }
            }

            if (m_callbacks) m_callbacks->onCharacteristicDiscovered(chr);
        }
    }

    return true;
}

bool BLEExplorer::subscribeToNotifications() {
    if (!isConnected()) return false;

    Serial.println("\n[BLE] Subscribing to all notify-capable characteristics...");
    int count = 0;

    const auto& services = m_client->getServices(false);
    if (services.empty()) return false;

    for (auto* svc : services) {
        const auto& chars = svc->getCharacteristics(false);

        for (auto* chr : chars) {
            if (chr->canNotify()) {
                if (chr->subscribe(true, notifyCallback)) {
                    Serial.printf("[SUB] Subscribed to %s (service %s)\n",
                                  chr->getUUID().toString().c_str(),
                                  svc->getUUID().toString().c_str());
                    count++;
                } else {
                    Serial.printf("[SUB] Failed to subscribe to %s\n",
                                  chr->getUUID().toString().c_str());
                }
            }
            if (chr->canIndicate()) {
                if (chr->subscribe(false, notifyCallback)) {
                    Serial.printf("[SUB] Subscribed (indicate) to %s\n",
                                  chr->getUUID().toString().c_str());
                    count++;
                }
            }
        }
    }

    Serial.printf("[BLE] Subscribed to %d characteristics\n", count);
    return count > 0;
}

const std::vector<NimBLERemoteService*>& BLEExplorer::getServices() const {
    static const std::vector<NimBLERemoteService*> empty;
    if (!m_client) return empty;
    return m_client->getServices(false);
}

// --- Notification callback ---
void BLEExplorer::notifyCallback(NimBLERemoteCharacteristic* chr,
                                  uint8_t* data, size_t length, bool isNotify) {
    Serial.printf("[%s] %s (%u bytes): ",
                  isNotify ? "NTF" : "IND",
                  chr->getUUID().toString().c_str(),
                  (unsigned)length);
    for (size_t i = 0; i < length; i++) {
        Serial.printf("%02x ", data[i]);
    }
    Serial.println();

    if (s_instance && s_instance->m_callbacks) {
        s_instance->m_callbacks->onNotification(chr, data, length);
    }
}

// --- Scan callbacks ---
void BLEExplorer::ScanCallbacks::onResult(const NimBLEAdvertisedDevice* advertisedDevice) {
    Serial.printf("[SCAN] %s | RSSI: %d | Name: \"%s\"",
                  advertisedDevice->getAddress().toString().c_str(),
                  advertisedDevice->getRSSI(),
                  advertisedDevice->getName().c_str());

    // Print manufacturer data if present
    if (advertisedDevice->haveManufacturerData()) {
        auto mfr = advertisedDevice->getManufacturerData();
        Serial.printf(" | MFR (%u bytes): ", (unsigned)mfr.size());
        for (size_t i = 0; i < mfr.size(); i++) {
            Serial.printf("%02x ", (uint8_t)mfr[i]);
        }
        // Print as ASCII if printable
        bool printable = true;
        for (size_t i = 2; i < mfr.size(); i++) { // skip company ID
            if ((uint8_t)mfr[i] < 0x20 || (uint8_t)mfr[i] > 0x7e) {
                printable = false;
                break;
            }
        }
        if (printable && mfr.size() > 2) {
            Serial.printf(" (\"%.*s\")", (int)(mfr.size() - 2), mfr.c_str() + 2);
        }
    }

    // Print service UUIDs if advertised
    if (advertisedDevice->haveServiceUUID()) {
        Serial.printf(" | Services: ");
        for (int i = 0; i < advertisedDevice->getServiceUUIDCount(); i++) {
            Serial.printf("%s ", advertisedDevice->getServiceUUID(i).toString().c_str());
        }
    }

    Serial.println();

    if (m_parent->m_callbacks) {
        m_parent->m_callbacks->onDeviceFound(*advertisedDevice);
    }
}

void BLEExplorer::ScanCallbacks::onScanEnd(const NimBLEScanResults& results, int reason) {
    Serial.printf("[SCAN] Scan ended. Found %u devices. Reason: %d\n",
                  (unsigned)results.getCount(), reason);
    if (m_parent->m_callbacks) m_parent->m_callbacks->onScanEnd(results.getCount());
}

// --- Client callbacks ---
void BLEExplorer::ClientCallbacks::onConnect(NimBLEClient* client) {
    Serial.printf("[BLE] Connected to %s\n", client->getPeerAddress().toString().c_str());
    m_parent->m_connected = true;
    if (m_parent->m_callbacks) m_parent->m_callbacks->onConnected(client);
}

void BLEExplorer::ClientCallbacks::onDisconnect(NimBLEClient* client, int reason) {
    Serial.printf("[BLE] Disconnected. Reason: %d\n", reason);
    m_parent->m_connected = false;
    if (m_parent->m_callbacks) m_parent->m_callbacks->onDisconnected(client, reason);
}
