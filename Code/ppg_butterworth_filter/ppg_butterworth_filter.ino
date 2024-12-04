#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

/* 
    orde of filter
*/

#define N 2 

/*
    butterworth filter constanta
    get this constanta from ppg_butterworth_filter
*/

float a[N + 1] = {1.0,        0.84860766, 0.3018775};     // feedback coefficient
float b[N + 1] = {0.53762129, 1.07524258, 0.53762129};    // feedforward coefficient

float x[N + 1];   // temp array for previous input value
float y[N + 1];   // temp array for previous output value

/*
    sensor attribute
*/

byte ledBrightness = 0x1F;      // Options: 0=Off to 255=50mA
byte sampleAverage = 4;         // Options: 1, 2, 4, 8, 16, 32
byte ledMode = 3;               // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
int sampleRate = 100;           // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 411;           // Options: 69, 118, 215, 411
int adcRange = 4096;            // Options: 2048, 4096, 8192, 16384

MAX30105 ppg;

/*
    function
*/

void filterButterworthInit() {
  for (int i = 0; i <= N; i++) {
    x[i] = 0;
    y[i] = 0;
  }
}

float filterButterworth(float input) {
  for (int i = N; i > 0; i--) {
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  }

  x[0] = input;

  y[0] = b[0] * x[0];
  for (int i = 1; i <= N; i++) {
    y[0] += b[i] * x[i] - a[i] * y[i];
  }

  return y[0];
}


void setup() {
  Serial.begin(115200);

  if (!ppg.begin(Wire, I2C_SPEED_FAST)) 
  {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    delay(500);
    while (1);
  }

  ppg.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  filterButterworthInit();
}

void loop() {
  float ir = ppg.getIR();
  float filteredIR = filterButterworth(ir); 

  Serial.print(ir);
  Serial.print("\t");
  Serial.println(filteredIR);
  
  /*
      adjust this delay value with frequency sampling
  */

  delay(100);
}