
#include <ESP8266WiFi.h>
#include <aether-client.h>

int switchState = LOW;
int prevSwitchState = LOW;

#define BUTTON1 4
#define LED1 13
#define LED2 12


/* Start the aether client */
AetherClient ae;

const char* ssid = "SSID";
const char* password = "PASS";

void setup() {
  // declare the LED pins as outputs
Serial.begin(115200);
  
  pinMode(BUTTON, INPUT);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

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

void loop() {
  ae.loop();
  
  switchState1 = digitalRead(BUTTON);

  if(switchState == LOW && prevSwitchState == LOW)
  {
    // do nothing
    }

  else if (switchState == LOW && prevSwitchState == HIGH)
   {
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, HIGH);
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
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    Serial.println(switchState);
    delay(10);
    ae.sendData();
  }
   prevSwitchState = switchState;
}

