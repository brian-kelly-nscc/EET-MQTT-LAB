#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

/* -------- Board Configuration --------
   Student Configurable Parameters:
     - BOARD_NAME: Unique name for this board (used as the MQTT client ID
       and for broadcasting on boot).
     - ip: Static IP address for the board.
           NOTE: For the PLC network, addresses between 192.168.60.170 and
           192.168.60.199 are available to be used.
     - subnet: Subnet mask for your network.
   
   IMPORTANT:
     Each board on the same network must have a unique MAC address.
     Change the 'mac' value below if using multiple boards.
*/

// Customize your board name here
#define BOARD_NAME "PLC-Board-01"

// Static network configuration
// For example, you could assign an IP from the available PLC network addresses:
// 192.168.60.170 to 192.168.60.199
IPAddress ip(192, 168, 1, 177);        // Change as needed for your network
IPAddress subnet(255, 255, 255, 0);    // Network's subnet mask

// Optional: For full network configuration (DNS and Gateway)
// For an unrouted, airgapped network, these values may be set to a dummy address.
IPAddress dns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);

// MQTT Broker IP address (update this to match your VM's broker)
const char* mqtt_server = "192.168.1.131";

// MAC address for the Ethernet shield
// IMPORTANT: Change this if you have multiple boards on the same network.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// ----- MQTT & Ethernet Clients -----
EthernetClient ethClient;
PubSubClient client(ethClient);

// ----- I/O Definitions -----
// Use digital pin 2 for the button press.
// The button is configured with INPUT_PULLUP, so it will read HIGH when not pressed and LOW when pressed.
const int buttonPin = 2;

// LED pin for flashing on message receipt.
// Using pin 7 since pins 11, 12, and 13 are used for SPI communication.
const int ledPin = 7;

// Flash duration (milliseconds) for the LED when a message is received.
const int flashDuration = 100;

// MQTT callback function: called when a subscribed topic receives a message.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  // Flash the LED on pin 7.
  digitalWrite(ledPin, HIGH);
  delay(flashDuration);
  digitalWrite(ledPin, LOW);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for the serial port to connect.
  }
  
  // Configure the LED pin as output.
  pinMode(ledPin, OUTPUT);
  
  // Configure the button pin with the internal pull-up resistor.
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize Ethernet with full configuration
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  delay(1000); // Allow time for the Ethernet hardware to initialize

  // Set the MQTT broker server and port (1883 is standard for unencrypted MQTT)
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); // Set the callback function for incoming messages

  Serial.print("Arduino IP address: ");
  Serial.println(Ethernet.localIP());
}

void reconnect() {
  // Loop until reconnected to the MQTT broker
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Use BOARD_NAME as the MQTT client ID for uniqueness
    if (client.connect(BOARD_NAME)) {
      Serial.println(" connected");

      // Publish board name on boot to notify the broker of this board's presence
      client.publish("board/boot", BOARD_NAME);

      // Subscribe to the sensor topic so that all boards receive published readings.
      client.subscribe("sensor/analog");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" - trying again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  // Ensure we remain connected to the MQTT broker
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check if the button (digital pin 2) is pressed.
  // Because we are using INPUT_PULLUP, a pressed button will result in LOW.
  if (digitalRead(buttonPin) == LOW) {
    // Read the analog voltage on A0 (0 - 1023 corresponding to 0V - 5V)
    int analogValue = analogRead(A0);
    
    // Convert the integer reading to a string (payload)
    char payload[10];
    itoa(analogValue, payload, 10);

    // Publish the analog reading to the topic "sensor/analog"
    if (client.publish("sensor/analog", payload)) {
      Serial.print("Published analog value: ");
      Serial.println(payload);
    } else {
      Serial.println("MQTT publish failed.");
    }
    
    // Simple debounce: wait 500 ms before checking again.
    delay(500);
  }
}