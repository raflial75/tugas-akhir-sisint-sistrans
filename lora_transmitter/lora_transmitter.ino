#include <SPI.h>
#include <LoRa.h>

// Pin LoRa
#define SS      5
#define RST     14
#define DIO0    26

int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Transmitter - ESP32-S3");

  // Atur pin ke library LoRa
  LoRa.setPins(SS, RST, DIO0);

  // Inisialisasi LoRa dengan frekuensi (433E6, 866E6, atau 915E6)
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  String message = "Paket #" + String(counter);

  Serial.print("Sending: ");
  Serial.println(message);

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  counter++;

  delay(2000); // interval pengiriman
}
