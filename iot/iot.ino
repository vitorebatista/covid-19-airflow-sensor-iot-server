#include "EspMQTTClient.h"
#include <ArduinoJson.h>
#include "secrets.h"

#define LED_RED 15     //D8 (GPIO1)
#define LED_YELLOW 13  //D7 (GPIO13)
#define LED_GREEN 12   //D6 (GPIO12)
#define FLOW_SENSOR 10 //SD3 (GPIO10)
#define BUZZER 14      //D5 (GPIO14)

const char *wifiSsid = WIFI_SSID;
const char *wifiPassword = WIFI_PASSWORD;
const char *mqttServerIp = MQTT_SERVER_IP;
const char *mqttUsername = MQTT_USERNAME;
const char *mqttPassword = MQTT_PASSWORD;
const char *mqttClientName = MQTT_CLIENT_NAME;
const short mqttServerPort = MQTT_SERVER_PORT;

bool isRed = false;
// Buzzer configurations
int frequency = 1000; //Specified in Hz
int timeOn = 500;     //specified in milliseconds
int timeOff = 500;    //specified in millisecods

// Flow Sensor
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

StaticJsonDocument<200> flowJson;

EspMQTTClient client(wifiSsid, wifiPassword, mqttServerIp, mqttUsername,
                     mqttPassword, mqttClientName, mqttServerPort);

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup()
{
  Serial.begin(115200);
  delay(20);
  Serial.println('\n');

  // Optionnal functionnalities of EspMQTTClient :
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater();    // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);

  pinMode(FLOW_SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
}

// This function is called once everything is connected (Wifi and MQTT)
void onConnectionEstablished()
{

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("alert/semaphore", [](const String &topic, const String &payload) {
    Serial.println(topic + ": " + payload);

    changeStatus(payload);
  });

  client.subscribe("alert/buzzer", [](const String &topic, const String &payload) {
    Serial.println(topic);

    if (payload == "ON")
    {
      playBuzzer();
    }
    else if (payload == "OFF")
    {
      stopBuzzer();
    }
    else
    {
      playBuzzer();
      stopBuzzer();
    }
  });
}

void changeStatus(const String color)
{
  digitalWrite(LED_RED, color == "RED" ? HIGH : LOW);
  digitalWrite(LED_YELLOW, color == "YELLOW" ? HIGH : LOW);
  digitalWrite(LED_GREEN, color == "GREEN" ? HIGH : LOW);

  if (color == "RED" && !isRed)
  {
    playBuzzer();
    stopBuzzer();
  }
  isRed = color == "RED";
}

void playBuzzer()
{
  tone(BUZZER, frequency);
  delay(timeOn);
}

void stopBuzzer()
{
  noTone(BUZZER);
  delay(timeOff);
}

void flowLoop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {

    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate)); // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t"); // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");

    flowJson["rate"] = int(flowRate);
    flowJson["total"] = totalMilliLitres;
    flowJson["time"] = currentMillis;

    String flowString;
    serializeJson(flowJson, flowString);
    if (client.isConnected())
    {
      client.publish("sensor/flow", flowString);
    }
  }
}

void loop()
{
  client.loop();

  flowLoop();
}