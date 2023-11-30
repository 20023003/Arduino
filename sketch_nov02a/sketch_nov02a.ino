#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "i2s.h"


const char* ssid = "onwords";
const char* password = "nikhil8182";

// Replace with your Azure Cognitive Services API key and region
const String apiKey = "YOUR_AZURE_API_KEY";
const String serviceRegion = "INDIA";

// Define your audio data buffer size
const size_t audioDataSize = 64;  // Adjust the size as needed
int16_t audioData[audioDataSize];    // Buffer to store audio data

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Set up I2S
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_NUM_0);

  delay(500);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  size_t bytesIn = 0;
  esp_err_t result = i2s_read(I2S_NUM_0, audioData, audioDataSize, &bytesIn, portMAX_DELAY);

  if (result == ESP_OK) {
    // Make a POST request to Azure Cognitive Services for speech recognition
    HTTPClient http;
    String endpoint = "https://" + serviceRegion + ".stt.speech.microsoft.com/speech/recognition/conversation/cognitiveservices/v1";
  
    // Set the headers
    http.addHeader("Authorization", "Bearer " + apiKey);
    http.addHeader("Content-Type", "audio/wav"); // Adjust content type as needed
    http.addHeader("Accept", "application/json;text/xml");
    http.addHeader("Host", serviceRegion + ".stt.speech.microsoft.com");

    // Send the audio data to Azure
    int httpResponseCode = http.POST(endpoint, audioData[]);

    if (httpResponseCode == 200) {
      String response = http.getString();
      // Process the JSON response from Azure Cognitive Services
      // The response will contain the recognized text
      Serial.println("Recognized text: " + response);
    } else {
      Serial.println("HTTP request failed");
    }

    http.end();
  }
}
