//Md mahbub ali


#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "sifat"
#define WLAN_PASS       ""

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "mahbub"
#define AIO_KEY         ""

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish aIrquality = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Air quality");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Photocell");
Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/
#include <Wire.h>


#include<dht.h>
dht DHT;
#define DHT11_PIN D1
//int airquality = 0;

int ldrValue = 0;
int ldr = A0;
int sensorValue= 0;
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
char data[32];

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;

void loop() {


   hum();
//  gas();
 density();

 if(s.available()){
 int x = s.read();
 Serial.println("AIR Quality");
 Serial.println(x);

         
  }
 delay(500);
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(ldrValue);
  Serial.print("...");
  
  if (! photocell.publish(ldrValue)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  
  Serial.print(F("\nSending Airquality val "));
  Serial.print(x);
  Serial.print("...");

  
  if (! aIrquality.publish(x)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  
  Serial.print(F("\nSending Humidity val "));
  Serial.print(DHT.humidity);
  Serial.print("...");
  if (! humidity.publish(DHT.humidity)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  
  Serial.print(F("\nSending Temperature value "));
  Serial.print(DHT.temperature);
  Serial.print("...");
  if (! temp.publish(DHT.temperature)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void hum()
{
  int chk = DHT.read11(DHT11_PIN);

  Serial.println(" Humidity " );

  Serial.println(DHT.humidity, 1);

  Serial.println(" Temparature ");

  Serial.println(DHT.temperature, 1);

  delay(500);



  delay(500);

}

//void gas()
//{
//  int sensorValue1 = analogRead(airpin);
//
//
//  Serial.print("Air Quality = ");
//   sensorValue = map(sensorValue, 0, 1023, 0, 100);
//  Serial.print(sensorValue);
//
//  Serial.print("*PPM");
//
//  Serial.println();
//
//
//
//  delay(500);
//
//
//
//}




void density()
{
    int ldrValue1 = analogRead(ldr);
    ldrValue = map(ldrValue1,0,1023,0,100);
Serial.println("ldrValue");
  Serial.println(ldrValue);


  delay(250);
  


}
