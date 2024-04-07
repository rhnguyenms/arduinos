#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Espap1";
const char* password = "thereisnospoon";
const char* serverUrl = "http://192.168.4.1/button";

const int buttonPin = D3;
int lastButtonState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  WiFi.begin(ssid, password);
  // Monitor: Check WiFi connection status
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  // Monitor: Read current state of the button
  int buttonState = digitalRead(buttonPin);
  
  // Analyze: Compare current button state to the last button state
  if (buttonState == HIGH && lastButtonState == LOW) {
    // Plan: Prepare for HTTP POST request if the button was pressed
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverUrl);
    // Execute: Send HTTP POST request
    http.POST("");
    http.end();
    delay(200); 
  }
  lastButtonState = buttonState;
}
