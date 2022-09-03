/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 0          // D3

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Variables to hold sensor readings
float temp;
float hum;
float temp2;
float hum2;

// Update these with values suitable for your network.

const char* ssid = "copur";
const char* password = "aisyah79";
const char* mqtt_server = "broker.mqtt-dashboard.com";
unsigned long previousMillis = 0;  
const long interval = 10000;    
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
char msg2[MSG_BUFFER_SIZE];
char msgtemp1[MSG_BUFFER_SIZE];
char msghum1[MSG_BUFFER_SIZE];
char msgtemp2[MSG_BUFFER_SIZE];
char msghum2[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(14, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(14, LOW);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("TANKUN2", "hello world");
      // ... and resubscribe
    //NODE1
     client.subscribe("TANKUN1");
     //NODE2
      //client.subscribe("TANKUN2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
   pinMode(14, OUTPUT);
  Serial.begin(115200);
    dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
//    //Node 1
//    snprintf (msg, MSG_BUFFER_SIZE, "Tes broker TANKUN2 #%ld", value);
//     snprintf (msgtemp1, MSG_BUFFER_SIZE, "Temperature %0.2f C", temp);
//      snprintf (msghum1, MSG_BUFFER_SIZE, "Humidity %0.2f ", hum);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    client.publish("TANKUN3", msg);
//          Serial.print("Publish message: ");
//    Serial.println(msgtemp1);
//        client.publish("TANKUN3", msgtemp1);
//        Serial.print("Publish message: ");
//    Serial.println(msghum1);
//    client.publish("TANKUN3", msghum1);

//    //Node 2
    snprintf (msg2, MSG_BUFFER_SIZE, "Tes broker TANKUN2 #%ld", value);
     snprintf (msgtemp2, MSG_BUFFER_SIZE, "Temperature %0.2f C", temp2);
      snprintf (msghum2, MSG_BUFFER_SIZE, "Humidity %0.2f ", hum2);
    Serial.print("Publish message: ");
    Serial.println(msg2);
    client.publish("TANKUN2", msg);
          Serial.print("Publish message: ");
    Serial.println(msgtemp2);
        client.publish("TANKUN2", msgtemp2);
        Serial.print("Publish message: ");
    Serial.println(msghum2);
    client.publish("TANKUN2", msghum2);


  }
    unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    // New DHT sensor readings
    hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp = dht.readTemperature();

    hum2 = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp2 = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //temp = dht.readTemperature(true);
    
    // Publish an MQTT message on topic esp/dht/temperature
                

    Serial.printf("Message: %.2f \n", temp2);

    // Publish an MQTT message on topic esp/dht/humidity
                        

    Serial.printf("Message: %.2f \n", hum2);
//   setting NODE1
//     snprintf (msgtemp1, MSG_BUFFER_SIZE, "Temperature %0.2f C", temp);
//      snprintf (msghum1, MSG_BUFFER_SIZE, "Humidity %0.2f ", hum);
//    client.publish("NODET", msgtemp1);
//    client.publish("NODEH", msghum1);

//    //Setting Node 2
         snprintf (msgtemp2, MSG_BUFFER_SIZE, "Temperature %0.2f C", temp2);
      snprintf (msghum2, MSG_BUFFER_SIZE, "Humidity %0.2f ", hum2);
      client.publish("NODET2", msgtemp2);
    client.publish("NODEH2", msghum2);
  }
  
}
