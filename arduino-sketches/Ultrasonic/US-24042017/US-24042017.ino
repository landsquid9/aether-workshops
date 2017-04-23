/* US-015 proximity switch with ESP8266 */

#include <NewPing.h>
#include <ESP8266WiFi.h>
#include <aether-client.h>

#define TRIGGER_PIN  D4 // D4 Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     D4  // D4 Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define TRIG_DISTANCE 25 // distance at which change of state is triggered. 

int distance = 0;   // variable to hold distance
int switchState = LOW;
int prevSwitchState = LOW;

#define LED D3

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

/* Start the aether client */
AetherClient ae;
const char* ssid = "SSID";
const char* password = "PASS";

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  pinMode(LED, OUTPUT);

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
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  distance = sonar.ping_cm();
  
 //---- get rid of 0 values --- 
  if (distance <=0 || distance >= MAX_DISTANCE)
  {
    distance = MAX_DISTANCE;
    }
 // ------------------
    
  if(distance < TRIG_DISTANCE)
  {
    //do nothing
    digitalWrite(LED, HIGH);
    switchState = HIGH;
  }
  else // if (distance >= TRIG_DISTANCE)
  {
    digitalWrite(LED, LOW);
    switchState = LOW;
    }
}

void loop() 
{
  ae.loop();
  
  getSwitchState();
  
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

