/*
 * Made by Gaganveer Singh 2210994783
*/
#include <WiFiNINA.h>

// WiFi network details
char ssid[] = "Diamond";
char pass[] = "diamond@580";

// Create an instance of the WiFi client
WiFiClient client;

// IFTTT Webhooks details
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/light_intensity/with/key/fgWXJE3fbyndV6YvZKVywMM_YhRImHk3y5uFJFKFDV";

void setup() {
  // Start WiFi connection using the provided SSID and password
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
  while (!Serial)
    ;

  // Check if the WiFi connection was successful and print the result to the Serial Monitor
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Connection to WiFi failed");
  }
}

void loop() {
  // Read the light intensity from the analog pin A0
  int lightIntensity = analogRead(A0);
  // Map the light intensity to a percentage (0-100)
  int scaledintensity = map(lightIntensity, 0, 1023, 0, 100);

  // Based on the mapped intensity, send an appropriate message to IFTTT
  if (scaledintensity < 20) {
    sendLightIntensityToIFTTT("Plant_is_in_very_Low_Light ", scaledintensity);
  } else if (scaledintensity >= 20 && scaledintensity < 50) {
    sendLightIntensityToIFTTT("Plant_is_in_Low_Light ", scaledintensity);
  } else if (scaledintensity >= 50 && scaledintensity < 80) {
    sendLightIntensityToIFTTT("Plant_is_in_Medium_Light ", scaledintensity);
  } else if (scaledintensity >= 80) {
    sendLightIntensityToIFTTT("Plant_is_in_High_Light ", scaledintensity);
  }

  delay(10000);  // Delay for a few seconds before taking the next reading
}

// Function to send the light intensity information to IFTTT
void sendLightIntensityToIFTTT(String message, int intensity) {
  // Construct the query string for the GET request
  String queryString = "?value1=" + message + "&value2=" + String(intensity) + "percentage";

  // Attempt to connect to IFTTT Webhooks
  if (client.connect(HOST_NAME, 80)) {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
    // Print the message and intensity to the Serial Monitor for debugging
    Serial.println(message + "Intensity: " + String(intensity) + " Percentage");
  } else {
    // Print an error message if the connection to IFTTT failed
    Serial.println("Connection to server failed");
  }
}
