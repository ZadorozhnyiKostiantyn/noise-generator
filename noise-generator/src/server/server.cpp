#include <LITTLEFS.h>
#include <AsyncTCP.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "../config/config.h"
#include "../audio/audio.h"
#include "../sd/sd_card.h"
#include "../display/display.h"
#include "../events/events.h"
#include <driver/i2s.h>

const char *ssid = "ESP32-AP";
const char *password = "12345678";

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

DNSServer dnsServer;

AsyncWebServer server(80);
AsyncEventSource events("/events");
bool ultrasonicMode = false; // Стан ультразвукового режиму

struct AppState
{
    int activeAudioId;              // ID активного аудіофайлу, або -1 якщо нічого не програється
    std::vector<String> audioFiles; // Список аудіофайлів
    bool isRecording;               // Чи йде запис
    bool isUltrasonic;              // Чи активний ультразвуковий режим
};

AppState getAppState()
{
    AppState state;

    // Отримання списку файлів
    state.audioFiles = getListFiles();

    // Визначення активного аудіофайлу
    state.activeAudioId = isPlaying ? currentFileIndex : -1;

    // Чи йде запис
    state.isRecording = isRecording;

    // Чи активний ультразвуковий режим
    state.isUltrasonic = ultrasonicMode;

    return state;
}

// Функція для обробки запиту на отримання списку файлів
String getFiles()
{
    String json = "[";
    std::vector<String> files = getListFiles();
    for (size_t i = 0; i < files.size(); ++i)
    {
        if (i > 0)
            json += ",";
        json += "{\"id\":" + String(i) + ",\"name\":\"" + files[i] + "\"}";
    }
    json += "]";
    return json;
}

// Функція для обробки запиту на відтворення файлу за id
void handleTogglePlayback(AsyncWebServerRequest *request)
{
    Serial.println("handleTogglePlayback");
    if (!request->hasParam("id", true))
    {
        request->send(400, "text/plain", "Missing 'id' parameter");
        return;
    }

    int id = request->getParam("id", true)->value().toInt();
    Serial.println("Audi event file id: " + String(id));
    std::vector<String> files = getListFiles(); // Отримання списку файлів

    if (id >= 0 && id < files.size())
    {
        audioEvent(id);
        request->send(200, "text/plain", "Playing file...");
    }
    else
    {
        request->send(404, "text/plain", "File not found");
    }
}

void handleDeleteFile(AsyncWebServerRequest *request)
{
    if (request->url().startsWith("/delete-audio/"))
    {
        String idStr = request->url().substring(String("/delete-audio/").length());
        int id = idStr.toInt();

        std::vector<String> files = getListFiles(); // Отримання списку файлів
        request->send(200, "text/plain", "File deleted: " + files[id % files.size()]);

        // if (id >= 0 && id < files.size())
        // {
        //     String filePath = "/" + files[id];
        //     if (LITTLEFS.remove(filePath))
        //     {
        //         Serial.print("Deleted file: " + filePath);
        //         request->send(200, "text/plain", "File deleted: " + filePath);
        //     }
        //     else
        //     {
        //         request->send(500, "text/plain", "Failed to delete file: " + filePath);
        //     }
        // }
        // else
        // {
        //     request->send(404, "text/plain", "File not found");
        // }
    }
    else
    {
        request->send(400, "text/plain", "Invalid request");
    }
}

// Функція для обробки запиту на запис файлу
void handleToggleRecording(AsyncWebServerRequest *request)
{
    if (!request->hasParam("filename", true))
    {
        request->send(400, "text/plain", "Missing 'filename' parameter");
        return;
    }

    String filename = request->getParam("filename", true)->value();
    Serial.print("Recording started: " + filename);
    recordingEvent(filename);
    request->send(200, "text/plain", "");
}

// Функція для обробки запиту на перемикання ультразвукового режиму
void handleToggleUltrasonic(AsyncWebServerRequest *request)
{
    if (!request->hasParam("isActive", true))
    {
        request->send(400, "text/plain", "Missing 'isActive' parameter");
        return;
    }

    String isActiveParam = request->getParam("isActive", true)->value();
    ultrasonicMode = (isActiveParam == "true");

    if (ultrasonicMode)
    {
        Serial.println("Ultrasonic mode activated");
        
    }
    else
    {
        Serial.println("Ultrasonic mode deactivated");
    }

    ultrasonicEvent(ultrasonicMode);

    request->send(200, "text/plain", "Ultrasonic mode set to: " + isActiveParam);
}

void handleGetState(AsyncWebServerRequest *request)
{
    AppState state = getAppState();

    String json = "{";
    json += "\"activeAudioId\": ";
    if (state.activeAudioId == -1)
    {
        json += "null";
    }
    else
    {
        json += String(state.activeAudioId);
    }
    json += ",";
    json += "\"audioFiles\": ";
    json += getFiles();
    json += ",";
    
    json += "\"isRecording\": " + String(state.isRecording ? "true" : "false") + ",";
    json += "\"isUltrasonic\": " + String(state.isUltrasonic ? "true" : "false");
    json += "}";

    request->send(200, "application/json", json);
}

void updateIsRecording(bool isRecording) {
    events.send(isRecording ? "true" : "false", "is-recording");
}

void updateActiveAudioId(int audioId) {
    events.send(String(audioId).c_str(), "active-audio-id");
}

void updateIsUltrasonic(bool isUltrasonic) {
    events.send(isUltrasonic ? "true" : "false", "is-ultrasonic");
}

void updateAudioFiles(const std::vector<String>& audioFiles) {
    String json = getFiles();
    events.send(json.c_str(), "audio-files");
}

// Основна функція для налаштування сервера
void serverSetup()
{
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(ssid, password);
    Serial.println("WiFi точка створена");
    Serial.println(WiFi.softAPIP());
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address:");
    Serial.println(WiFi.softAPIP());

    dnsServer.start(53, "*", WiFi.softAPIP());

    if (!LITTLEFS.begin())
    {
        Serial.println("Помилка ініціалізації LittleFS");
        return;
    }

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    // server.on("/delete-audio", HTTP_DELETE, handleDeleteFile);
    server.on("/toggle-playback", HTTP_POST, handleTogglePlayback);
    server.on("/toggle-recording", HTTP_POST, handleToggleRecording);
    server.on("/toggle-ultrasonic", HTTP_POST, handleToggleUltrasonic);

    // API endpoint to get the current application state
    server.on("/state", HTTP_GET, handleGetState);

    server.serveStatic("/", LITTLEFS, "/").setDefaultFile("index.html");

    server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->redirect("/"); });
    server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->redirect("/"); });
    server.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->redirect("/"); });
    server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->redirect("/"); });
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->redirect("/"); });

    server.addHandler(&events);

    server.begin();
}

void serverLoop()
{
    dnsServer.processNextRequest();
}