/**
   Fun Leds v0.1

   Simple led controller, Arduino part. 
   
   See: https://github.com/feri/funleds
   
   Ferenc Szekely (c) 2020
**/

/* color definitions */
#include "colors.h"

String ver = "0.1";

// enable / disable debug mode
#define DEBUG

/* output pins where the leds are connected on the Arduino Nano */
#define redLed 9
#define greenLed 10
#define blueLed 11

static boolean debug = false;

int leds[] = {redLed, greenLed, blueLed};

const int num = 3;
int prevStates[num];

float in = 0;
float out = 0;

const int rows = 2;
const int columns = 5;
//float color[3] = black;
//float fade[2] = {in, out};

typedef struct rgb {
  int rval; 
  int gval;
  int bval; 
} rgb;

typedef struct showStruct {
  struct rgb color; 
  float in;
  float out; 
} showStruct;

int items = 19;
struct showStruct *previous, *current;
//struct showStruct show[] = {{black, 0, 0}, {red, 1, 0}, {green, 1, 0}, {blue, 1, 0}, {white, 2, 2}};
//struct showStruct show[] = {
//  {{0, 0, 0}, 1, 0}, 
//  {{40, 0, 0}, 1, 0}, 
//  {{80, 0, 0}, 1, 0},
//  {{120, 0, 0}, 1, 0},
//  {{160, 0, 0}, 1, 0},
//  {{200, 0, 0}, 1, 0},
//  {{255, 0, 0}, 1, 0},
//  {{200, 0, 0}, 1, 0},
//  {{160, 0, 0}, 1, 0},
//  {{120, 0, 0}, 1, 0},
//  {{80, 0, 0}, 1, 0},
//  {{40, 0, 0}, 1, 0}, 
//  {{0, 0, 0}, 1, 0}, 
//  {{0, 40, 0}, 1, 0},
//  {{0, 80, 0}, 1, 0},
//  {{0, 120, 0}, 1, 0},
//  {{0, 160, 0}, 1, 0},
//  {{0, 200, 0}, 1, 0},
//  {{0, 255, 0}, 1, 0},
//  {{0, 200, 0}, 1, 0},
//  {{0, 160, 0}, 1, 0},
//  {{0, 120, 0}, 1, 0},
//  {{0, 80, 0}, 1, 0},
//  {{0, 40, 0}, 1, 0},
//  {{0, 0, 0}, 1, 0}
//};

struct showStruct show[] = {
  {{0, 0, 0}, 0.5, 0}, 
  {{40, 0, 0}, 0.5, 0}, 
  {{80, 0, 0}, 0.5, 0},
  {{120, 0, 0}, 0.5, 0},
  {{160, 0, 0}, 0.5, 0},
  {{200, 0, 0}, 0.5, 0},
  {{255, 0, 0}, 0.5, 0}, 
  {{255, 40, 0}, 0.5, 0},
  {{255, 80, 0}, 0.5, 0},
  {{255, 120, 0}, 0.5, 0},
  {{255, 160, 0}, 0.5, 0},
  {{255, 200, 0}, 0.5, 0},
  {{255, 255, 0}, 0.5, 0},
  {{255, 255, 40}, 0.5, 0},
  {{255, 255, 80}, 0.5, 0},
  {{255, 255, 120}, 0.5, 0},
  {{255, 255, 160}, 0.5, 0},
  {{255, 255, 200}, 0.5, 0},
  {{255, 255, 255}, 0.5, 0},
};

/**
 * Sends greetings via serial port.
 * Resets all inputs and outputs.
 */
void trace(String message, bool newline = false) {
  if (debug) {
    newline ? Serial.println(message) : Serial.print(message);
  }
}

/**
 * Sends greetings via serial port.
 * Resets all inputs and outputs.
 */
void setup() {
  Serial.begin(115200);
  Serial.print("Fun Leds v");
  Serial.println(ver);

#ifdef DEBUG
  debug = true;
  Serial.println("************* DEBUG MODE *************");
#endif 
 
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  setLed(redLed, 0);
  setLed(greenLed, 0);
  setLed(blueLed, 0);

  digitalWrite(LED_BUILTIN, HIGH);

#ifdef DEBUG
  Serial.println(sizeof(show));
  Serial.println("-------------");
#endif 

  previous = current = &show[0];
}

/** 
 *  Sets intesity of a led
 */
void setLed(int led, int val) {
  prevStates[led] = val;  
  analogWrite(led, val);
#ifdef DEBUG
//  Serial.print("led: ");
//  Serial.println(led);  
//  Serial.print(" set to: ");
//  Serial.println(val);
#endif 
}

/**
 * Process the light show definition stored in show[]
 */
void process(showStruct previous, showStruct current) {
  // instant switch on
  if (current.in == 0) {
      setLed(redLed, current.color.rval);
      setLed(greenLed, current.color.gval);
      setLed(blueLed, current.color.bval);
      return;
  }
 
  int redSteps = current.color.rval - previous.color.rval;
  int greenSteps = current.color.gval - previous.color.gval;
  int blueSteps = current.color.bval - previous.color.bval;

#ifdef DEBUG
  Serial.print("Total Steps: ");
  Serial.print(redSteps);
  Serial.print(',');
  Serial.print(greenSteps);
  Serial.print(',');
  Serial.println(blueSteps);
#endif 

  // find the biggest step among colors
  int biggest = abs(redSteps);
  biggest < abs(greenSteps) ? biggest = abs(greenSteps) : 1;
  biggest < abs(blueSteps) ? biggest = abs(blueSteps) : 1;
  
  // figure out how much to sleep between setting new value
  int sleep = ceil((current.in * 1000) / biggest);

  int redStep, greenStep, blueStep;
  redStep = greenStep = blueStep = 0;

#ifdef DEBUG
  Serial.print("Biggest: ");
  Serial.println(biggest);
#endif 

  (redSteps != 0) ? redStep = (int)(biggest / redSteps) : 1;
  (greenSteps != 0) ? greenStep = (int)(biggest / greenSteps) : 1;
  (blueSteps != 0 ) ? blueStep = (int)(biggest / blueSteps) : 1;

#ifdef DEBUG
  Serial.print("Steps: ");
  Serial.print(redStep);
  Serial.print(',');
  Serial.print(greenStep);
  Serial.print(',');
  Serial.println(blueStep);
#endif 
  
  for (int i = 0; i < biggest ; i++) {
    setLed(redLed, prevStates[redLed] += redStep);
    setLed(greenLed, prevStates[greenLed] += greenStep);
    setLed(blueLed, prevStates[blueLed] += blueStep);
    delay(sleep);
  }

#ifdef DEBUG
  Serial.println("-----------");
#endif 
}

/**
 * Visual indication if the controller is OK.
 */
void heartBeat() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);                     
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);                      
}

/**
 * Main loop runs forever
 */
void loop() {
  //heartBeat();  
  for (int i=0; i < items; i++) {
    previous = current;
    current = &show[i];
    process(*previous, *current);
  }
}
