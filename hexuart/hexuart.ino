

void setup() {
 // Set the button pin as an input with a pull-up resistor
  Serial.begin(9600); // Change the baud rate to match your setup
}

void loop() {
  // Check if the button is pressed
 
    // Button is pressed, send the value 0xAA over the serial port
    Serial.write(0xAA);
 
}
