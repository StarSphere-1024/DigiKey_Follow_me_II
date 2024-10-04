#include <OPAMP.h>
#include "analogWave.h" // Include the library for analog waveform generation

analogWave wave(DAC);

int freq = 100;

void setup () {
  Serial.begin(250000);
  analogReadResolution(14);
  wave.sine(freq);
  wave.amplitude(0.5);
  OPAMP.begin(OPAMP_SPEED_HIGHSPEED);
}

void loop() {
  Serial.print(analogRead(A4)); // DAC output
  Serial.print(" ");
  Serial.println(analogRead(A5)); // OPAMP output
}
