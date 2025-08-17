#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// WiFi credentials
char ssid[] = "Tenda_FB1FB0";     
char pass[] = "southnorth959";    

// ThingSpeak settings
char server[] = "api.thingspeak.com";
String apiKey = "07CG3OK2HNE764IH";   // Your Write API Key

WiFiClient client;

int sensorPin = A0;   // Soil Moisture Sensor AO connected to A0
int sensorValue = 0;

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }
  Serial.println("\n✅ Connected to WiFi!");
}

void loop() {
  // Read soil moisture value
  sensorValue = analogRead(sensorPin);

  Serial.print("🌱 Soil Moisture (Analog): ");
  Serial.println(sensorValue);

  // Send to ThingSpeak using GET
  if (client.connect(server, 80)) {
    String url = "/update?api_key=" + apiKey + "&field1=" + String(sensorValue);

    Serial.println("➡ Sending GET: " + url);

    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();

    // Print server response
    while (client.connected()) {
      if (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.println("🔎 Response: " + response);
      }
    }
  } else {
    Serial.println("❌ Connection failed");
  }

  client.stop();
  delay(20000); // >=15s required by ThingSpeak free tier
}
