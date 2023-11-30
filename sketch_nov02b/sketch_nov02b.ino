

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <driver/i2s.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Replace with your Azure Cognitive Services API key and region
const String apiKey = "YOUR_AZURE_API_KEY";
const String serviceRegion = "YOUR_AZURE_SERVICE_REGION";

// Define input buffer length for audio data
#define bufferLen 64
int16_t sBuffer[bufferLen];

void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = I2S_MODE_MASTER | I2S_MODE_RX,
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = bufferLen,
    .use_apll = false
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = 32, // Replace with your pin numbers
    .ws_io_num = 25,
    .data_out_num = -1,
    .data_in_num = 33
  };

  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // Set up I2S
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_NUM_0);

  delay(500);
}

void loop() {
  int rangelimit = 10000;
  Serial.print(rangelimit * -1);
  Serial.print(" ");
  Serial.print(rangelimit);
  Serial.print(" ");

  // Get I2S data and place in data buffer
  size_t bytesIn = 0;
  esp_err_t result = i2s_read(I2S_NUM_0, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);

  if (result == ESP_OK) {
    // Read I2S data buffer
    int16_t samples_read = bytesIn / 8;
    if (samples_read > 0) {
      // Here you can process the audio data as needed before sending it to Azure
      // For example, you might convert it to the appropriate format (e.g., WAV) and send it to Azure
      // You will also need to handle data synchronization and real-time constraints

      // Once you've prepared the audio data, you can make an HTTP request to Azure Cognitive Services
      HTTPClient http;
      String endpoint = "https://" + serviceRegion + ".stt.speech.microsoft.com/speech/recognition/conversation/cognitiveservices/v1";
      http.addHeader("Authorization", "Bearer " + apiKey);
      http.addHeader("Content-Type", "audio/wav"); // Adjust content type as needed
      http.addHeader("Accept", "application/json;text/xml");
      http.addHeader("Host", serviceRegion + ".stt.speech.microsoft.com");

      // Send the audio data to Azure
      int httpResponseCode = http.POST(endpoint, audioData);

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
}


void loop() {
  // put your main code here, to run repeatedly:

}
