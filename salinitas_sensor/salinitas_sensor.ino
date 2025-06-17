/*****************************************
 * Board: ESP32 Dev Module
 * Sensor Salinitas via ADS1115 (ADC eksternal)
 * Kalibrasi berdasarkan regresi logaritmik
 *****************************************/

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <math.h>  // untuk pow()

// Inisialisasi ADS1115
Adafruit_ADS1115 ads;

// LED indikator opsional
#define indikator 2

// Variabel
int16_t adcReading;     // Nilai mentah ADC dari ADS1115
float voltage;          // Tegangan dalam volt
float salinitas;        // Nilai salinitas dalam ppm
float salinity;

void setup() {
  Serial.begin(115200);
  pinMode(indikator, OUTPUT);
  digitalWrite(indikator, LOW);

  if (!ads.begin()) {
    Serial.println("ADS1115 tidak terdeteksi! Cek koneksi I2C.");
    while (1);
  }

  ads.setGain(GAIN_ONE);  // Rentang ±4.096V (1 bit = 125 µV)
  Serial.println("Kalibrasi Salinitas dengan Regresi Polinomial Siap!");
}

void loop() {
  digitalWrite(indikator, HIGH);
  delay(1000);

  adcReading = ads.readADC_SingleEnded(0);
  voltage = adcReading * 4.096 / 32767.0;

// Kalibrasi salinitas dengan regresi logaritmik
  salinity = exp((voltage + 0.310) / 0.4112);


  // Tampilkan ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(adcReading);
  Serial.print(" | Tegangan (V): ");
  Serial.print(voltage, 5);
  Serial.print(" | Salinitas (ppm): ");
  Serial.println(salinity, 0);

  digitalWrite(indikator, LOW);
  delay(1000);
}
