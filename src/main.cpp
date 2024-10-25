#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>

int relay = 15;  

const char* ssid = "IoT518";
const char* password = "iot123456";

WebServer server(80);

void handleRoot() {
    String html = "<html><head><meta charset='utf-8'><title>ESP32 Relay Control</title></head>";
    html += "<body><h1>ESP32 릴레이 제어</h1>";
    html += "<p>릴레이 상태: " + String(digitalRead(relay) ? "켜짐" : "꺼짐") + "</p>";
    html += "<button onclick='location.href=\"/turnOn\"'>켜기</button>";
    html += "<button onclick='location.href=\"/turnOff\"'>끄기</button>";
    html += "<button onclick='location.href=\"/toggle\"'>토글</button>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleTurnOn() {
    Serial.println("handleTurnOn called");
    digitalWrite(relay, HIGH);
    server.send(200, "text/plain", "Relay ON");
}

void handleTurnOff() {
    Serial.println("handleTurnOff called");
    digitalWrite(relay, LOW);
    server.send(200, "text/plain", "Relay OFF");
}

void handleToggle() {
    Serial.println("handleToggle called");
    digitalWrite(relay, !digitalRead(relay));
    server.send(200, "text/plain", "Relay Toggled");
}

void setup() {
    Serial.begin(115200);
    pinMode(relay, OUTPUT);
    digitalWrite(relay, LOW);  

    Serial.println("\nConnecting to WiFi...");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  // IP 주소 출력
    
    if (MDNS.begin("server")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", HTTP_GET, handleRoot);
    server.on("/turnOn", HTTP_GET, handleTurnOn);
    server.on("/turnOff", HTTP_GET, handleTurnOff);
    server.on("/toggle", HTTP_GET, handleToggle);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}