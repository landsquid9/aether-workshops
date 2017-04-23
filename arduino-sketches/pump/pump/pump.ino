/*
 *  Receive messages from the internet which are used
 *  to turn a water pump on and off
 *  
 *  (This is a comment. Comments are ignored by the computer
 *  and are used to make notes.)
 */

/* First we include code that other people have written to 
 *  do various things. 
 */
#include <ESP8266WiFi.h> // This gives us access to internet communication code.
#include <aether-client.h> // This gives us access to Aether-specific code.


/* We store the pin number "2" in a variable to use later */
const int PUMP_PIN = D2;

/* This "true/false" value says whether the pump should be on or not */
bool pumpState = false;

/* We declare some variables to hold the name of the network we are connecting
 *  to, and the password. 
 */
const char* ssid     = "SSID";
const char* password = "PASS";


/* We set up Aether */
AetherClient ae;

/* This code will run when we receive a message
 *  from Aether
 */
void receiveFunc()
{
    /*  We set pumpState to its opposite 
   *  (true if it's false, and false if it's true)
   */
  pumpState = !pumpState;

  /*  We send a signal to the pin we defined up there ^^
   *  if pumpState is "true" this will be 3.3 volts.
   *  if it is false it will be 0 volts
   */
  digitalWrite(PUMP_PIN, pumpState);
}

/* Setup runs ONCE when the program starts */
void setup() 
{
  /* --- PUMP SETUP --- */
  
  /* We tell the program that we want the pin number we
   *  sepcified up there ^^^ to be used for "output" rather
   *  than "input".
   */
  pinMode(PUMP_PIN, OUTPUT);
  /* We send a signal to the pin to put it in an initial state */
  digitalWrite(PUMP_PIN, pumpState);
    /* This enables us to see messages from the micro-controller
   *  on our computer.
   */
  Serial.begin(115200);

  /* --- AETHER SETUP --- */
  
  /* First we give it the name of the server */
  ae.setCustomUrl("aether-iot.herokuapp.com");
  /* We set the port to connect to */
  ae.setCustomPort(80);
  /* We let it know we want as many notifications as possible */
  ae.setLogVerbosity(LOG_VERBOSE);

  /* We declare a variabling to indicate if we are connected */
  bool connection = false;

  /* Everything in the while loop runs while "connection" is
   *  NOT true. The exclamation mark means "not".
   */
  while(!connection) // While we are not connected ...
  {
    /* We try to connect using the network name and password 
     *  we set up there ^^^
     */
    connection = ae.connectToAP(ssid, password); // ... run this code
  }

  /* This registers our device, telling Aether what we want to do.
   *  The first part is the name of this device, the second is
   *  whether we want to send or receive, and the third is the type
   *  of data we want to send or receive. "Pulse" just means that we do 
   *  not care what the data is, we just run the same thing
   *  whenever we receive a message. The message basically acts like
   *  a button.
   */
  ae.connectToServer("Daniel", MODE_RECEIVE, DATA_PULSE);
  
    /* We tell Aether what to do when we receive a message. This is the 
   *  code we wrote up there ^^^ with the name "receiveFunc".
   */
  ae.receiveData(receiveFunc);
}


/* This runs REPEATEDLY after "setup" has run */
void loop()
{
  /* Aether stuff. We need to put it here. */
  ae.loop();
}
 
