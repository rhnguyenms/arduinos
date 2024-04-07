#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <cstdlib>

const char* ssid = "Espap1";
const char* password = "thereisnospoon";
ESP8266WebServer server(80);

float lastTemperature = 0.0;
int32_t lastPressure = 0;
float lastHumidity = 0.0;
String backgroundColor = "#FFFFFF";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  randomSeed(analogRead(0));

  // Monitor: Serving the sensor data on the root URL, incorporating the background color which can change and the temp, pressure and humidity changes.
  server.on("/", HTTP_GET, []() {
    String webpage = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='3'></head>"
                     "<body style='background-color:" + backgroundColor + ";'>";
    webpage += "<h1>Sensor Data</h1>"
               "<p>Temperature: " + String(lastTemperature) + " °C</p>"
               "<p>Pressure: " + String(lastPressure) + " Pa</p>"
               "<p>Humidity: " + String(lastHumidity) + "%</p>";
    webpage += "</body></html>";
    server.send(200, "text/html", webpage);
  });

  // Analyze and Plan: Upon receiving sensor data, it updates the stored values.
  server.on("/update", HTTP_POST, []() {
    if (server.hasArg("temperature")) {
      lastTemperature = server.arg("temperature").toFloat();
      Serial.println("Received temperature: " + String(lastTemperature));
    }
    if (server.hasArg("pressure")) {
      lastPressure = server.arg("pressure").toInt();
      Serial.println("Received pressure: " + String(lastPressure));
    }
    if (server.hasArg("humidity")) {
      lastHumidity = server.arg("humidity").toFloat();
      Serial.println("Received humidity: " + String(lastHumidity));
    }
    server.send(200, "text/plain", "Data updated");
    // Knowledge: Updating these variables affects future responses to GET requests on "/",
    // demonstrating learning and adaptation.
  });

  // Execute: Changes the background color upon a button press, demonstrating an adaptive response.
  server.on("/button", HTTP_POST, []() {
    // Generate a new random color as a direct response (Plan and Execute phases).
    backgroundColor = "#" + String(random(0, 256), HEX) + String(random(0, 256), HEX) + String(random(0, 256), HEX);
    server.send(200, "text/plain", "Background color changed");
  });

  server.begin();
}

void loop() {
  // Monitor: The loop function continuously monitors for HTTP requests.
  server.handleClient();
}
