#include <WiFi.h>

const char *ssid = "maxspier.com";
const char *password = "maxIsGreat$4";

const int relayPin = 2; // Pin connected to the relay or light
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Set up the ESP32 as an Access Point
  WiFi.softAP(ssid, password);

  // Set up the relay pin as an output
  pinMode(relayPin, OUTPUT);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(myIP);

  // Start server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");

    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);

        if (request.indexOf("/on") != -1) {
          digitalWrite(relayPin, HIGH); // Turn on the light
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/plain");
          client.println();
          client.println("Request processed. Light successfully turned on.");
        } else if (request.indexOf("/off") != -1) {
          digitalWrite(relayPin, LOW); // Turn off the light
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/plain");
          client.println();
          client.println("Request processed. Light successfully turned off.");
        }else if(request.indexOf("/") != -1){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/plain");
          client.println();
          client.println("Please go to either /on or /off to turn the light either on or off!");

          client.println();

            // the content of the HTTP response follows the header:
          client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
          client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
          client.println();
        } else {
          client.println("HTTP/1.1 404 Not Found");
          client.println("Content-type:text/plain");
          client.println();
          client.println("The server cannot find the requested resource. The URL is not recognized.");

        }

        // Respond to the client

        // Break out of the loop after handling one request
        break;
      }
    }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}
