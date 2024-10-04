#include "analogWave.h"

analogWave wave(DAC);

int freq = 5;

void setup() {
  Serial.begin(115200);
  wave.sine(freq);
}

void loop() {
  
}
