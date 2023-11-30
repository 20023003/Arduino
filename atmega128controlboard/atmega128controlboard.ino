
#include <SPI.h>
#define Light1 13//49 // Define the LED pin
#define Light2 50
#define Light3 51
#define Light4 52
#define T1led 25
#define T2led 23
#define T3led 22
#define T4led 25
#define af 0
#define af1 1 
#define af2 2 
#define af3 3 
const int slaveSelectPin = 10;
#define backLight 35
String data="";
void setup() {
  SPI.begin();
    SPI.setDataMode(SPI_MODE0);
     pinMode(slaveSelectPin, INPUT);  
  Serial.begin(9600);
  pinMode(Light1, OUTPUT);
  pinMode(Light2, OUTPUT);
  pinMode(Light3, OUTPUT);
  pinMode(T1led, OUTPUT);
  pinMode(T2led, OUTPUT);
  pinMode(T3led, OUTPUT);
  pinMode(backLight, OUTPUT);// Set the LED pin as an output
}

void loop() {
  
  if(Serial.available()>0)
  {
   data= Serial.readStringUntil('\n');
   //Serial.print("string received from UART:");
   Serial.println(data);
  }
  if(data =="d11")
  {
    digitalWrite(Light1,HIGH);
    digitalWrite(T1led,HIGH);
    Serial.print("L11");
  }
   if(data =="L21")
  {
    digitalWrite(Light2,HIGH);
    digitalWrite(T2led,HIGH);
    Serial.print("L21");
  }
   if(data =="L31")
  {
    digitalWrite(Light3,HIGH);
    digitalWrite(T3led,HIGH);
    Serial.print("L31");
  }
   if(data =="L10")
  {
    digitalWrite(Light1,LOW);
    digitalWrite(T1led,LOW);
    Serial.print("L10");
  }
   if(data =="L20")
  {
    digitalWrite(Light2,LOW);
     digitalWrite(T2led,LOW);
     Serial.print("L20");
  }
   if(data =="L30")
  {
    digitalWrite(Light3,LOW);
     digitalWrite(T3led,LOW);
     Serial.print("L30");
  }
   if(data =="L40")
  {
    digitalWrite(Light4,LOW);
     digitalWrite(T4led,LOW);
     Serial.print("L40");
     delay(500);
      Serial.print("spd0");
  }
   if(data =="L41")
  {
    digitalWrite(Light4,HIGH);
     digitalWrite(T4led,HIGH);
     Serial.print("L41");
     fanOnLogic();
  }
  if(data =="b1")
  {
    digitalWrite(backLight,HIGH);
   Serial.print("b1");
  }
   if(data =="b0")
  {
    digitalWrite(backLight,LOW);
   Serial.print("b0");
  }
   
}
void spi()
{
if (digitalRead(slaveSelectPin) == LOW) {
    char receivedChar = SPI.transfer(0);  // Receive data from the master
    
    if (receivedChar == '\n') {
     
      Serial.println(data);
      data = "";  // Reset the string for the next data
    } else {
      data += receivedChar;  // Append the character to the string
    }
  }
}
void as0()
{
  digitalWrite(af, LOW);
  delay(100);
  digitalWrite(af1, LOW);
  delay(100);
  digitalWrite(af2, LOW);
  delay(100);
  digitalWrite(af3, LOW);
  delay(100);
}
void as1()
{
  digitalWrite(af, LOW);
  delay(100);
  digitalWrite(af1, LOW);
  delay(100);
  digitalWrite(af2, HIGH);
  delay(100);
  digitalWrite(af3, LOW);
  delay(100);
}

void as2()
{
  digitalWrite(af, LOW);
  delay(100);
  digitalWrite(af1, HIGH);
  delay(100);
  digitalWrite(af2, LOW);
  delay(100);
  digitalWrite(af3, LOW);
  delay(100);
}


void as3()
{
  digitalWrite(af, HIGH);
  delay(100);
  digitalWrite(af1, LOW);
  delay(100);
  digitalWrite(af2, LOW);
  delay(100);
  digitalWrite(af3, LOW);
  delay(100);

}

void as4()
{
  digitalWrite(af, LOW);
  delay(100);
  digitalWrite(af1, HIGH);
  delay(100);
  digitalWrite(af2, HIGH);
  delay(100);
  digitalWrite(af3, LOW);
  delay(100);

}
void as5()
{
  digitalWrite(af, LOW);
  delay(100);
  digitalWrite(af1, LOW);
  delay(100);
  digitalWrite(af2, LOW);
  delay(100);
  digitalWrite(af3, HIGH);
  delay(100);
}
void fanOnLogic()
{
  if(data =="S0")
  {
    as0();
   Serial.print("spd0");
  }
    if(data =="S1")
  {
    as1();
   Serial.print("spd1");
  }
    if(data =="S2")
  {
    as2();
   Serial.print("spd2");
  }
    if(data =="S3")
  {
    as3();
   Serial.print("spd3");
  }
    if(data =="S4")
  {
    as4();
   Serial.print("spd4");
  }
    if(data =="S5")
  {
    as5();
   Serial.print("spd5");
  } 
}
