#include <OPAMP.h>
#include "analogWave.h" 
analogWave wave(DAC);

int freq = 3;

void setup () {

  OPAMP.begin();
  wave.sine(freq);
  wave.amplitude(0.3);
}

void loop() {
}
