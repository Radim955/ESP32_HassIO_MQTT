/*
 * Author:  Radim Lipka
 * Project: ESP32, School project
 * Sensors: DHT11, AM312, LED diode
 */

/*************************** LIBRARIES **********************************/
// MQTT Client library
#include <PubSubClient.h>

// DHT Sensor library
#include <DHT.h>

// WIFI library
#include "WiFi.h"
/*************************** LIBRARIES **********************************/

/********************** VARIABLES & DEFINES *****************************/
// WIFI Access point information
#define WLAN_SSID "Brloh"
#define WLAN_PASS "Kokokodak"

// MQTT server information
#define MQTT_SERVER "192.168.100.101"
#define MQTT_PORT 1883
#define MQTT_USER "Project"
#define MQTT_PASS "Project"
#define MQTT_CLIENT_ID "ESP32"

// PINS sources (GPIO numbers)
#define PINDHT 5
#define PINLED 16
#define PINMOT 12

// Values from sensors
float temperature = 0;
float humidity = 0;
bool  motion = false;

// Instances of external libraries
DHT sensorDHT(PINDHT, DHT11);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// MQTT Topics
char * TOPIC_TEMP = "project/temperature";
char * TOPIC_HUM  = "project/humidity";
char * TOPIC_MOT  = "project/motion";
char * TOPIC_LED_STATUS = "project/led/status";
char * TOPIC_LED_SWITCH = "project/led/switch";

// Measuring delays
#define DELAY_TEMP_HUM 10000 // miliseconds
#define DELAY_MOTION 100 // miliseconds

// Timers
long timerTempHum = 0;
long timerMotion = 0;

// Subscribed message
char message[35];

// Help variables
int lastMotionValue = 0;
bool turnedOnBySwitch = 0;
/************************ GLOBAL VARIABLES ******************************/

void mqttReceiving(char * topic, byte * payload, unsigned int length);
void mqttReconnect();

void setup() {

  // --- Setting serial communication ---
  Serial.begin(115200);
  
  // --- Connecting to WIFI network ---
  Serial.println();
  Serial.print("WIFI - Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.disconnect();
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // --- MQTT server configuration ---
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttReceiving);  // When received subscribed topics

  // --- PIN settings
  pinMode(PINLED, OUTPUT);
  pinMode(PINMOT, INPUT);

  // --- Sensors settings
  sensorDHT.begin();
}

// Repeated function - body of whole application
void loop() {
  
  // Reconnecting client when disconnected
  if (! mqttClient.connected()) {
    mqttReconnect();
  }

  // Listening for subscribed topics
  mqttClient.loop();

  // Publish humuduty and temperature values
  if(millis() > timerTempHum) {

    temperature = sensorDHT.readTemperature();
    humidity    = sensorDHT.readHumidity();

    // Publish temperature
    if (!isnan(temperature)) {
      //snprintf (message, 35, "\{\"temperature\":\"%.1f\"\}", temperature);
      snprintf (message, 35, "%.1f", temperature);
      mqttClient.publish(TOPIC_TEMP, message);
      Serial.print("Teplota: ");
      Serial.println(temperature);
      Serial.println(message);
    }
    // Publish humidity
    if (!isnan(humidity)) {
      //snprintf (message, 35, "\{\"humidity\":\"%.1f\"\}", humidity);
      snprintf (message, 35, "%.1f", humidity);
      mqttClient.publish(TOPIC_HUM, message);
      Serial.print("Vlhkost: ");
      Serial.println(humidity);
      Serial.println(message);
    }
    timerTempHum = millis() + DELAY_TEMP_HUM;
  }

  // Publish motion sensor value
  if(millis() > timerMotion) {
    if(turnedOnBySwitch != 1) {
      if(lastMotionValue != digitalRead(PINMOT)) {
        lastMotionValue = digitalRead(PINMOT);
        if(lastMotionValue == 1) {
          //mqttClient.publish(TOPIC_MOT, "\{\"state\":\"1\"\}");
          mqttClient.publish(TOPIC_MOT, "1");
          Serial.println("Senzor pohybu: ON");
        } else {
          //mqttClient.publish(TOPIC_MOT, "\{\"state\":\"0\"\}");
          mqttClient.publish(TOPIC_MOT, "0");
          Serial.println("Senzor pohybu: OFF");
        }
      }
    }
    timerMotion = millis() + DELAY_MOTION;
  }
}


// Get data from subscribed topics
void mqttReceiving(char * topic, byte * payload, unsigned int length) {
  
  Serial.print("Subscribed message received: ");
  Serial.println(topic);

  // Serial.print("Payload: \{\"state\":\"");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(topic, TOPIC_MOT) == 0) {
    if ((char)payload[0] == '1') {
      //mqttClient.publish(TOPIC_LED_STATUS, "\{\"state\":\"1\"\}");  
      mqttClient.publish(TOPIC_LED_STATUS, "1");
      digitalWrite(PINLED, HIGH);
    } else {
      //mqttClient.publish(TOPIC_LED_STATUS, "\{\"state\":\"0\"\}"); 
      mqttClient.publish(TOPIC_LED_STATUS, "0");   
      digitalWrite(PINLED, LOW);
    }
  }

  if(strcmp(topic, TOPIC_LED_SWITCH) == 0) {
    if ((char)payload[0] == '1') {
      //mqttClient.publish(TOPIC_LED_STATUS, "\{\"state\":\"1\"\}");  
      turnedOnBySwitch = 1;
      mqttClient.publish(TOPIC_LED_STATUS, "1");
      digitalWrite(PINLED, HIGH);
    } else {
      //mqttClient.publish(TOPIC_LED_STATUS, "\{\"state\":\"0\"\}"); 
      turnedOnBySwitch = 0;
      mqttClient.publish(TOPIC_LED_STATUS, "0");   
      digitalWrite(PINLED, LOW);
    }
  }
}

// When connection to server is lsot, we need to reconnect
void mqttReconnect() {
  
  // Loop until reconnected
  while (!mqttClient.connected()) {
    
    Serial.print("Trying to connect to MQTT server ...");
    if (mqttClient.connect(MQTT_CLIENT_ID)) {
      
      Serial.println("connected");
      // Publish our topics
      mqttClient.publish(TOPIC_TEMP, "-");
      mqttClient.publish(TOPIC_HUM, "-");
      mqttClient.publish(TOPIC_MOT, "0");
      mqttClient.publish(TOPIC_LED_STATUS, "0");
      // Subscribe control of our light
      mqttClient.subscribe(TOPIC_LED_SWITCH);
      mqttClient.subscribe(TOPIC_MOT);
    } else {
      
      Serial.print("failed, status code =");
      Serial.print(mqttClient.state());
      Serial.println("trying again in 5 seconds");
      // Wait before retrying
      delay(5000);
    }
  }
  Serial.println("Connected to MQTT Succesfully!");
}

