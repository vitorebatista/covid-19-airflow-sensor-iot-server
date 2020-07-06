#include "EspMQTTClient.h"
#include <ArduinoJson.h>
#include "secrets.h"

#define LED_RED 1
#define LED_YELLOW 3
#define LED_GREEN 15

const char *wifiSsid = WIFI_SSID;
const char *wifiPassword = WIFI_PASSWORD;
const char *mqttServerIp = MQTT_SERVER_IP;
const char *mqttUsername = MQTT_USERNAME;
const char *mqttPassword = MQTT_PASSWORD;
const char *mqttClientName = MQTT_CLIENT_NAME;
const short mqttServerPort = MQTT_SERVER_PORT;

int frequency = 1000; //Specified in Hz
int buzzPin = 14;
int timeOn = 500;  //specified in milliseconds
int timeOff = 500; //specified in millisecods

EspMQTTClient client(wifiSsid, wifiPassword, mqttServerIp, mqttUsername,
                     mqttPassword, mqttClientName, mqttServerPort);

void setup()
{
  Serial.begin(115200);

  // Optionnal functionnalities of EspMQTTClient :
  client.enableDebuggingMessages();                                          // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater();                                             // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline"); // You can activate the retain flag by setting the third parameter to true
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  StaticJsonDocument<200> jsonDoc;

  char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  // JsonObject &root = jsonBuffer.parseObject(json);
  auto error = deserializeJson(jsonDoc, json);

  if (error)
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    // return false;
  }
  else
  {
    const char *sensor = jsonDoc["sensor"];
    long time = jsonDoc["time"];
    double latitude = jsonDoc["data"][0];
    double longitude = jsonDoc["data"][1];
    Serial.println(sensor);
  }

  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("mytopic/test", [](const String &payload) {
    Serial.println(payload + "!!!");
  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("buzzer/action/#", [](const String &topic, const String &payload) {
    Serial.println(topic + ": " + payload);
    tone(buzzPin, frequency);
    delay(timeOn);
    noTone(buzzPin);
    delay(timeOff);
  });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/test", "This is a message sent 5 seconds later");
  });
}

void loop()
{
  client.loop();

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  delay(1000); // wait for a second
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, LOW);
  delay(1000); // wait for a second
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(1000); // wait for a second
}
