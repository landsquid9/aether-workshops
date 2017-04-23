#include <ESP8266WiFi.h>
#include <aether-client.h>
const int ldrPin = A0;
const int led = D2;

int ldrValue = 0; // analogue ldr value 0-1023
int ldrReal = 0;  //ldr value converted to 0-100

int trigValue = 60; // value at which switchState is triggered.

int switchState = LOW;
int prevSwitchState = LOW;

/* Start the aether client */
AetherClient ae;
const char* ssid = "SSID";
const char* password = "PASS";

void setup()
{
  Serial.begin(115200);
  pinMode(ldrPin, INPUT); 
  pinMode(led, OUTPUT);

  ae.setCustomUrl("aether-iot.herokuapp.com");
   ae.setCustomPort(80);
   ae.setLogVerbosity(LOG_VERBOSE);

   bool connection = false;
    /* while we are not connected to an access point */
  while(!connection)
  {
    connection = ae.connectToAP(ssid, password);
    // connection = ae.connectToAP("SSID", "PASSWORD", 20000);
    Serial.print(".");
  }
  ae.connectToServer("Rosie", MODE_SEND, DATA_PULSE);

}

void getSwitchState() {
  
// read value from sensor
  ldrValue = analogRead(ldrPin);
  ldrReal = map(ldrValue, 0, 1023, 0, 100);  // map sensor range to values 0-100
  //Serial.println(ldrReal); // prints out value. 

  if(ldrReal <= trigValue)
  {
    digitalWrite(led, HIGH);
    switchState = HIGH;
  }
  else
  {
    digitalWrite(led, LOW);
    switchState = LOW;
  }
  
}

void loop()
{
  ae.loop();

  getSwitchState(); // calls getSwitchState function.

  if(switchState == LOW && prevSwitchState == LOW)
  {
    // do nothing
    }

  else if (switchState == LOW && prevSwitchState == HIGH)
   {
   Serial.println(switchState);
   delay(10);
   ae.sendData();
    }
    
  else if (switchState == HIGH && prevSwitchState == HIGH)
   {
   // do nothing
    }
    
  else// if(switchState == HIGH && prevSwitchState == LOW)
  {
    Serial.println(switchState);
    delay(10);
    ae.sendData();
  }
   prevSwitchState = switchState;
   
}

