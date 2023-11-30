#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(Serial2, 16,  17);

const char *ssid = "Huawei Y7";
const char *password = "12345678"; 

const char* mqtt_broker = "mqtt.onwords.in";
const char* mqtt_username = "Nikhil";
const char* mqtt_password = "Nikhil8182";
const int mqtt_port = 1883;
String topicStr1 = "onwords/" + PId + "/status";
const char* topic1 = topicStr1.c_str();
String topicStr2 = "onwords/" + PId + "/getCurrentStatus";
const char* topic2 = topicStr2.c_str();
String topicStr3 = "onwords/" + PId + "/currentStatus";
const char* topic3 = topicStr3.c_str();

int v = 0;
int i = 0;
int p = 0;
int e = 0;
int pf= 0;

void callback(char* topic, byte* payload, unsigned int length);
void setup() {

  // pzem.resetEnergy()
    
   Serial.begin(9600);

  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  // Connecting to the MQTT broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public EMQX MQTT broker connected");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe(topic3);

}

 //Serial.print("Message arrived in topic: ");
 // Serial.println(topic);
  //Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
   // Serial.print((char)payload[i]);
  }
  //Serial.println();

  DynamicJsonDocument doc(2000);

  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Deserialization failed: ");
    Serial.println(error.c_str());
    return;
  }

  if (doc.containsKey("device1")) {
    device1 = doc["device1"];
    Serial.print("d1");
    Serial.println(device1);
  }
  if (doc.containsKey("device2")) {
    device2 = doc["device2"];
    Serial.print("d2");
    Serial.println(device2);
    // Perform your actions with device2
  }
  if (doc.containsKey("device3")) {
    device3 = doc["device3"];
    Serial.print("d3");
    Serial.println(device3);
  }
  if (doc.containsKey("device4")) {
    device4 = doc["device4"];
    Serial.print("d4");
    Serial.println(device4);
    
  }
  if(device4==1)
    {
       if (doc.containsKey("speed")) {
    spd = doc["speed"];
    Serial.print("s");
    Serial.println(spd);
    }
  
    }
  if (doc.containsKey("backLight")) {
    backLight = doc["backLight"];
    Serial.print("b");
    Serial.println(backLight);
  }

  // Send a response if needed
  if (doc.containsKey("request")) {
    String response = doc["request"];
    if (response == "getCurrentStatus") {
      StaticJsonDocument<2000> docs;
      docs["device1"] = device1;
      docs["device2"] = device2;
      docs["device3"] = device3;
       docs["device4"] = device4;
        docs["speed"] = spd;
      serializeJson(docs, currentStatus);
      client.publish(topic3, currentStatus.c_str(), false); // Convert currentStatus to a const char*
    }
  }

 // Serial.println("-----------------------");
}


void  loop() {
client.loop();
Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(),  HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float  energy = pzem.energy();
    float frequency = pzem.frequency();
    float  pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current))  {
        Serial.println("Error reading current");
    } else if (isnan(power))  {
        Serial.println("Error reading power");
    } else if (isnan(energy))  {
        Serial.println("Error reading energy");
    } else if (isnan(frequency))  {
        Serial.println("Error reading frequency");
    } else if (isnan(pf))  {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage:  ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current:  ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power:  ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy:  ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency:  ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF:  ");           Serial.println(pf);
    
    }
   


  
String svoltage = String(voltage);
String scurrent  = String(current);
String spower = String(power);
String senergy = String(energy,3);
String  sfrequency = String(frequency,1);
String spf = String(pf);

 client.publish(topic3,"{voltage}:\voltage\");
  
 
  delay(5000);  //Post  Data at every 5 seconds
}
