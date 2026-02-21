#pragma once

#include <NimBLEDevice.h>

// Callback interface for explorer events
class BLEExplorerCallbacks {
public:
    virtual ~BLEExplorerCallbacks() = default;
    virtual void onDeviceFound(const NimBLEAdvertisedDevice& device) {}
    virtual void onConnected(NimBLEClient* client) {}
    virtual void onDisconnected(NimBLEClient* client, int reason) {}
    virtual void onServiceDiscovered(NimBLERemoteService* service) {}
    virtual void onCharacteristicDiscovered(NimBLERemoteCharacteristic* chr) {}
    virtual void onNotification(NimBLERemoteCharacteristic* chr, uint8_t* data, size_t length) {}
};

class BLEExplorer {
public:
    BLEExplorer();

    void init();
    void setCallbacks(BLEExplorerCallbacks* cb);

    // Scanning
    void startScan(uint32_t durationSeconds = 10);
    void stopScan();
    bool isScanning() const;

    // Connection
    bool connectToDevice(const NimBLEAddress& address);
    void disconnect();
    bool isConnected() const;

    // Discovery
    bool discoverServices();
    bool subscribeToNotifications();

    // Getters
    NimBLEClient* getClient() const { return m_client; }
    const std::vector<NimBLERemoteService*>& getServices() const;

private:
    NimBLEClient* m_client = nullptr;
    NimBLEScan* m_scan = nullptr;
    BLEExplorerCallbacks* m_callbacks = nullptr;
    bool m_connected = false;

    // Internal scan callback
    class ScanCallbacks : public NimBLEScanCallbacks {
    public:
        ScanCallbacks(BLEExplorer* parent) : m_parent(parent) {}
        void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override;
        void onScanEnd(const NimBLEScanResults& results, int reason) override;
    private:
        BLEExplorer* m_parent;
    };

    // Internal client callback
    class ClientCallbacks : public NimBLEClientCallbacks {
    public:
        ClientCallbacks(BLEExplorer* parent) : m_parent(parent) {}
        void onConnect(NimBLEClient* client) override;
        void onDisconnect(NimBLEClient* client, int reason) override;
    private:
        BLEExplorer* m_parent;
    };

    ScanCallbacks m_scanCallbacks{this};
    ClientCallbacks m_clientCallbacks{this};

    // Notification callback (static so we can pass to NimBLE)
    static void notifyCallback(NimBLERemoteCharacteristic* chr,
                               uint8_t* data, size_t length, bool isNotify);
    static BLEExplorer* s_instance;
};
