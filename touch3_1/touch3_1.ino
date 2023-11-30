#include <SPI.h>
#define fan_relay1 45
#define fan_relay2 46
#define fan_relay3 47
#define fan_relay4 48
#define Light1 49 // Define the LED pin
#define Light2 50
#define Light3 51
#define T1led 25
#define T2led 23
#define T3led 22
#define fled 24
#define s1led 32
#define s2led 31
#define s3led 30
#define s4led 29
#define s5led 28
#define af 45
#define af1 46
#define af2 47
#define af3 48
int ledstate1=0,ledstate2=0,ledstate3=0;
int f = 0, s = 0, led1 = 0, led2 = 0, led3 = 0;
char receivedData[20];
const int slaveSelectPin = 8;
#define backLight 35
#define buzzer 34
String data = "";
void setup() {
  SPI.begin();
  pinMode(slaveSelectPin, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(Light1, OUTPUT);
  pinMode(Light2, OUTPUT);
  pinMode(Light3, OUTPUT);
  pinMode(T1led, OUTPUT);
  pinMode(T2led, OUTPUT);
  pinMode(T3led, OUTPUT);
  pinMode(backLight, OUTPUT);// Set the LED pin as an output
  digitalWrite(Light1, LOW);
  digitalWrite(Light2, LOW);
  digitalWrite(Light3, LOW);
  digitalWrite(T1led, LOW);
  digitalWrite(T2led, LOW);
  digitalWrite(T3led, LOW);
}

void loop()
{

 if (digitalRead(SS) == LOW) {
   char receivedChar = SPI.transfer(0); 
    Serial.print(receivedChar); 
  }
  

  if (strcmp(receivedData, "L1") == 0) {
    ledstate1 = digitalRead(Light1);
    ledstate1 = !ledstate1;
    digitalWrite(Light1, ledstate1);
    digitalWrite(T1led, ledstate1);
  }

  if (strcmp(receivedData, "L2") == 0)
  {
    ledstate2 = digitalRead(Light2);
    ledstate2 = !ledstate2;
    digitalWrite(Light2, ledstate2);
    digitalWrite(T2led, ledstate2);
  }
  if (strcmp(receivedData, "L3") == 0)
  {
    ledstate3 = digitalRead(Light3);
    ledstate3 = !ledstate3;
    digitalWrite(Light3, ledstate3);
    digitalWrite(T3led, ledstate3);
  }


  if (strcmp(receivedData, "F1") == 0)
  {
    f++;
    fanOnLogic();
  }
  if (f % 2 == 0)
  {
    digitalWrite(fled, LOW);

    Serial.print("L40");
    fanOnLogic();
  }
  else
  {
    digitalWrite(fled, HIGH);

    Serial.print("L41");
    fanOnLogic();
  }

  if (data == "b1")
  {
    digitalWrite(backLight, HIGH);
    Serial.print("b1");
  }
  if (data == "b0")
  {
    digitalWrite(backLight, LOW);
    Serial.print("b0");
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
  digitalWrite(s4led, LOW);
  digitalWrite(s3led, LOW);
  digitalWrite(s2led, LOW);
  digitalWrite(s1led, LOW);

  digitalWrite(s5led, LOW);

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
  digitalWrite(s1led, HIGH);
  digitalWrite(s4led, LOW);
  digitalWrite(s3led, LOW);
  digitalWrite(s2led, LOW);


  digitalWrite(s5led, LOW);
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
  digitalWrite(s2led, HIGH);
  digitalWrite(s4led, LOW);
  digitalWrite(s3led, LOW);
  digitalWrite(s2led, LOW);
  digitalWrite(s1led, HIGH);

  digitalWrite(s5led, LOW);
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
  digitalWrite(s3led, HIGH);
  digitalWrite(s4led, LOW);

  digitalWrite(s2led, HIGH);
  digitalWrite(s1led, HIGH);

  digitalWrite(s5led, LOW);
}

void as4()
{
  digitalWrite(af, LOW);

  digitalWrite(af1, HIGH);
  delay(100);
  digitalWrite(af2, HIGH);
  delay(100);
  digitalWrite(af3, LOW);
  delay(100);
  digitalWrite(s4led, HIGH);
  digitalWrite(s3led, HIGH);
  digitalWrite(s2led, HIGH);
  digitalWrite(s1led, HIGH);
  digitalWrite(s5led, LOW);
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
  digitalWrite(s5led, HIGH);
  digitalWrite(s4led, HIGH);
  digitalWrite(s3led, HIGH);
  digitalWrite(s2led, HIGH);
  digitalWrite(s1led, HIGH);



}
void fanOnLogic()
{
  if (strcmp(receivedData, "H") == 0)
  {
    s++;
  }
  if (data == "L")
  {
    s--;
  }
  if (s == 0)
  {
    as0();
    Serial.print("spd0");
  }
  if (s == 1)
  {
    as1();
    Serial.print("spd1");
  }
  if (s == 2)
  {
    as2();
    Serial.print("spd2");
  }
  if (s == 3)
  {
    as3();
    Serial.print("spd3");
  }
  if (s == 4)
  {
    as4();
    Serial.print("spd4");
  }
  if (s == 5)
  {
    as5();
    Serial.print("spd5");
  }
}
