#include <SPI.h>
#include <LoRa.h>

// ---------- LoRa Pin Setup (ESP32-S3) ----------
#define LORA_SCK  36
#define LORA_MISO 37
#define LORA_MOSI 35
#define LORA_SS   39
#define LORA_RST  9
#define LORA_DIO0 8

int receivedPackets = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Quality Test Receiver");

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed.");
    while (1);
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr(); // available only on some chips like SX1276
    receivedPackets++;

    Serial.print("Received: ");
    Serial.println(incoming);
    Serial.print("RSSI: ");
    Serial.print(rssi);
    Serial.print(" dBm, SNR: ");
    Serial.print(snr);
    Serial.print(" dB, Total Received: ");
    Serial.println(receivedPackets);
    Serial.println("---------------------------");
  }
}