#include "Adafruit_SHT4x.h"
Adafruit_SHT4x sht4;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println(F("##################################"));
  Serial.println(F("SGP40 test with SHT40 compensation"));


  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************        

   //Here we can configure the SHT40 Temperature and Humidity Sensor
   //First we set the measurement precision
   //There are three precision levels: High, Medium and Low
   //The precision levels direclty affect the measurement duration, noise level and energy consumption
   //On doubt, just leave it on default (High precision)
   sht4.setPrecision(SHT4X_HIGH_PRECISION);

  
  switch (sht4.getPrecision()) {
     case SHT4X_HIGH_PRECISION: 
       Serial.println(F("SHT40 set to High precision"));
       break;
     case SHT4X_MED_PRECISION: 
       Serial.println(F("SHT40 set to Medium precision"));
       break;
     case SHT4X_LOW_PRECISION: 
       Serial.println(F("SHT40 set to Low precision"));
       break;
  }   
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************       
  
  // The SHT40 has a built-in heater, which can be used for self-decontamination.
  // The heater can be used for periodic creep compensation in prolongued high humidity exposure.
  // For normal operation, leave the heater turned off.

  sht4.setHeater(SHT4X_NO_HEATER);

  
  switch (sht4.getHeater()) {
     case SHT4X_NO_HEATER: 
       Serial.println(F("SHT40 Heater turned OFF"));
       break;
     case SHT4X_HIGH_HEATER_1S: 
       Serial.println(F("SHT40 Heater: High heat for 1 second"));
       break;
     case SHT4X_HIGH_HEATER_100MS: 
       Serial.println(F("SHT40 Heater: High heat for 0.1 second"));
       break;
     case SHT4X_MED_HEATER_1S: 
       Serial.println(F("SHT40 Heater: Medium heat for 1 second"));
       break;
     case SHT4X_MED_HEATER_100MS: 
       Serial.println(F("SHT40 Heater: Medium heat for 0.1 second"));
       break;
     case SHT4X_LOW_HEATER_1S: 
       Serial.println(F("SHT40 Heater: Low heat for 1 second"));
       break;
     case SHT4X_LOW_HEATER_100MS: 
       Serial.println(F("SHT40 Heater: Low heat for 0.1 second"));
       break;
  }

  
  //*********************************************************************
  //*************ADVANCED SETUP IS OVER - LET'S CHECK THE CHIP ID!*******

  
  if (! sht4.begin(&Wire1)) {
    Serial.println(F("SHT40 sensor not found!"));
    while (1) ;
  }
   else
  {
    Serial.print(F("SHT40 detected!\t"));
    Serial.print(F("Serial number:\t"));
    Serial.println(sht4.readSerial(), HEX);    
  } 
  Serial.println(F("----------------------------------"));
 
}
  //*********************************************************************
  //*************NOW LET'S START MEASURING*******************************
  
void loop() {
  
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  
  float t = temp.temperature;
  Serial.println("Temp *C = " + String(t)); 
  float h = humidity.relative_humidity;
  Serial.println("Hum. % = " + String(h)); 

 

  delay(1000);
}