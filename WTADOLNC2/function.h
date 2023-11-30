#define EEPROM_SIZE 512
#define STRING1_ADDR 0
#define STRING2_ADDR 20
#define RESULT_ADDR 40
#define FLOAT_HIGH 33
#define FLOAT_LOW 25
#define SUMP 26
#define SUMP 26
#define FLOAT_HIGH 33
#define FLOAT_LOW 25

int  onbutton= 15;
int offbutton= 23;
  int maxDistance;
  int minDistance;
  int data1;
  int data2;
  int lastState=0;
const int addr1 =100;
const int addr2 =200;
int percentage=0;
#define TRIGGER_PIN 14
#define ECHO_PIN 27
#define MAX_PERCENTAGE 100
bool  sumpState=0;
bool floatHighState=0;
bool floatLowState=0;
int device1 = 0;
int device2 = 0;//intial state
int device3 = 0;

const int buttonPin = 4;
unsigned long buttonPressStart = 0;
bool buttonPressed = false;

extern String PId = "wtadol006"; // THIS THE PRODUCT ID change if needed
char status[100];
char currentStatus[300];
char productid[100];
String topicStr1 = "onwords/" + PId + "/status";
const char* topic1 = topicStr1.c_str();                 //SUB
String topicStr2 = "onwords/" + PId + "/getCurrentStatus";
const char* topic2 = topicStr2.c_str();                  //SUB
String topicStr3 = "onwords/" + PId + "/currentStatus";
const char* topic3 = topicStr3.c_str();                //pub
String topicStr4 = "onwords/hardReset";
const char* topic4 = topicStr4.c_str();                  //pub
String topicStr5 = "onwords/" + PId + "/softReset";
const char* topic5 = topicStr5.c_str();                   //SUB
String topicStr6 = "onwords/" + PId + "/softReset";
const char* topic6 = topicStr6.c_str();  //pub
String topicStr7 = "onwords/" + PId + "/readings";
const char* topic7 = topicStr7.c_str(); 
String topicStr8 = "onwords/" + PId + "/distance";
const char* topic8 = topicStr8.c_str(); 
String topicStr9 = "onwords/" + PId + "/calibration";
const char* topic9 = topicStr9.c_str();   
String topicStr10 = "onwords/" + PId + "/errormessage";
const char* topic10 = topicStr10.c_str();   
String topicStr11 = "onwords/" + PId + "/notification";
const char* topic11 = topicStr11.c_str();   


const int publishQoS = 1;     // QoS level for publishing messages
const int subscribeQoS = 1;   // QoS level for subscribing messages




WiFiClient espClient;
PubSubClient client(espClient);



int R1 = 32;//MOTOR ON
int led = 21;//led
int R2= 18;//sump
int R3=19;//level
int led2 =2;//MOTOR OFF



String handlereadssid() {
  String ssid;
  for (int i = STRING1_ADDR; i < STRING2_ADDR; i++) {
    char character = EEPROM.read(i);
    if (character == 0) {
      break; // Null terminator, end of string
    }
    ssid += character;
  }
  Serial.println("ssid added: " + ssid);
  return ssid;
}

String handlereadpassword() {
  String password;
  for (int i = STRING2_ADDR; i < RESULT_ADDR; i++) {
    char character = EEPROM.read(i);
    if (character == 0) {
      break; // Null terminator, end of string
    }
    password += character;
  }
  Serial.println("password added: " + password);
  return password;
}

void handlewritestring(String data) {
  int separatorIndex = data.indexOf(':');
  if (separatorIndex != -1) {
    String string1 = data.substring(0, separatorIndex);
    String string2 = data.substring(separatorIndex + 1);

    if (STRING2_ADDR + string1.length() + string2.length() <= EEPROM_SIZE) {
      for (int i = 0; i < string1.length(); i++) {
        EEPROM.write(STRING1_ADDR + i, string1[i]);
      }
      for (int i = 0; i < string2.length(); i++) {
        EEPROM.write(STRING2_ADDR + i, string2[i]);
      }
      EEPROM.commit();

      Serial.println("Strings written to EEPROM");
    } else {
      Serial.println("String length exceeds available EEPROM size. Please reduce the input length.");
    }
  } else {
    Serial.println("Invalid input format. Please provide two strings separated by a colon ':'");
  }
}
void handle_currentstatus(int relay1, int relay2)
{
  StaticJsonDocument<2000> docs;
  docs["device1"] = relay1;
  docs["device2"] = relay2;
  serializeJson(docs, currentStatus);
    Serial.println(currentStatus);
    client.publish(topic3, currentStatus, false);
}


void handlecleareeprom()
{
  int buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if (buttonState == LOW) {
    if (!buttonPressed) {
      buttonPressStart = millis();
      buttonPressed = true;
    }
    if (millis() - buttonPressStart >= 3000) {
      // Clear EEPROM data
      for (int i = 0 ; i < 512  ; i++) {
        EEPROM.write(i, 0);
      }
      // Don't forget EEPROM.end();
      EEPROM.end();
      Serial.println("EEPROM Clear Done!");
      delay(100);
      StaticJsonDocument<2000> pids;
      pids["id"] = PId;
      serializeJson(pids, productid);
      client.publish(topic4, productid, publishQoS);
      delay(500);
      ESP.restart();
    }
  } else {
    buttonPressed = false;
  }
}

void motoron()
{
  digitalWrite(R1, HIGH);
  delay(2000);
  digitalWrite(R1, LOW);
}
void  motoroff()
{
  digitalWrite(led2, LOW);
  delay(2000);
  digitalWrite(led2, HIGH);
}
 void independent_function  (){
   if (digitalRead(SUMP) == HIGH) {
      if(sumpState==0)
      {
    motoroff();
      sumpState=1;
   }
    }
    if (digitalRead(FLOAT_HIGH) == LOW) {
      if(floatHighState==0)
      {
   motoroff();
   lock=0;
      floatHighState=1;
      }
   }
  
  // Check conditions for controlling the motor based on FLOAT_LOW and SUMP
  if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == HIGH) {
      if(floatLowState==0)
      {
       motoroff();
      floatLowState=1;
      }
  }
    if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == LOW)
     {
       if(floatLowState==0)
      {
        motoron();
      floatLowState=1;
      }
      }
 
  if(sumpState==1)
{
  if(digitalRead(SUMP) ==LOW)
 {
  sumpState=0;
 }  
}
if(floatHighState==1)
{
  if(digitalRead(FLOAT_HIGH) ==HIGH)
 {
  floatHighState=0;
 }  
}
if(floatLowState==1)
{
  if(digitalRead(FLOAT_LOW) ==LOW)
 {
  floatLowState=0;
 }  
}

  if (digitalRead(onbutton) == LOW)
  {
    motoron();
  }
  if (digitalRead(offbutton) == HIGH)
  {
    motoroff();
  }
 }
