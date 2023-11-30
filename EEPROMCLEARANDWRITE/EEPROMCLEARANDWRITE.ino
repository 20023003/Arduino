#include <EEPROM.h>

#define EEPROM_SIZE 512
#define STRING1_ADDR 0
#define STRING2_ADDR 30
#define STRING3_ADDR 60
#define RESULT_ADDR 100

void setup()
{
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);

  Serial.println("To write data, send 'write:string1:string2:string3' for SSID:password:additionalString or 'write:string1:string2' for SSID:password");
  Serial.println("To read result, send 'read'");
  Serial.println("To clear EEPROM, send 'clear'");
}

void loop() {
  if (Serial.available()) {
    String str = Serial.readStringUntil('\n');
    str.trim(); // Remove leading/trailing whitespace

    if (str.startsWith("write:")) {
      str.remove(0, 6); // Remove "write:" from the input string

      // Split the input into parts based on ':'
      String parts[3];
      int separatorIndex;
      for (int i = 0; i < 3; i++) {
        separatorIndex = str.indexOf(':');
        if (separatorIndex != -1) {
          parts[i] = str.substring(0, separatorIndex);
          str.remove(0, separatorIndex + 1);
        } else {
          parts[i] = str; // Last part (if fewer than 3) or empty if more than 3
          break;
        }
      }

      // Check if it's a valid format (SSID:password:additionalString)
      if (parts[0].length() > 0 && parts[1].length() > 0) {
        writeStrings(parts[0], parts[1], parts[2]);
      } else {
        Serial.println("Invalid input format. Please provide valid input.");
      }
    } else if (str == "read") {
      readResult();
    } else if (str == "clear") {
      clearEEPROM();
    }
  }
}

void writeStrings(String ssid, String password, String additionalString) {
  if ((STRING3_ADDR + ssid.length() + password.length() + additionalString.length() <= EEPROM_SIZE)) {
    for (int i = 0; i < ssid.length(); i++) {
      EEPROM.write(STRING1_ADDR + i, ssid[i]);
    }
    for (int i = 0; i < password.length(); i++) {
      EEPROM.write(STRING2_ADDR + i, password[i]);
    }
    for (int i = 0; i < additionalString.length(); i++) {
      EEPROM.write(STRING3_ADDR + i, additionalString[i]);
    }
    EEPROM.commit();
    Serial.println("Data written to EEPROM");
  } else {
    Serial.println("Data length exceeds available EEPROM size.");
  }
}

void readResult() {
  String ssid, password, additionalString;

  for (int i = STRING1_ADDR; i < STRING2_ADDR; i++) {
    char character = EEPROM.read(i);
    if (character == 0) {
      break; // Null terminator, end of string
    }
    ssid += character;
  }

  for (int i = STRING2_ADDR; i < STRING3_ADDR; i++) {
    char character = EEPROM.read(i);
    if (character == 0) {
      break; // Null terminator, end of string
    }
    password += character;
  }

  for (int i = STRING3_ADDR; i < RESULT_ADDR; i++) {
    char character = EEPROM.read(i);
    if (character == 0) {
      break; // Null terminator, end of string
    }
    additionalString += character;
  }

  Serial.println("SSID: " + ssid);
  Serial.println("Password: " + password);
  Serial.println("Additional String: " + additionalString);
}

void clearEEPROM() {
  for (int i = 0; i < 60read; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();

  Serial.println("EEPROM cleared");
}
