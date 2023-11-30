#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
bool device1 = 0;
bool device2 = 0;
bool device3 = 0;
bool device4 = 0;
int speed = 0;
// Update these with your WiFi and MQTT broker information
const char *ssid = "Onwords";
const char *password = "nikhil8182";
const char *mqttServer = "mqtt.onwords.in";
const int mqttPort = 1883;
const char *mqttUser = "Nikhil";
const char *mqttPassword = "Nikhil8182";

const char *subscribeTopic = "topic/sub";
const char *publishTopic = "topic/pub";

WiFiClient espClient;
PubSubClient client(espClient);

String message; // Variable to store the received message

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT broker
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }

  // Subscribe to the topic
  client.subscribe(subscribeTopic);

  // Set the callback function for handling incoming messages
  client.setCallback(callback);
}

void loop() {
  client.loop();
  if (Serial.available() > 0)
  {
    char data = Serial.read();
    if (data == 'A')
    {
      device1 = !device1;
    }
    StaticJsonDocument<200> jsonDocument;
    jsonDocument["device1"] = device1;
    jsonDocument["device2"] = device2;
    jsonDocument["device3"] = device3;



    String jsonString;
    serializeJson(jsonDocument, jsonString);

    // Publish the JSON document
    client.publish(publishTopic, jsonString.c_str());
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  DynamicJsonDocument doc(2000);
  DeserializationError error = deserializeJson(doc, message); // Deserialize the message into the JSON document
  if (error) {
    Serial.print("Error deserializing JSON: ");
    Serial.println(error.c_str());
    return;
  }
  if (doc.containsKey("device1")) {
    device1 = doc["device1"];
    Serial.println("L1");

  }
  if (doc.containsKey("device2")) {
    device2 = doc["device2"];
    Serial.println("L2");
  }
  if (doc.containsKey("device3")) {
    device3 = doc["device3"];
    Serial.println("L3");
  }
  if (doc.containsKey("device4")) {
    device4 = doc["device4"];
    Serial.println("F1");
  }
  if (doc.containsKey("speed")) {
    speed = doc["speed"];
    Serial.print("U");
    Serial.println(speed);

  }
  StaticJsonDocument<200> Document;
  Document["device1"] = device1;
  Document["device2"] = device2;
  Document["device3"] = device3;



  String jsonString;
  serializeJson(Document, jsonString);

  // Publish the JSON document
  client.publish(publishTopic, jsonString.c_str());
}
