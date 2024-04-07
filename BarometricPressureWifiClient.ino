#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <LOLIN_HP303B.h>

LOLIN_HP303B HP303BPressureSensor = LOLIN_HP303B();

const char* ssid = "Espap1";
const char* password = "thereisnospoon";
const char* serverUrl = "http://192.168.4.1/update";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // Set WiFi to station mode
  WiFi.begin(ssid, password);
  // Monitor: Check WiFi connection status
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  HP303BPressureSensor.begin();
}


void loop() {
  // Monitor: Read temperature and pressure data from the sensor
  int32_t temperature = 0, pressure = 0;
  HP303BPressureSensor.measureTempOnce(temperature);
  HP303BPressureSensor.measurePressureOnce(pressure);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("HTTP Request Data: ");
  // Analyze: Construct HTTP request data from sensor readings
  String httpRequestData = "temperature=" + String(temperature) + "&pressure=" + String(pressure); 

  Serial.println(httpRequestData);

  // Plan: Continue to check the WIFI as it was a constant issue
  if (WiFi.status() == WL_CONNECTED) {
    int32_t temperature = 0, pressure = 0;
    HP303BPressureSensor.measureTempOnce(temperature);
    HP303BPressureSensor.measurePressureOnce(pressure);
    // Execute: Send the data to the server
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "temperature=" + String(temperature) + "&pressure=" + String(pressure); 
    int httpResponseCode = http.POST(httpRequestData);
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);
    
    // Knowledge update: Process server response
    if(httpResponseCode > 0) {
        String responsePayload = http.getString();
        Serial.print("Response payload: ");
        Serial.println(responsePayload);
        Serial.println("Data sent successfully");
    } else {
      Serial.print("Error sending data: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(5000);
}
