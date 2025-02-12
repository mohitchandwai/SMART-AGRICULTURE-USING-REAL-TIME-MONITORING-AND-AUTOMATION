#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "env.h"

float co2, light, soil_moisture;
String response1;  // Stores soil moisture status (Dry/Wet)

// WiFi credentials
const char WIFI_SSID[] = "********";
const char WIFI_PASSWORD[] = "************";

// Device name from AWS
const char THINGNAME[] = "esp8266";

// MQTT broker host address from AWS
const char MQTT_HOST[] = "<MQTT ENDPOINT>";

// MQTT topics
const char AWS_IOT_PUBLISH_TOPIC[] = "esp8266/pub";
const char AWS_IOT_SUBSCRIBE_TOPIC[] = "esp8266/sub";

// Publishing interval
const long interval = 5000;

// Timezone offset from UTC
const int8_t TIME_ZONE = -5;

// Last time message was sent
unsigned long lastMillis = 0;

// Soil moisture sensor pins
#define DIGITAL_PIN D2   // Soil Moisture Digital Output
#define ANALOG_PIN A0    // Soil Moisture Analog Output
#define LED_PIN D1       // LED/Pump Indicator

// WiFiClientSecure object for secure communication
WiFiClientSecure net;

// X.509 certificate for the CA
BearSSL::X509List cert(cacert);

// X.509 certificate for the client
BearSSL::X509List client_crt(client_cert);

// RSA private key
BearSSL::PrivateKey key(privkey);

// MQTT client instance
PubSubClient client(net);

// Function to connect to NTP server and set time
void NTPConnect() {
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 1510592825) { // January 13, 2018
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
}

// Callback function for message reception
void messageReceived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Function to connect to AWS IoT Core
void connectAWS() {
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  NTPConnect(); // Connect to NTP server

  // Set CA and client certificate
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);

  Serial.println("Connecting to AWS IoT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(1000);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}

// Function to read soil moisture sensor
void readSoilMoisture() {
  int digitalMoisture = digitalRead(DIGITAL_PIN);  // Read digital output (0 or 1)
  int analogMoisture = analogRead(ANALOG_PIN);    // Read analog output (0-1023)
  
  // Convert analog value to percentage (adjust based on calibration)
  soil_moisture = map(analogMoisture, 1023, 300, 0, 100);
  soil_moisture = constrain(soil_moisture, 0, 100);

  // Check digital output and set response1 status
  if (digitalMoisture == HIGH) {  // LOW = Soil is dry
    response1 = "DRY";
    digitalWrite(LED_PIN, HIGH);  // Turn on LED/Pump
  } else {
    response1 = "WET";
    digitalWrite(LED_PIN, LOW);   // Turn off LED/Pump
  }
}

// Function to publish message to AWS IoT Core
void publishMessage() {
  readSoilMoisture();  // Get soil moisture readings

  // Generate random CO2 and pH values
  float co2 = random(0, 1024);
  float ph = random(5.0, 9.0);

  // Read light sensor value
  light = analogRead(A0);
  
  // Convert light reading to lux
  float A = 500, B = -1.2;
  float Vout = (light / 1023.0) * 3.3;
  float Vmax = 3.3;
  float lux = A * pow((Vout / (Vmax - Vout)), B);
  light = lux;

  // Get current timestamp
  char timestamp[32];
  time_t now = time(nullptr);
  strftime(timestamp, 32, "%Y-%m-%dT%H:%M:%S", localtime(&now));

  Serial.print("Timestamp: ");
  Serial.print(timestamp);
  Serial.print(" CO2: ");
  Serial.print(co2);
  Serial.print(" ppm Light: ");
  Serial.print(light);
  Serial.print(" lux pH: ");
  Serial.print(ph);
  Serial.print(" Soil Moisture: ");
  Serial.print(soil_moisture);
  Serial.print("% Response1: ");
  Serial.println(response1);

  // Create JSON document for message
  StaticJsonDocument<200> doc;
  doc["timestamp"] = timestamp;
  doc["co2"] = co2;
  doc["light"] = light;
  doc["ph"] = ph;
  doc["soil_moisture"] = soil_moisture;
  doc["response1"] = response1;  // Add soil moisture status

  // Serialize JSON document
  char jsonBuffer[200];
  serializeJson(doc, jsonBuffer);

  // Publish message to MQTT topic
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  Serial.begin(115200);

  // Set up soil moisture sensor and LED
  pinMode(DIGITAL_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Connect to AWS IoT Core
  connectAWS();
}

void loop() {
  // Publish data at intervals
  if (millis() - lastMillis > interval) {
    lastMillis = millis();
    if (client.connected()) {
      publishMessage();
    }
  }
  
  client.loop();
}
