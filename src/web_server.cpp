#include "web_server.h"
#include "web_ui.h"

WebServer::WebServer(BLEExplorer& explorer, uint16_t port)
    : m_explorer(explorer), m_server(port), m_ws("/ws") {}

void WebServer::connectWiFi(const char* ssid, const char* password) {
    Serial.printf("[WiFi] Connecting to %s", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
}

void WebServer::begin(const char* ssid, const char* password) {
    connectWiFi(ssid, password);

    m_ws.onEvent([](AsyncWebSocket* server, AsyncWebSocketClient* client,
                    AwsEventType type, void* arg, uint8_t* data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            Serial.printf("[WS] Client #%u connected\n", client->id());
        } else if (type == WS_EVT_DISCONNECT) {
            Serial.printf("[WS] Client #%u disconnected\n", client->id());
        }
    });
    m_server.addHandler(&m_ws);

    setupRoutes();
    m_server.begin();
    Serial.printf("[Web] Server started at http://%s/\n", WiFi.localIP().toString().c_str());
}

void WebServer::sendEvent(const String& event, const String& data) {
    String msg = "{\"event\":\"" + event + "\",\"data\":" + data + "}";
    m_ws.textAll(msg);
}

void WebServer::setupRoutes() {
    m_server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", WEB_UI_HTML);
    });

    m_server.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleStatus(request);
    });

    m_server.on("/api/scan", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleScan(request);
    });

    m_server.on("/api/devices", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleDevices(request);
    });

    m_server.on("/api/connect", HTTP_POST,
        [](AsyncWebServerRequest* request) {},
        nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            handleConnect(request, data, len, index, total);
        });

    m_server.on("/api/disconnect", HTTP_POST, [this](AsyncWebServerRequest* request) {
        handleDisconnect(request);
    });

    m_server.on("/api/services", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleServices(request);
    });

    m_server.on("/api/subscribe", HTTP_POST, [this](AsyncWebServerRequest* request) {
        handleSubscribe(request);
    });

    m_server.on("/api/debug", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        doc["pending_op"] = m_pendingOp.load(std::memory_order_acquire);
        doc["pending_addr"] = String(m_pendingAddress);
        doc["last_debug"] = m_lastDebug;
        doc["devices_count"] = m_devices.size();
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    m_server.on("/api/giant/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleGiantStatus(request);
    });

    m_server.on("/api/giant/command", HTTP_POST,
        [](AsyncWebServerRequest* request) {},
        nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            handleGiantCommand(request, data, len, index, total);
        });

    // Ride logger routes
    m_server.on("/api/rides", HTTP_GET, [this](AsyncWebServerRequest* request) {
        if (!m_rideLogger) { request->send(500, "application/json", "{\"error\":\"No logger\"}"); return; }
        JsonDocument doc;
        doc["logging"] = m_rideLogger->isLogging();
        doc["currentFile"] = m_rideLogger->currentRideFile();
        doc["totalKB"] = m_rideLogger->totalBytes() / 1024;
        doc["usedKB"] = m_rideLogger->usedBytes() / 1024;
        JsonArray arr = doc["rides"].to<JsonArray>();
        auto rides = m_rideLogger->listRides();
        for (const auto& r : rides) {
            JsonObject o = arr.add<JsonObject>();
            o["file"] = r.filename;
            o["size"] = r.size;
            o["samples"] = r.samples;
        }
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    m_server.on("/api/rides/start", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!m_rideLogger) { request->send(500, "application/json", "{\"error\":\"No logger\"}"); return; }
        bool ok = m_rideLogger->start();
        request->send(200, "application/json", ok ? "{\"status\":\"recording\"}" : "{\"error\":\"Failed to start\"}");
    });

    m_server.on("/api/rides/stop", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!m_rideLogger) { request->send(500, "application/json", "{\"error\":\"No logger\"}"); return; }
        m_rideLogger->stop();
        request->send(200, "application/json", "{\"status\":\"stopped\"}");
    });

    m_server.on("/api/rides/download", HTTP_GET, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("file")) { request->send(400, "application/json", "{\"error\":\"Missing file param\"}"); return; }
        String filename = request->getParam("file")->value();
        // Sanitize: strip path separators
        filename.replace("/", "");
        filename.replace("..", "");
        String path = String(RideLogger::RIDE_DIR) + "/" + filename;
        if (!LittleFS.exists(path)) { request->send(404, "application/json", "{\"error\":\"Not found\"}"); return; }
        AsyncWebServerResponse* response = request->beginResponse(LittleFS, path, "text/csv");
        response->addHeader("Content-Disposition", "attachment; filename=\"" + filename + "\"");
        request->send(response);
    });

    m_server.on("/api/rides/delete", HTTP_DELETE, [this](AsyncWebServerRequest* request) {
        if (!m_rideLogger) { request->send(500, "application/json", "{\"error\":\"No logger\"}"); return; }
        if (!request->hasParam("file")) { request->send(400, "application/json", "{\"error\":\"Missing file param\"}"); return; }
        String filename = request->getParam("file")->value();
        filename.replace("/", "");
        filename.replace("..", "");
        bool ok = m_rideLogger->deleteRide(filename);
        request->send(200, "application/json", ok ? "{\"status\":\"deleted\"}" : "{\"error\":\"Delete failed\"}");
    });
}

void WebServer::handleStatus(AsyncWebServerRequest* request) {
    JsonDocument doc;
    doc["wifi_ip"] = WiFi.localIP().toString();
    doc["wifi_rssi"] = WiFi.RSSI();
    doc["ble_connected"] = m_explorer.isConnected();
    doc["ble_scanning"] = m_explorer.isScanning();
    doc["heap_free"] = ESP.getFreeHeap();
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
}

void WebServer::handleScan(AsyncWebServerRequest* request) {
    if (m_explorer.isConnected()) {
        request->send(400, "application/json", "{\"error\":\"Disconnect first\"}");
        return;
    }
    uint32_t duration = 10;
    if (request->hasParam("duration")) {
        duration = request->getParam("duration")->value().toInt();
    }
    m_explorer.startScan(duration);
    request->send(200, "application/json", "{\"status\":\"scanning\",\"duration\":" + String(duration) + "}");
}

void WebServer::handleDevices(AsyncWebServerRequest* request) {
    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    for (const auto& d : m_devices) {
        JsonObject obj = arr.add<JsonObject>();
        obj["address"] = d.address.toString();
        obj["name"] = d.name;
        obj["rssi"] = d.rssi;
        if (d.services.length() > 0) obj["services"] = d.services;
        if (d.manufacturer.length() > 0) obj["manufacturer"] = d.manufacturer;
    }
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
}

void WebServer::handleConnect(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
    String body;
    for (size_t i = 0; i < len; i++) body += (char)data[i];

    JsonDocument doc;
    if (deserializeJson(doc, body)) {
        request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    const char* address = doc["address"];
    if (!address) {
        request->send(400, "application/json", "{\"error\":\"Missing address\"}");
        return;
    }

    // Store address for deferred connect in loop()
    strncpy(m_pendingAddress, address, sizeof(m_pendingAddress) - 1);
    m_pendingAddress[sizeof(m_pendingAddress) - 1] = '\0';
    m_pendingOp.store((int)PendingOp::CONNECT, std::memory_order_release);
    request->send(200, "application/json", "{\"status\":\"connecting\"}");
}

void WebServer::handleDisconnect(AsyncWebServerRequest* request) {
    m_pendingOp.store((int)PendingOp::DISCONNECT, std::memory_order_release);
    request->send(200, "application/json", "{\"status\":\"disconnecting\"}");
}

void WebServer::handleServices(AsyncWebServerRequest* request) {
    if (!m_explorer.isConnected()) {
        request->send(400, "application/json", "{\"error\":\"Not connected\"}");
        return;
    }

    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    const auto& services = m_explorer.getServices();
    for (auto* svc : services) {
        JsonObject svcObj = arr.add<JsonObject>();
        svcObj["uuid"] = svc->getUUID().toString();

        JsonArray charsArr = svcObj["characteristics"].to<JsonArray>();
        const auto& chars = svc->getCharacteristics(false);
        for (auto* chr : chars) {
            JsonObject chrObj = charsArr.add<JsonObject>();
            chrObj["uuid"] = chr->getUUID().toString();

            JsonArray props = chrObj["properties"].to<JsonArray>();
            if (chr->canRead()) props.add("READ");
            if (chr->canWrite()) props.add("WRITE");
            if (chr->canWriteNoResponse()) props.add("WRITE_NR");
            if (chr->canNotify()) props.add("NOTIFY");
            if (chr->canIndicate()) props.add("INDICATE");

            if (chr->canRead()) {
                NimBLEAttValue val = chr->readValue();
                if (val.size() > 0) {
                    String hexStr;
                    for (size_t i = 0; i < val.size(); i++) {
                        char buf[4];
                        snprintf(buf, sizeof(buf), "%02x ", val.data()[i]);
                        hexStr += buf;
                    }
                    hexStr.trim();
                    chrObj["value_hex"] = hexStr;

                    bool printable = true;
                    for (size_t i = 0; i < val.size(); i++) {
                        if (val.data()[i] < 0x20 || val.data()[i] > 0x7e) {
                            printable = false;
                            break;
                        }
                    }
                    if (printable) {
                        chrObj["value_str"] = String((const char*)val.data(), val.size());
                    }
                }
            }
        }
    }
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
}

void WebServer::handleSubscribe(AsyncWebServerRequest* request) {
    if (!m_explorer.isConnected()) {
        request->send(400, "application/json", "{\"error\":\"Not connected\"}");
        return;
    }
    m_pendingOp.store((int)PendingOp::SUBSCRIBE, std::memory_order_release);
    request->send(200, "application/json", "{\"status\":\"subscribing\"}");
}

void WebServer::loop() {
    m_ws.cleanupClients();

    int opVal = m_pendingOp.load(std::memory_order_acquire);
    if (opVal == (int)PendingOp::NONE) return;
    m_pendingOp.store((int)PendingOp::NONE, std::memory_order_release);
    PendingOp op = (PendingOp)opVal;

    switch (op) {
        case PendingOp::CONNECT: {
            String addrStr(m_pendingAddress);
            m_lastDebug = "CONNECT starting for " + addrStr;
            if (m_explorer.isScanning()) {
                m_explorer.stopScan();
                delay(500);
            }
            NimBLEAddress addr;
            bool found = false;
            for (const auto& d : m_devices) {
                if (String(d.address.toString().c_str()) == addrStr) {
                    addr = d.address;
                    found = true;
                    break;
                }
            }
            if (!found) {
                addr = NimBLEAddress(std::string(m_pendingAddress), 0);
            }
            m_lastDebug = "Connecting type=" + String(addr.getType()) + " found=" + String(found);
            Serial.printf("[Web] Connecting to %s (type=%d)\n", m_pendingAddress, addr.getType());
            if (m_explorer.connectToDevice(addr)) {
                m_lastDebug = "Connected! Discovering services...";
                m_explorer.discoverServices();
                sendEvent("connected", "{\"address\":\"" + addrStr + "\"}");
            } else {
                uint8_t altType = addr.getType() == 0 ? 1 : 0;
                NimBLEAddress altAddr(std::string(m_pendingAddress), altType);
                m_lastDebug = "Retry type=" + String(altType);
                if (m_explorer.connectToDevice(altAddr)) {
                    m_lastDebug = "Connected on retry!";
                    m_explorer.discoverServices();
                    sendEvent("connected", "{\"address\":\"" + addrStr + "\"}");
                } else {
                    m_lastDebug = "Both attempts failed";
                    sendEvent("error", "{\"message\":\"Connection to " + addrStr + " failed\"}");
                }
            }
            break;
        }
        case PendingOp::DISCONNECT:
            m_explorer.disconnect();
            sendEvent("disconnected", "{}");
            break;
        case PendingOp::SUBSCRIBE:
            m_explorer.subscribeToNotifications();
            sendEvent("subscribed", "{}");
            break;
        case PendingOp::GIANT_CMD: {
            String cmdName(m_pendingGiantCmd);
            bool ok = false;
            if (m_giantBike && m_giantBike->isInitialized()) {
                GiantCommand gcmd;
                bool known = true;
                if (cmdName == "connect") gcmd = CONNECT_GEV;
                else if (cmdName == "disconnect") gcmd = DISCONNECT_GEV;
                else if (cmdName == "riding") gcmd = READ_RIDING_DATA;
                else if (cmdName == "factory") gcmd = READ_FACTORY_DATA;
                else if (cmdName == "tuning") gcmd = READ_TUNING_DATA;
                else if (cmdName == "light") gcmd = TRIGGER_LIGHT;
                else if (cmdName == "assist_up") gcmd = TRIGGER_ASSIST_UP;
                else if (cmdName == "assist_down") gcmd = TRIGGER_ASSIST_DOWN;
                else if (cmdName == "power") gcmd = TRIGGER_POWER;
                else if (cmdName == "range") gcmd = READ_REMAINING_RANGE;
                else known = false;

                if (known) {
                    ok = m_giantBike->sendCommand(gcmd);
                }
            }
            String result = "{\"type\":\"giant_cmd_result\",\"cmd\":\"" + cmdName + "\",\"success\":" + (ok ? "true" : "false") + "}";
            m_ws.textAll(result);
            break;
        }
        default:
            break;
    }
}

void WebServer::handleGiantStatus(AsyncWebServerRequest* request) {
    JsonDocument doc;
    if (!m_giantBike || !m_giantBike->isInitialized()) {
        doc["initialized"] = false;
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
        return;
    }
    doc["initialized"] = true;
    GevRideData ride = m_giantBike->getRideData();
    JsonObject r = doc["ride"].to<JsonObject>();
    r["speed"] = ride.speed;
    r["crank"] = ride.crank;
    r["torque"] = ride.torque;
    r["watt"] = ride.watt;
    r["rsoc"] = ride.rsoc;
    r["distance"] = ride.currentRidingDistance;
    r["time"] = ride.currentRidingTime;
    r["range"] = ride.carr;
    r["errorCode"] = ride.errorCode;

    GevBikeData bike = m_giantBike->getBikeData();
    JsonObject b = doc["bike"].to<JsonObject>();
    b["odo"] = bike.odo;
    b["epCapacity"] = bike.epPercentageCapacity;
    b["epLife"] = bike.epPercentageLife;
    b["rcUiFw"] = bike.rideControlUIFwVer;
    b["duFw"] = bike.syncDriveDuFwVersion;
    b["epFw"] = bike.epVersion;
    b["duType"] = bike.duType;

    GevFactoryData factory = m_giantBike->getFactoryData();
    JsonObject f = doc["factory"].to<JsonObject>();
    f["frameNumber"] = factory.frameNumber;
    f["rcHwVersion"] = factory.rcHwVersion;
    f["rcType"] = factory.rcType;

    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
}

void WebServer::handleGiantCommand(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
    String body;
    for (size_t i = 0; i < len; i++) body += (char)data[i];

    JsonDocument doc;
    if (deserializeJson(doc, body)) {
        request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }
    const char* cmd = doc["cmd"];
    if (!cmd) {
        request->send(400, "application/json", "{\"error\":\"Missing cmd\"}");
        return;
    }
    strncpy(m_pendingGiantCmd, cmd, sizeof(m_pendingGiantCmd) - 1);
    m_pendingGiantCmd[sizeof(m_pendingGiantCmd) - 1] = '\0';
    m_pendingOp.store((int)PendingOp::GIANT_CMD, std::memory_order_release);
    request->send(200, "application/json", "{\"status\":\"queued\",\"cmd\":\"" + String(cmd) + "\"}");
}

void WebServer::addScannedDevice(const NimBLEAdvertisedDevice& device) {
    // Deduplicate by address
    String addrStr = device.getAddress().toString().c_str();
    for (auto& d : m_devices) {
        if (String(d.address.toString().c_str()) == addrStr) {
            d.rssi = device.getRSSI();
            if (device.getName().length() > 0) d.name = device.getName().c_str();
            return;
        }
    }
    ScannedDevice sd;
    sd.address = device.getAddress();
    sd.name = device.getName().c_str();
    sd.rssi = device.getRSSI();
    m_devices.push_back(sd);
}
