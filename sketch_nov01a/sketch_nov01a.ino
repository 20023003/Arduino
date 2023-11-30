int rec = 2;
int play = 3;
int sensor = 4;
int led = 13;

void setup() {
  Serial.begin(9600);
  pinMode(rec, OUTPUT);
  pinMode(play, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);

  digitalWrite(rec, LOW);
  digitalWrite(play, LOW);
  digitalWrite(led, LOW);
}

void loop() {
  if (digitalRead(sensor) == LOW) {
    digitalWrite(led, HIGH);
    digitalWrite(rec, HIGH);
    delay(5000);
    digitalWrite(led, LOW);
    digitalWrite(rec, LOW);
    delay(1000);
    digitalWrite(play, HIGH);
    int analogValue = analogRead(A0); // Read analog value from A0
    String binaryValue = intToBinary(analogValue, 10); // Convert to binary with a specified number of bits
    Serial.println(binaryValue); // Print the binary representation
    delay(6000);
    digitalWrite(play, LOW);

    while (digitalRead(sensor) == LOW)
      ;
  }
}

String intToBinary(int value, int numBits) {
  String binaryString = "";
  for (int i = numBits - 1; i >= 0; i--) {
    int bit = (value >> i) & 1;
    binaryString += String(bit);
  }
  return binaryString;
}
