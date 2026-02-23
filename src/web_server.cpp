#include "web_server.h"
#include "web_ui.h"
#include "portal_ui.h"
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "config.h"

WebServer::WebServer(BLEExplorer& explorer, uint16_t port)
    : m_explorer(explorer), m_server(port), m_ws("/ws") {}

void WebServer::begin(const char* ssid, const char* password) {
    bool connected = m_wifiManager.begin(ssid, password);

    if (connected) {
        // mDNS — access at http://MDNS_HOSTNAME.local
        if (MDNS.begin(MDNS_HOSTNAME)) {
            MDNS.addService("http", "tcp", 80);
            Serial.printf("[mDNS] Registered: http://%s.local/\n", MDNS_HOSTNAME);
        } else {
            Serial.println("[mDNS] Failed to start");
        }

        // OTA updates
        ArduinoOTA.setHostname(MDNS_HOSTNAME);
        ArduinoOTA.onStart([]() { Serial.println("[OTA] Update starting..."); });
        ArduinoOTA.onEnd([]() { Serial.println("\n[OTA] Update complete, rebooting"); });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("[OTA] %u%%\r", progress * 100 / total);
        });
        ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("[OTA] Error %u: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive failed");
            else if (error == OTA_END_ERROR) Serial.println("End failed");
        });
        ArduinoOTA.begin();
        Serial.println("[OTA] Ready");
    }

    m_ws.onEvent([](AsyncWebSocket* server, AsyncWebSocketClient* client,
                    AwsEventType type, void* arg, uint8_t* data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            Serial.printf("[WS] Client #%u connected\n", client->id());
        } else if (type == WS_EVT_DISCONNECT) {
            Serial.printf("[WS] Client #%u disconnected\n", client->id());
        }
    });
    m_server.addHandler(&m_ws);

    if (m_wifiManager.isAPMode()) {
        setupPortalRoutes();
    } else {
        setupRoutes();
    }
    m_server.begin();

    if (m_wifiManager.isAPMode()) {
        Serial.printf("[Web] Portal started at http://%s/\n", WiFi.softAPIP().toString().c_str());
    } else {
        Serial.printf("[Web] Server started at http://%s/ | http://%s.local/\n",
                      WiFi.localIP().toString().c_str(), MDNS_HOSTNAME);
    }
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

    // Ride logger routes — specific paths before general to avoid prefix matching
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

    m_server.on("/api/rides/delete", HTTP_DELETE, [this](AsyncWebServerRequest* request) {
        if (!m_rideLogger) { request->send(500, "application/json", "{\"error\":\"No logger\"}"); return; }
        if (!request->hasParam("file")) { request->send(400, "application/json", "{\"error\":\"Missing file param\"}"); return; }
        String filename = request->getParam("file")->value();
        filename.replace("/", "");
        filename.replace("..", "");
        bool ok = m_rideLogger->deleteRide(filename);
        request->send(200, "application/json", ok ? "{\"status\":\"deleted\"}" : "{\"error\":\"Delete failed\"}");
    });

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

    // WiFi management routes
    m_server.on("/api/wifi/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        doc["mode"] = m_wifiManager.isAPMode() ? "ap" : "sta";
        doc["ip"] = WiFi.localIP().toString();
        doc["rssi"] = WiFi.RSSI();
        doc["hostname"] = String(MDNS_HOSTNAME) + ".local";
        String ssid, pass;
        doc["has_saved_credentials"] = m_wifiManager.loadCredentials(ssid, pass);
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    m_server.on("/api/wifi/config", HTTP_POST,
        [](AsyncWebServerRequest* request) {},
        nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            String body;
            for (size_t i = 0; i < len; i++) body += (char)data[i];
            JsonDocument doc;
            if (deserializeJson(doc, body)) { request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}"); return; }
            const char* ssid = doc["ssid"];
            const char* pass = doc["password"];
            if (!ssid || strlen(ssid) == 0) { request->send(400, "application/json", "{\"error\":\"Missing ssid\"}"); return; }
            m_wifiManager.saveCredentials(String(ssid), String(pass ? pass : ""));
            request->send(200, "application/json", "{\"status\":\"saved\"}");
            delay(3000);
            ESP.restart();
        });

    m_server.on("/api/wifi/forget", HTTP_POST, [this](AsyncWebServerRequest* request) {
        m_wifiManager.clearCredentials();
        request->send(200, "application/json", "{\"status\":\"cleared\"}");
    });

    // Webhook routes
    m_server.on("/api/webhook/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        doc["configured"] = m_webhook && m_webhook->hasUrl();
        doc["url"] = m_webhook ? m_webhook->getUrl() : "";
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    m_server.on("/api/webhook/config", HTTP_POST,
        [](AsyncWebServerRequest* request) {},
        nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            if (!m_webhook) { request->send(500, "application/json", "{\"error\":\"No webhook\"}"); return; }
            String body;
            for (size_t i = 0; i < len; i++) body += (char)data[i];
            JsonDocument doc;
            if (deserializeJson(doc, body)) { request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}"); return; }
            const char* url = doc["url"];
            if (!url) { request->send(400, "application/json", "{\"error\":\"Missing url\"}"); return; }
            m_webhook->setUrl(String(url));
            request->send(200, "application/json", "{\"status\":\"saved\"}");
        });

    m_server.on("/api/webhook/test", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!m_webhook || !m_webhook->hasUrl()) {
            request->send(400, "application/json", "{\"error\":\"No webhook URL configured\"}");
            return;
        }
        bool ok = m_webhook->sendTest();
        request->send(200, "application/json", ok ? "{\"status\":\"sent\"}" : "{\"error\":\"Failed\"}");
    });

    m_server.on("/api/webhook/clear", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (m_webhook) m_webhook->clearUrl();
        request->send(200, "application/json", "{\"status\":\"cleared\"}");
    });

    // Charge monitor routes
    m_server.on("/api/charge/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        if (m_chargeMonitor) {
            doc["state"] = ChargeMonitor::stateToString(m_chargeMonitor->getState());
            doc["battery_pct"] = m_chargeMonitor->getBatteryPct();
            doc["enabled"] = m_chargeMonitor->isEnabled();
            doc["threshold"] = m_chargeMonitor->getNotifyThreshold();
        } else {
            doc["state"] = "unavailable";
        }
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    m_server.on("/api/charge/config", HTTP_POST,
        [](AsyncWebServerRequest* request) {},
        nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            if (!m_chargeMonitor) { request->send(500, "application/json", "{\"error\":\"No monitor\"}"); return; }
            String body;
            for (size_t i = 0; i < len; i++) body += (char)data[i];
            JsonDocument doc;
            if (deserializeJson(doc, body)) { request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}"); return; }
            if (doc.containsKey("enabled")) m_chargeMonitor->setEnabled(doc["enabled"].as<bool>());
            if (doc.containsKey("threshold")) m_chargeMonitor->setNotifyThreshold(doc["threshold"].as<int>());
            request->send(200, "application/json", "{\"status\":\"updated\"}");
        });
}

void WebServer::setupPortalRoutes() {
    // Serve captive portal on all paths
    m_server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", PORTAL_HTML);
    });

    m_server.on("/api/wifi/scan", HTTP_GET, [](AsyncWebServerRequest* request) {
        int n = WiFi.scanNetworks();
        JsonDocument doc;
        JsonArray arr = doc["networks"].to<JsonArray>();
        for (int i = 0; i < n; i++) {
            JsonObject net = arr.add<JsonObject>();
            net["ssid"] = WiFi.SSID(i);
            net["rssi"] = WiFi.RSSI(i);
            net["open"] = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN);
        }
        WiFi.scanDelete();
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    m_server.on("/api/wifi/config", HTTP_POST,
        [](AsyncWebServerRequest* request) {},
        nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            String body;
            for (size_t i = 0; i < len; i++) body += (char)data[i];
            JsonDocument doc;
            if (deserializeJson(doc, body)) { request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}"); return; }
            const char* ssid = doc["ssid"];
            const char* pass = doc["password"];
            if (!ssid || strlen(ssid) == 0) { request->send(400, "application/json", "{\"error\":\"Missing ssid\"}"); return; }
            m_wifiManager.saveCredentials(String(ssid), String(pass ? pass : ""));
            request->send(200, "application/json", "{\"status\":\"saved\"}");
            delay(3000);
            ESP.restart();
        });

    // Captive portal redirect: any other path serves the portal
    m_server.onNotFound([](AsyncWebServerRequest* request) {
        request->redirect("/");
    });
}

void WebServer::handleStatus(AsyncWebServerRequest* request) {
    JsonDocument doc;
    doc["wifi_ip"] = WiFi.localIP().toString();
    doc["wifi_rssi"] = WiFi.RSSI();
    doc["hostname"] = String(MDNS_HOSTNAME) + ".local";
    doc["ap_mode"] = m_wifiManager.isAPMode();
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
    m_wifiManager.loop();
    if (!m_wifiManager.isAPMode()) {
        ArduinoOTA.handle();
    }

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
                else if (cmdName == "battery") gcmd = READ_BATTERY;
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
    b["epDischarge"] = bike.epPercentageDischarge;
    b["epChargeCycles"] = bike.epChargeCycles;
    b["epChargeTimes"] = bike.epChargeTimes;
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
