#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Mak-Unipod";
const char* password = "1nNova7ioN5";

WebServer server(80);

const int LED = 2;

void handleOn() {
  digitalWrite(LED, HIGH);
  server.send(200, "text/plain", "LED ON");
}

void handleOff() {
  digitalWrite(LED, LOW);
  server.send(200, "text/plain", "LED OFF");
}

void setup() {

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(115200);

  Serial.print("Connecting to WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();

  Serial.println("HTTP Server Started");
}

void loop() {

  server.handleClient();

}
