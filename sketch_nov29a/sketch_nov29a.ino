

#include <NewPing.h>

#define TRIGGER_PIN  14 // Arduino pin connected to the trigger pin on the ultrasonic sensor.
#define ECHO_PIN     27 // Arduino pin connected to the echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance (in centimeters) to ping.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open a serial connection for debugging.
}

void loop() {
  delay(50);                      // Wait 50ms between pings.
  unsigned int distance = sonar.ping_cm();
  
  if (distance == 0) {
    Serial.println("No obstacle");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}
