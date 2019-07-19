// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "EmonLib.h"                   // Include Emon Library
int voltage = 127;

EnergyMonitor emon0;                   // Create an instance
EnergyMonitor emon1;                   // Create an instance
EnergyMonitor emon2;                   // Create an instance
EnergyMonitor emon3;                   // Create an instance
EnergyMonitor emon4;                   // Create an instance

EthernetClient ethClient;
PubSubClient client(ethClient);

float getApparentPowerA0(void);
float getApparentPowerA1(void);
float getApparentPowerA2(void);
float getApparentPowerA3(void);
float getApparentPowerA4(void);
void mqttEmit(String topic, String value);
void callback(char* topic, byte* payload, unsigned int length);
void reconnect(void);


void setup()
{  
  // Update these with values suitable for your network.
  IPAddress server(172,31,141,156);
  byte mac[]    = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
  Serial.begin(9600);
  emon0.current(0, 6.0606);             // Current: input pin, calibration.
  emon1.current(1, 6.0606);             // Current: input pin, calibration.
  emon2.current(2, 6.0606);             // Current: input pin, calibration.
  emon3.current(3, 6.0606);             // Current: input pin, calibration.
  emon4.current(4, 6.0606);             // Current: input pin, calibration.
  client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.begin(mac);
  // Allow the hardware to sort itself out
    // print your local IP address:
  Serial.println(Ethernet.localIP());
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  else {
    Serial.println("sending data to mqtt");
      mqttEmit("sensors/arduino/energy/A0",(String)getApparentPowerA0());
      mqttEmit("sensors/arduino/energy/A1",(String)getApparentPowerA1());
      mqttEmit("sensors/arduino/energy/A2",(String)getApparentPowerA2());
      mqttEmit("sensors/arduino/energy/A3",(String)getApparentPowerA3());
      mqttEmit("sensors/arduino/energy/A4",(String)getApparentPowerA4());
      delay(1500);
      mqttEmit("sensors/arduino/energy/total",(String)(getApparentPowerA4() +getApparentPowerA3() +getApparentPowerA2() +getApparentPowerA1() +getApparentPowerA0()));
  }
  client.loop();

}

float getApparentPowerA0() {
  double IrmsA0 = emon0.calcIrms(1480);  // Calculate Irms only
  Serial.print ("Current in A0 ->");
  Serial.print(IrmsA0*voltage);         // Apparent power
  Serial.print(" ");
  Serial.println(IrmsA0);          // Irms
  return (IrmsA0*voltage);
}
float getApparentPowerA1() {
  double IrmsA1 = emon1.calcIrms(1480);  // Calculate Irms only
  Serial.print ("Apparent power in A1 ->");
  Serial.print(IrmsA1*voltage);         // Apparent power
  Serial.print(" ");
  Serial.println(IrmsA1);          // Irms
  return (IrmsA1*voltage);
}
float getApparentPowerA2() {
  double IrmsA2 = emon2.calcIrms(1480);  // Calculate Irms only
  Serial.print ("Apparent power in A2 ->");
  Serial.print(IrmsA2*voltage);         // Apparent power
  Serial.print(" ");
  Serial.println(IrmsA2);          // Irms
  return (IrmsA2*voltage);
}
float getApparentPowerA3() {
  double IrmsA3 = emon3.calcIrms(1480);  // Calculate Irms only
  Serial.print ("Apparent power in A3 ->");
  Serial.print(IrmsA3*voltage);         // Apparent power
  Serial.print(" ");
  Serial.println(IrmsA3);          // Irms
  return ( IrmsA3*voltage);
}
float getApparentPowerA4() {
  double IrmsA4 = emon4.calcIrms(1480);  // Calculate Irms only
  Serial.print ("Apparent power in A4 ->");
  Serial.print(IrmsA4*voltage);         // Apparent power
  Serial.print(" ");
  Serial.println(IrmsA4);          // Irms
  return (IrmsA4*voltage);
}
void mqttEmit(String topic, String value) {
  if (client.publish((char*) topic.c_str(), (char*) value.c_str())) {
    //Serial.print("Publish ok (topic: ");
    //Serial.print(topic);
    //Serial.print(", value: ");
    //Serial.print(value);
    //Serial.println(")");
  } else {
    Serial.println("Publish failed");
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
