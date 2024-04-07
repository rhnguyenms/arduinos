#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);

const char* ssid = "Espap1";
const char* password = "thereisnospoon";
const char* serverUrl = "http://192.168.4.1/update";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  // Monitor: Check the WiFi connection status
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Wire.begin();
}

void loop() {
  // Monitor: Attempt to read data from the SHT3X sensor
  if (sht30.get() == 0) { 
    // Analyze: Print the sensor readings to the serial monitor
    Serial.print("Temperature in Celsius: ");
    Serial.println(sht30.cTemp);
    Serial.print("Relative Humidity: ");
    Serial.println(sht30.humidity);

    // Plan: Check if WiFi is connected before attempting to send data
    if (WiFi.status() == WL_CONNECTED) {
      // Execute: Set up HTTP client and send data to server
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Construct the HTTP request data string
      String httpRequestData = "temperature=" + String(sht30.cTemp) + "&humidity=" + String(sht30.humidity);
      int httpResponseCode = http.POST(httpRequestData);

      // Knowledge update: Log the HTTP request and server response for testing
      Serial.print("HTTP Request Data: ");
      Serial.println(httpRequestData);
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);

      if (httpResponseCode > 0) {
        String responsePayload = http.getString();
        Serial.println("Server response: " + responsePayload);
      } else {
        Serial.println("Failed to send data.");
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected.");
    }
  } else {
    Serial.println("Error: Failed to read from sensor.");
  }
  delay(5000);
}
