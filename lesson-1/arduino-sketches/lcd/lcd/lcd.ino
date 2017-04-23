/*
 *  Receive messages from the internet which are used
 *  to turn a message on and off.
 *  
 *  (This is a comment. Comments are ignored by the computer
 *  and are used to make notes.)
 */

/* First we include code that other people have written to 
 *  do various things. 
 */
#include <ESP8266WiFi.h>    // This gives us access to internet communication code.
#include <aether-client.h>  // This gives us access to Aether-specific code.
#include <LiquidCrystal.h>  // This fives us access to controls for the LCD

/* Initialise the LCD. The numbers correspond to the "pins" on the 
 * Micro-controller. */
LiquidCrystal lcd(14, 12, 13, 16, 5, 4);

/* We declare some variables to hold the name of the network we are connecting
 *  to, and the password. 
 */
const char* ssid     = "SSID";
const char* password = "PASS";

/* This is a true/false variable to say whether the message should be displayed */
bool lcdOn = false;

/* We set up Aether */
AetherClient ae;

/* This little bit of code is run when we receive a message from Aether */
void receiveFunc()
{
  /*  We set lcdOn to its opposite 
   *  (true if it's false, and false if it's true)
   */
  lcdOn = !lcdOn; 
}

/* Setup runs ONCE when the program starts */
void setup() 
{
  /* This enables us to see messages from the micro-controller
   *  on our computer.
   */
  Serial.begin(115200);

  /* --- LCD SETUP --- */

  /* We tell the program how many columns and rows our
   *  LCD has.
   */
  lcd.begin(16, 2);

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
  ae.connectToServer("LCD", MODE_RECEIVE, DATA_PULSE);

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

  /* This code runs IF we have set lcdOn to "true" */
  if(lcdOn) // If lcdOn is "true" ...
  {
    /* First we set the cursor to the first line */
    lcd.setCursor(0, 0);          // ... do this
    /* We display a message */
    lcd.print("A Message From");  // ... and this
    /* we set the cursor to the second line */
    lcd.setCursor(0, 1);          // ... and this
    /* and write the rest of the message */
    lcd.print("The Internet");    // ... and finally this
  }
  else // If lcdOn was NOT "true" ...
  {
    /* Clear the display */
    lcd.clear(); // ... run this instead
  }

  /* Delay for 10ms. Stops the program running too fast */
  delay(10);
 }
 
