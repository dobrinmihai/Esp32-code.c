#include <WiFi.h>              // Library for Wi-Fi functions (ESP32 built-in)
#include <WebServer.h>         // Simple HTTP server library for ESP32/ESP8266
#include <ArduinoJson.h>       // Library for creating and parsing JSON data

// Wi-Fi credentials
const char* ssid = "wifi_name";
const char* password = "password";

// Create a web server object that listens on port 8321
WebServer server(8321);

// Function to simulate sensor data and return it as a JSON string
String getSensorData() {
  StaticJsonDocument<512> doc;           // Create a JSON document
  JsonArray sensors = doc.to<JsonArray>(); // Create a JSON array

  // Simulated temperature sensor data
  JsonObject temp = sensors.createNestedObject();
  temp["name"] = "Temperature";
  temp["type"] = "DHT22";
  temp["value"] = 24.5;

  // Simulated humidity sensor data
  JsonObject hum = sensors.createNestedObject();
  hum["name"] = "Humidity";
  hum["type"] = "DHT22";
  hum["value"] = 60;

  // Convert JSON document to string
  String json;
  serializeJson(sensors, json);
  return json;
}

// Handler for the /status route - returns online status
void handleStatus() {
  server.send(200, "application/json", "{\"online\": true}");
}

// Handler for the /sensors route - returns sensor data
void handleSensors() {
  String json = getSensorData();                     // Get simulated sensor data
  server.send(200, "application/json", json);        // Send it as response
}

void setup() {
  Serial.begin(115200);                              // Start serial communication for debugging

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Wi-Fi connected, print IP address
  Serial.println("\nConnected! IP address:");
  Serial.println(WiFi.localIP());

  // Define HTTP endpoints
  server.on("/status", handleStatus);   // GET /status
  server.on("/sensors", handleSensors); // GET /sensors

  // Start the server
  server.begin();
  Serial.println("Server started on port 8321");
}

void loop() {
  server.handleClient();   // Handle incoming client requests
}
