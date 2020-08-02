// includes
#include <RunningMedian.h>
#include "EspMQTTClient.h"

// consts

#include "secrets_wifi.h"

const int trig = 4;
const int echo = 5;

const float soundspeed = 0.034; // cm/us;
float duration;
float distance;

String basestring = "schuur,type=water value=";

// mqtt setup
EspMQTTClient client(
  ssid,
  password,
  "autopi",  // MQTT Broker server ip
  "esp_schuur_1",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);


RunningMedian samples = RunningMedian(5);


void setup() {
  // put your setup code here, to run once:
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  client.enableDebuggingMessages();
}


void onConnectionEstablished()
{
}


void loop() {

//  keep mqtt running
  client.loop();

//  measure distance
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  duration = pulseIn(echo, HIGH);
  distance = duration * soundspeed / 2;
  samples.add(distance);

//  report to mqtt topic
  String sendstring = basestring + samples.getMedian();
  client.publish("sensors",sendstring);
  delay(5000);
}
