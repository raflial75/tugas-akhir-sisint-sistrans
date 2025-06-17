#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "?";
const char* password = "kopiko04";

const char* serverName = "http://62.171.164.152:5000/data";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected.");
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"suhu\": 28.5, \"kelembaban\": 70}";
    int httpResponseCode = http.POST(jsonData);

    Serial.println(httpResponseCode);
    http.end();
  }
  delay(5000); // kirim tiap 5 detik
}
