#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ---------- Koneksi ke VPS ----------
const char* ssid = "?";
const char* password = "kopiko04";

const char* serverName = "http://62.171.164.152:5000/data";

// ---------- Sensor ADS1115 untuk pH dan TDS ----------
Adafruit_ADS1115 ads;

// ---------- Sensor Suhu DS18B20 ----------
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected.");

  // Inisialisasi sensor suhu
  sensors.begin();

  // Inisialisasi ADS1115
  if (!ads.begin()) {
    Serial.println("Gagal menemukan ADS1115. Cek koneksi!");
    while (1);
  }
  ads.setGain(GAIN_ONE); // ±4.096V, resolusi 0.125 mV/bit
}

void loop() {
  // ---------- Pembacaan pH dan TDS ----------
  int16_t adc0 = ads.readADC_SingleEnded(0);
  float voltage = adc0 * 0.125 / 1000.0;  // dalam Volt

  // Kalibrasi pH disesuaikan (sementara)
  float pH = -10.87 * voltage + 19.077;

  // TDS berdasarkan rumus kalibrasi
  float tds = 0.0265 * adc0;

  // ---------- Pembacaan Suhu ----------
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  float tempF = sensors.getTempFByIndex(0);

  // ---------- Output Serial ----------
  Serial.print("ADC: ");
  Serial.print(adc0);
  Serial.print(" | Tegangan: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.print(pH, 2);
  Serial.print(" | TDS: ");
  Serial.print(tds, 2);
  Serial.print(" ppm | Suhu: ");
  Serial.print(tempC, 2);
  Serial.println(" °C");

  // ---------- Kirim Data ke Server ----------
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"suhu\": " + String(tempC, 2) + 
                      ", \"ph\": " + String(pH, 2) + 
                      ", \"tds\": " + String(tds, 2) + "}";

    int httpResponseCode = http.POST(jsonData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode != 200) {
      Serial.println("Gagal mengirim data ke server!");
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected!");
  }

  delay(5000);  // Delay antar pengiriman
}

