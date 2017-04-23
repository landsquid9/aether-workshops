/*
 *  Receive messages from the internet which are used
 *  to flash some lights prettily with a PCA9685
 *  
 *  (This is a comment. Comments are ignored by the computer
 *  and are used to make notes.)
 */

 /* ===== INITIAL CODE ===== */

 /* First we include code that other people have written to 
 *  do various things. 
 */
#include <ESP8266WiFi.h> // This gives us access to internet communication code.
#include <aether-client.h> // This gives us access to Aether-specific code.
#include <Wire.h> // This is used in tandem with the Adafruit library
#include <Adafruit_PWMServoDriver.h> // This allows us to control the PCA9685 board

 /* We declare some variables to hold the name of the network we are connecting
 *  to, and the password. 
 */
const char* ssid     = "Watery Grave 2.4";
const char* password = "PigTurtle16";

/* We set up Aether */
AetherClient ae;

/* We set up the PCA9685 */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* We create a "true/false" variable saying if the lights should be on */
bool lightOn = false;

/* We create a variable saying how many lights there are */
const unsigned int NUM_LEDS = 16;

/* We create an "array" of variables, each of which determines
 *  how bright one of LEDs is 
 */
int pwmValues[]= {0, 50, 100, 150, 200, 250, 300, 350, 400,
                  450, 500, 550, 600, 650, 700, 750};

/* ===== ON RECEIVING A MESSAGE  CODE ===== */

/* This code will run when we receive a message
 *  from Aether
 */
void receiveFunc()
{
  /* We set lightOn to be the opposite of what it was already.
   *  true if it was false, and false if it was true
   *  the exclamation mark means "not"
   */
  lightOn = !lightOn;
}

/* ===== SETUP CODE ===== */
 
void setup() 
{
  /* This enables us to see messages from the micro-controller
   *  on our computer.
   */
  Serial.begin(115200);

  /* --- LIGHT SETUP --- */
  /* We set up pins 4 and 5 to communicate with the PCA9685 */
  Wire.pins(4, 5); 
  /* We open communications */
  pwm.begin();
  /* We set how fast the pulse are which we send to the lights */
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

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

/* ===== LOOP CODE ===== */

/* This runs REPEATEDLY after "setup" has run */
void loop()
{
  /* Aether stuff. We need to put it here. */
  ae.loop();

  /* The code in this "for loop" will run for each LED we have.
   *  We set i to 0. 
   *  As long as i is "LESS THAN" NUM_LEDS we repeat the loop.
   *  We increase i by 1 each time the loop runs.
   */
  for(int i = 0; i < NUM_LEDS; i ++) 
  {
    /* If lightOn has been set to "true",
     *  carry out the code in the curly brackets
     */
    if(lightOn) 
    {
      /* we increase the "i"th value in the array by 10 (brighter) */
      pwmValues[i] += 10; 

      /* if it is less than 4096 (our maximum) we set it back to 0 */
      if(pwmValues[i] > 4096)
      {
        pwmValues[i] = 0;
      }

      /* finally we send a message to the PCA9685, saying we want the "i"th 
       *  light to have its brightness set to the "i"th element of our array
       */
      pwm.setPWM(i, 0, pwmValues[i]);
    } 
    else // if lightOn was not set to true do this instead
    {

      /*  we send a message to the PCA9685, saying we want the "i"th 
       *  light to have its brightness set to 0
       */
      pwm.setPWM(i, 0, 0);
    }
  }
 }
 
