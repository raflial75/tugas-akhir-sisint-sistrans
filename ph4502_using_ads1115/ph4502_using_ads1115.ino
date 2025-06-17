#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// ---------- Sensor ADS1115 untuk pH ----------
Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(115200);

  // Inisialisasi ADS1115
  if (!ads.begin()) {
    Serial.println("Gagal menemukan ADS1115. Cek koneksi!");
    while (1);
  }
  ads.setGain(GAIN_ONE); // ±4.096V, resolusi 0.125 mV/bit
}

void loop() {
  // ---------- Pembacaan pH ----------
  int16_t adc0 = ads.readADC_SingleEnded(1);
  

  
  // Menghitung tegangan dari ADC dengan menggunakan rumus konversi yang benar
  float voltage = adc0 * 4.096 / 32767.0; // Mengubah nilai ADC menjadi tegangan
  
  // Menghitung pH berdasarkan persamaan kalibrasi: 
  // y = -0.0065x² - 0.0806x + 3.8851
  float pH = -0.4010 * voltage + 4.0214;

  // y = -0.0065x² - 0.0806x + 3.8851
  // float pH = -0.0065 * voltage * voltage - 0.0806 * voltage + 3.8851;
  // float pH = -0.0003 * voltage * voltage * voltage * voltage * voltage + 0.0078 * voltage * voltage * voltage * voltage - 0.0643 * voltage * voltage * voltage + 0.2118 * voltage * voltage - 0.335 * voltage + 3.8962;

  //-0,0003 * voltage * voltage * voltage * voltage * voltage + 0,0078 * voltage * voltage * voltage * voltage - 0,0643 * voltage * voltage * voltage + 0,2118 * voltage * voltage - 0,335 * voltage + 3,8962
  // ---------- Output Serial ----------
  Serial.print("ADC: ");
  Serial.print(adc0);
  Serial.print(" | Tegangan: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.println(pH, 2);
  
  delay(1000);  // Delay antar pembacaan
}
