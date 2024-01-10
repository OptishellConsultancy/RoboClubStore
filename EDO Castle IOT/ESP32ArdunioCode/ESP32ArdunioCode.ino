#include <Arduino.h>
#include <ESP8266WiFi.h> //https://github.com/tzapu/WiFiManager
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
// https://io.adafruit.com/JShelley/dashboards/edolightcontrol
#include "settings.h"
// WIFI//////////////////////
#define WIFI_SSID "8Glaives0"
#define WIFI_PASS "Homeworld!"
// MQTT//////////////////////
//#define MQTT_SERV "io.adafruit.com"
//#define MQTT_PORT 1883
//#define MQTT_NAME "<your username from in a settings.h file>"
//#define MQTT_PASS "<your oi key from in a settings.h file>" // Adafruit IO Key is a string of characters that can be found by pressing the gold key button on your dashboard

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe feed_EdoCastle = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/EdoCastle");
//-------

int LED_0_Pin = 4;  // Yellow
int LED_1_Pin = 5;  // Pink
int LED_2_Pin = 2;  // blue
int LED_3_Pin = 16; // green
//
int LEDF_0_Pin = 0;  // Yellow
int LEDF_1_Pin = 15; // Yellow
int LEDF_2_Pin = 13; // Yellow
int LEDF_3_Pin = 12; // Yellow

int status = 1;

void setup()
{

  Serial.begin(9600);
  pinMode(LED_0_Pin, OUTPUT);
  pinMode(LED_1_Pin, OUTPUT);
  pinMode(LED_2_Pin, OUTPUT);
  pinMode(LED_3_Pin, OUTPUT);
  pinMode(LEDF_0_Pin, OUTPUT);
  pinMode(LEDF_1_Pin, OUTPUT);
  pinMode(LEDF_2_Pin, OUTPUT);
  pinMode(LEDF_3_Pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // test
  digitalWrite(LED_2_Pin, HIGH);

  Serial.write("Setting up wifi and MQTT..");
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  // Subscribe to the feed_EdoCastle topic
  mqtt.subscribe(&feed_EdoCastle);

  Serial.write("DONE");
}

void MQTT_connect()
{
  int8_t ret;
  // Stop if already connected
  if (mqtt.connected())
  {
    Serial.println("MQTT Connected");
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}

void maintainMQTTConnection()
{
  // Connect/Reconnect to MQTT
  MQTT_connect();

  // Read from our subscription queue until we run out, or
  // wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    // If we're in here, a subscription updated...
    if (subscription == &feed_EdoCastle)
    {
      // Print the new value to the serial monitor
      Serial.print("feed_EdoCastle: ");
      Serial.println((char *)feed_EdoCastle.lastread);

      // If the new value is  "ON", turn the light on.
      // Otherwise, turn it off.
      if (!strcmp((char *)feed_EdoCastle.lastread, "ON"))
      {        
         turnOnRoutine();
      }
      else
      {
       turnOffRoutine();
      }
    }
  }
}

void turnOffRoutine()
{
  digitalWrite(LED_0_Pin, LOW);
  digitalWrite(LED_1_Pin, LOW);
  digitalWrite(LED_2_Pin, LOW);
  digitalWrite(LED_3_Pin, LOW);
  digitalWrite(LEDF_0_Pin, LOW);
  digitalWrite(LEDF_1_Pin, LOW);
  digitalWrite(LEDF_2_Pin, LOW);
  digitalWrite(LEDF_3_Pin, LOW);
}

void turnOnRoutine()
{
  digitalWrite(LED_0_Pin, HIGH);
  digitalWrite(LED_1_Pin, HIGH);
  digitalWrite(LED_2_Pin, HIGH);
  digitalWrite(LED_3_Pin, HIGH);
  digitalWrite(LEDF_0_Pin, HIGH);
  digitalWrite(LEDF_1_Pin, HIGH);
  digitalWrite(LEDF_2_Pin, HIGH);
  digitalWrite(LEDF_3_Pin, HIGH);
}

void brightenLED(int ledId, int dtime = 30)
{
  for (int a = 0; a < 255; a = a + 10)
  {
    analogWrite(ledId, a);
    delay(dtime);
  }
}

void dimLED(int ledId, int dtime = 30)
{
  for (int a = 255; a >= 0; a = a - 10)
  {
    analogWrite(ledId, a);
    delay(dtime);
  }
}

void loop()
{
  maintainMQTTConnection();
}
