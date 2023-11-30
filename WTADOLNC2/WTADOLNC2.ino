#include <WiFi.h>
#include <HTTPClient.h>// library
//#include <HttpClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h> // inbuild memory
#include <PubSubClient.h>
#include <NewPing.h>
int lock=1;
#include"function.h" /// function

 
unsigned int distance=0;
unsigned long previousMillis = 0;  // Variable to store the last time the LED was updated
const long interval = 5000;
NewPing sonar(TRIGGER_PIN, ECHO_PIN);

int count = 0;
int count1 = 1;
int count2 = 0;
WiFiServer server(8182);
bool parinMode = true;

String result;
String store;
String action;

char network[200];
char ssid[32];
char password[64];
char hardwarests[200];
char softreset[200];
char readings[200];

char distances[200];


const char* mqttServer = "mqtt.onwords.in";// mqttt
const int mqttPort = 1883;
const char* mqttUsername = "Nikhil";
const char* mqttPassword = "Nikhil8182";



String hotspot = "onwords-" + PId; //hotsopt name with pid
const char* hotspot_cstr = hotspot.c_str();

String mqttClientId1 = PId;
const char* mqttClientId = mqttClientId1.c_str();  // host id.

String response;

void setup()
{

  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(R1, OUTPUT); //PINMODE DECLARATION
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(led, OUTPUT);//led
  pinMode(FLOAT_HIGH, INPUT_PULLUP);
  pinMode(FLOAT_LOW, INPUT_PULLUP);
  pinMode(SUMP, INPUT_PULLUP);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
   pinMode(led2,OUTPUT);
  digitalWrite(R1, LOW);
  digitalWrite(led, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
   digitalWrite(led2, HIGH);
  EEPROM.begin(EEPROM_SIZE);
  Serial.println("read from eeprom");
  String ssid = handlereadssid();
  String password = handlereadpassword() ;
  Serial.println("ssid = " + ssid);
  Serial.println("password = " + password);

  if (ssid == "")
  {
    Serial.println("PARING MODE ON");
    delay(2000);
    
    for (int i=0;i<3;i++)
    {digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    }
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hotspot_cstr, "");
    server.begin();
    Serial.println(WiFi.softAPIP());
    delay(2000);
  }
  else
  {
     EEPROM.get(addr1 ,data2);
  EEPROM.get(addr2 ,data1);
  minDistance=data1;
  maxDistance=data2;
    Serial.println("connecting to ssid id =" + ssid + " password =" + password);
    delay(20);
    WiFi.begin(ssid.c_str(), password.c_str());
   handle_reconnecting_wifi();
    Serial.println("connected to wifi");
    digitalWrite(led, HIGH);
    parinMode = false;
    delay(200);
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

  }

}
void handle_reconnecting_wifi()
{
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);

    int buttonState = digitalRead(buttonPin);
    Serial.println(buttonState);
    if (buttonState == LOW) {
      if (!buttonPressed) {
        buttonPressStart = millis();
        buttonPressed = true;
      }
      if (millis() - buttonPressStart >= 5000) {

        for (int i = 0 ; i < 512  ; i++) {
          EEPROM.write(i, 0);
        }

        EEPROM.end();
        Serial.println("EEPROM Clear Done!");
        ESP.restart();
        delay(100);

      }
    }
  }
}

void loop() {

  if (parinMode)
  {

    WiFiClient clients;
    clients = server.available();
    if (!clients) { // WIFIMANAGER
      return;

    }
    count1=0;
    Serial.println("New client");
    while (!clients.available()) {

      delay(1);

    }
    delay(2000);
    String jsonPayload = clients.readString();
    Serial.println(String(jsonPayload));
    int bodyStart = jsonPayload.indexOf("\r\n\r\n") + 4;
    String body = jsonPayload.substring(bodyStart);
    Serial.println("Body: " + body);
    clients.flush();
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);
    const char* ssid = doc["ssid"].as<const char*>();
    const char* password = doc["password"].as<const char*>();
    Serial.println("SSID: " + String(ssid));
    Serial.println("Password: " + String(password));
    Serial.println("HTTP/1.1 200 OK");
    String x = "connecting to " + String(ssid);
    clients.println("Content-Type: text/plain");
    clients.println("");
    clients.println("ok");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
      digitalWrite(led, HIGH);
      delay(400);
      digitalWrite(led, LOW);
      delay(400);
    }
    result = ssid + String(':') + password;
    handlewritestring(result);
    digitalWrite(led, HIGH);
    Serial.println("Connected to WiFi");
    delay(1);
    Serial.println("Client disconnected");
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    parinMode = false;
  }
  else
  {
    handlecleareeprom();
  }
  if (!client.connected())
  {
    reconnect();
    
   if(lock==1)   {  independent_function();   } 
  }
  client.loop();

 
   if (count1 == 0) {
    if (digitalRead(FLOAT_LOW) == LOW&&count2==0) {
      maxDistance = sonar.ping_cm(); // Store maximum distanc
        EEPROM.put(addr1, maxDistance);
        EEPROM.commit(); 
      Serial.println("Maximum distance detected");
       Serial.println(maxDistance);
      count2=1;
         }
     if (digitalRead(FLOAT_HIGH) == LOW) {
      minDistance = sonar.ping_cm(); // Store minimum distance
      EEPROM.put(addr2 , minDistance);
      EEPROM.commit(); 
     Serial.println("Minimum distance detected");
      Serial.println(minDistance);
      count1 = 1;
    }
    client.publish(topic7, "{\"readings\":\"calibrating\"}");
client.publish(topic9, "{\"readings\":\"calibrating\"}");

  }

 
  
    unsigned int distance = sonar.ping_cm();

    // Calculate the percentage of the distance between the minimum and maximum distances
     percentage = map(distance, minDistance, maxDistance, 100, 0);
     Serial.println(percentage);
    if(percentage>=100)
    {
      percentage=100;
    }
    
     if(percentage<=0)
    {
      percentage=0;
    }
 
   if(lock==1) 
   { 
    independent_function(); 
   } 
 
  if (count == 50) {
   
    StaticJsonDocument <2000>docsss;
    Serial.println("insideeeee");
    docsss["readings"] = percentage;
    serializeJson(docsss, readings);
    Serial.println(readings);
    client.publish(topic7, readings, true);
    count = 0;
   
  }
  count++;
delay(100);
if(floatHighState==1)
{
  if(digitalRead(FLOAT_HIGH) ==HIGH)
 {
  floatHighState=0;
  lock=1;
 } 
 else
 {
  lock=0; 
 }
}
}

void reconnect()
{
  while (!client.connected()) {
    String ssid = handlereadssid();
    String password = handlereadpassword() ;
    Serial.println("ssid = " + ssid);
    Serial.println("password = " + password);
    delay(200);
    WiFi.begin(ssid.c_str(), password.c_str());
    handle_reconnecting_wifi();
    Serial.println("Connected to WiFi");
    if (client.connect(mqttClientId, mqttUsername, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(topic1, subscribeQoS);
      client.subscribe(topic2, subscribeQoS);
      client.subscribe(topic5, subscribeQoS);
      client.publish(topic3, currentStatus, false);

      client.publish(topic4, productid, publishQoS);
      client.publish(topic6, softreset, publishQoS);
      client.publish(topic7, readings, true);
      client.publish(topic8, distances, publishQoS);
      digitalWrite(led, HIGH);
        if(lock==1)   {  independent_function();   } 
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      digitalWrite(led, LOW);
      delay(500);
    if(lock==1)
    {
      independent_function();
    } 
    }
      if(lock==1)  
      {
        independent_function(); 
        } 
  }
}

void callback(char* topic, byte * payload, unsigned int length) {
  Serial.print("Received message on topic: ");
  Serial.print(topic);
  Serial.print("Message: ");

  String aa = "";
  for (int i = 0; i < length; i++) {
    aa += (char)payload[i];
  }
  Serial.println(aa);
  DynamicJsonDocument doc(6000);
  DeserializationError error = deserializeJson(doc, aa); // Deserialize the message into the JSON document
  if (error) {
    Serial.print("Error deserializing JSON: ");
    Serial.println(error.c_str());
    return;
  }
 if (doc.containsKey("device1")) {
    device1 = doc["device1"];
    if (device1 == 1)
    {
      if(percentage<100)
      {
      handle_currentstatus(1, 0);
      digitalWrite(R1, HIGH);
      delay(2000);
      digitalWrite(R1, LOW);
      lastState = 1;
    }
    else
    {
      
      handle_currentstatus(0, 1);
    }
    }
  }
  if (doc.containsKey("device2")) {
    device2 = doc["device2"];
    if (device2 == 1)
    {
      handle_currentstatus(0, 1);
      digitalWrite(R2, LOW);
      delay(2000);
      digitalWrite(R2, HIGH);
      lastState = 0;
    }
  }
  String response = doc["request"];
  if (response == "getCurrentStatus")
  {
    if (lastState == 1) {
      StaticJsonDocument <2000>resp;
      Serial.println("insideeeee");
      resp["device1"] = 1;
      resp["device2"] = 0;
      serializeJson(resp, currentStatus);
      Serial.println(currentStatus);
      client.publish(topic3, currentStatus, false);
    }
    if (lastState == 0) {
      StaticJsonDocument <2000>resp;
      Serial.println("insideeeee");
      resp["device1"] = 0;
      resp["device2"] = 1;
      serializeJson(resp, currentStatus);
      Serial.println(currentStatus);
      client.publish(topic3, currentStatus, false);
    }
  }
  int sftret = doc["reset"];
  if (sftret == 1)
  {
    for (int i = 0 ; i < 512  ; i++) {
      EEPROM.write(i, 0);
    }

    EEPROM.end();
    Serial.println("EEPROM Clear Done!");
    delay(100);
    StaticJsonDocument <2000>strs;
    strs["status"] = true;
    serializeJson(strs, softreset);
    Serial.println(softreset);
    client.publish(topic6, softreset, publishQoS);
    delay(500);
    ESP.restart();
  }

}
