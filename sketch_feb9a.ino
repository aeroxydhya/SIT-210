#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22
#define FANRELAY 7      // Relay controlling fan (active-LOW)
#define LIGHTLED 8      // LED for Light
#define ACLED 9         // LED for AC

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(FANRELAY, OUTPUT);
  pinMode(LIGHTLED, OUTPUT);
  pinMode(ACLED, OUTPUT);

  // Initialize everything OFF
  digitalWrite(FANRELAY, HIGH); // OFF
  digitalWrite(LIGHTLED, LOW);  // OFF
  digitalWrite(ACLED, LOW);     // OFF
}

void loop() {
  // Read temperature and humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp)) {
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" °C, Humidity: ");
    Serial.println(hum);
    
    // Automatic AC LED control
    if (temp > 27.0) {
      digitalWrite(ACLED, HIGH); // AC LED ON
    } 
    else if (temp < 21.0) {
      digitalWrite(ACLED, LOW);  // AC LED OFF
    }
  } 
  else {
    Serial.println("Failed to read DHT22");
  }

  // Check serial commands (fan/light/ac override)
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();

    if (cmd == "fan on") digitalWrite(FANRELAY, LOW);
    else if (cmd == "fan off") digitalWrite(FANRELAY, HIGH);
    else if (cmd == "light on") digitalWrite(LIGHTLED, HIGH);
    else if (cmd == "light off") digitalWrite(LIGHTLED, LOW);
    else if (cmd == "ac on") digitalWrite(ACLED, HIGH);
    else if (cmd == "ac off") digitalWrite(ACLED, LOW);
  }

  delay(2000); // Read DHT every 2 seconds
}
