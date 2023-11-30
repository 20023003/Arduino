// Include the required library
#include <Arduino.h>

// Define the GPIO pin
const int ledPin = 4;

void setup() {
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Turn the LED on
  digitalWrite(ledPin, HIGH);

  // Wait for 1 second
  delay(1000);

  // Turn the LED off
  digitalWrite(ledPin, LOW);

  // Wait for another 1 second
  delay(1000);
}
