#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 ready");
  } else {
    Serial.println("Error initialising BH1750");
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();

  // Make sure lux is never negative
  if (lux < 0) {
    lux = 0;
  }

  // Print in a clean format for Node-RED
  Serial.print("Light: ");
  Serial.print(lux, 2); // 2 decimal places
  Serial.println(" lx");

  delay(1000); // 1 second interval
}
