#include <SPI.h>

const int slaveSelectPin = 10;  // Must match the slave select pin of the receiver
char dataToSend = 'A';  // Data to be sent

void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  pinMode(slaveSelectPin, OUTPUT);  // Set the SS pin as an output (master mode)
}

void loop() {
  digitalWrite(slaveSelectPin, LOW);  // Enable communication with the slave
  SPI.transfer(dataToSend);           // Send data to the receiver
  digitalWrite(slaveSelectPin, HIGH); // Disable communication with the slave

  Serial.print("Sent: ");
  Serial.println(dataToSend);

  delay(1000);  // Delay for 1 second before sending the next data
  dataToSend++;  // Increment the data for the next transmission
}
