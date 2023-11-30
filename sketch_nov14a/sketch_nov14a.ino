#include <SPI.h>
#define SS 8
int led1 = 25;
int led2 = 23;
int led3 = 25;
int led4 = 22; 
int backled = 34;
int ledstate1;
int ledstate2;
int ledstate3;
int ledstate4;
int relaystate1;
int relaystate2;
int relaystate3;
int relaystate4;

int relay1 = 48;
int relay2 = 49;
int relay3 = 50;
int relay4 = 51;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(SS, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(backled, OUTPUT);
  digitalWrite(backled,HIGH);
}

void loop() {
  if (digitalRead(SS) == LOW) { // Check if slave is selected
    char receivedData[20]; // Use a character array to store the received data
    int index = 0;
    while (true) {
      char receivedChar = SPI.transfer(0); // Receive a character from the master
      if (receivedChar == '\n') { // Check for null character indicating end of string
        receivedData[index] = '\0'; // Null-terminate the received data
        break;
      }
      receivedData[index++] = receivedChar; // Store the character in the array
    }
    Serial.println(receivedData);
    if (strcmp(receivedData, "L1") == 0) {
      ledstate1 = digitalRead(led1);
      ledstate1 = !ledstate1;
      digitalWrite(led1,ledstate1);
      digitalWrite(relay1,ledstate1);
    }
    if (strcmp(receivedData, "L2") == 0) {
     ledstate2 = digitalRead(led2);
      ledstate2 = !ledstate2;
      digitalWrite(led2,ledstate2);
      digitalWrite(relay2,ledstate2);
    }
    if (strcmp(receivedData, "L3") == 0) {
      ledstate3 = digitalRead(led3);
      ledstate3 = !ledstate3;
      digitalWrite(led3,ledstate3);
       digitalWrite(relay3,ledstate3);
    }
    if (strcmp(receivedData, "L4") == 0) {
      ledstate4 = digitalRead(led4);
      ledstate4 = !ledstate4;
      digitalWrite(led4,ledstate4);
      digitalWrite(relay4,ledstate4);
    }
  }
}
