/*

  note_1: run this arduino code (ppg_read_sensor.ino) and python code (ppg_to_csv.py) together
          to get the raw_ppg.csv
  note_2: dont forget to close the arduino IDE serial monitor or serial plotter
          so the python code can communicate via serial to microcontroller

*/



#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

byte ledBrightness = 0x1F;      // Options: 0=Off to 255=50mA
byte sampleAverage = 4;         // Options: 1, 2, 4, 8, 16, 32
byte ledMode = 3;               // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
int sampleRate = 100;           // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 411;           // Options: 69, 118, 215, 411
int adcRange = 4096;            // Options: 2048, 4096, 8192, 16384

MAX30105 ppg;

void setup() {
  Serial.begin(115200);

  if (!ppg.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    delay(500);
    while (1);
  }

  ppg.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  /* print header to serial or CSV */
  Serial.println("Timestamp,IR_Value");       
}

void loop() {
  float ir = ppg.getIR();
  unsigned long timestamp = millis();

  /* print data to serial or CSV */
  Serial.print(timestamp);
  Serial.print(",");
  Serial.println(ir);

  /* sampling frekuensi 10 Hz */
  delay(100); 
}