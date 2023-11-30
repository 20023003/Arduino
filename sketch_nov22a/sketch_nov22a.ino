void setup() {
  Serial.begin(9600);
}

void loop() {
  // Send 0xAA (170 in decimal) over the serial port
  Serial.write(0xAA);
   Serial.write(0xBB);
 delay(200); // Add a delay to control the transmission rate
}
