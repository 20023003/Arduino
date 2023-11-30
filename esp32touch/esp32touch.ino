bool device1=0;
bool device2=0;
bool device3=0;
int spd=0;
void setup() {
 Serial.begin(9600);

}

void loop() {
 if(Serial.available>0)
 {
  String data=Serial.read();
  if(data == "L1")
  {
    device1=!device1;
  
  }
   if(data == "L2")
  {
    device2=!device2;
  
  }
   if(data == "L3")
  {
    device3=!device3;
  
  }
   if(data == "F1")
  {
    device4=!device4;
   
  }
   if(data == "S0")
  {
    spd=0;
   
  }
   if(data == "S1")
  {
    spd=1;
     
     
  }
   if(data == "S2")
  {
    spd=2;
     
  }
   if(data == "S3")
  {
    spd=3;
     
  }
   if(data == "S4")
  {
    spd=4;
    
  }
   if(data == "S5")
  {
    spd=5;
     
  }
 }

}
