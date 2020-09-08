/* includes code from Smoothing.ino (in the public domain), written by David A. Mellis & modified by Tom Igoe:

  http://www.arduino.cc/en/Tutorial/Smoothing

*/
#include <Adafruit_CircuitPlayground.h>
#include <FastLED.h>

#define LED_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define CAP_PIN1 A1 // change this number to read a different pin! 
#define THRESHOLD 150 // the min. deviation from the running average that we're counting as a touch!
#define MAX_TOUCH_VALUE 1000 // if the touch exceeds this value, we're counting it, since it's close to the max the pin can measure

#define SENSOR_UPDATE_TIME 200
unsigned long sensorMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

#define SLOW_LIGHT_UPDATE 60
#define FAST_LIGHT_UPDATE 20
int lightingUpdateTime = SLOW_LIGHT_UPDATE; // this time we plan to change this value based on our reading! so let's make it a variable
unsigned long lightingMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

uint8_t gHue = 0; // put this above setup(), as a global variable. ‘Global’ means that it’s not defined within any function, and is thus accessible from any function. 
uint8_t pos = 0; // pixel position for the lighting

// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
     
void setup() {
  // Initialize serial.
  Serial.begin(9600); 
  
  // Initialize Circuit Playground library.
  CircuitPlayground.begin();

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[pos] = CHSV( gHue, 255, 255);
  FastLED.show();

  delay(1000); 
}

// maybe the easiest algorithm?
void smartest_chase() {
  if (millis() > lightingMillis) {
    // fade them all. instant fade 
    //  fadeToBlackBy( leds, NUM_LEDS, 255);
    
    // fade them all. slow fade, combine with a slower update speed to get cool trails! 
    //fadeToBlackBy( leds, NUM_LEDS, 100);
    
    // fade them all. really slow fade, creates a 'brighter spot moves through lit strip' effect
    //fadeToBlackBy( leds, NUM_LEDS, 40);
    
    // fade them all. constantly changing fade values!
    fadeToBlackBy( leds, NUM_LEDS, beatsin16(10, 30, 200));
    
    // then light the LED at our position
    leds[pos] = CHSV( gHue, 255, 255);
  
    // then increase the position for next time, resetting it if we've gotten to the end of our LEDS.
    pos++;
    if (pos == NUM_LEDS) {
      pos = 0;
    }
    // then shift the color for next time, and show the lights.
    //gHue++; 
    // let's do this with more variation!
    uint8_t beatz = beatsin16(10, 50, 70); // I heard you like beats...
    uint8_t bonus = beatsin16(beatz, 1, 7); // seriously though, using a slow BPM to vary the BPM itself can create a cool sense of change over time, esp when it's driving something subtle like color 
    //Serial.println(bonus); // peek into what's happening if you want!  
    gHue = gHue + bonus;
    FastLED.show();
    lightingMillis = millis() + lightingUpdateTime; // the last thing inside your timer control block is always saving the target time for the next loop around! 
  }
}

int takeReading() {
  int saveIndex = 0;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = CircuitPlayground.readCap(CAP_PIN1);
  // and save our index so we can return the value
  saveIndex = readIndex; 
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(average);
  // then return the latest reading so we can use it
  return (readings[saveIndex]);
}

void loop() {
  if (millis() > sensorMillis) {
    // saves the cap touch data to a variable "capData"
    int capData = takeReading();
    // print the data and average
    Serial.print("Capacitive Touch Reading: "); 
    Serial.println(capData); 
    Serial.print("Running average: "); 
    Serial.println(average); 

    // do something IF the touch pad is being touched! 
    // in this case we check if the value is close to the max, OR if it is greater than the average + threshold.
    if (capData > MAX_TOUCH_VALUE || capData > average + THRESHOLD){
      Serial.println("TOUCHED!");
      // we're going to figure a new lighting update time based on the value read.
      // this will have the effect of making the light go faster when you hold the touch. 
      // to do this, we'll use map to tie the value of lightingUpdateTime to the value coming from the cap touch pin.
      // we'll use the expected max and min values from the cap touch pin, and translate them to max and min values for lighting timing.
      lightingUpdateTime = map(capData, 200, 1016, SLOW_LIGHT_UPDATE, FAST_LIGHT_UPDATE);
    } else {
      // do nothing - in this case we're still updating the animation speed. you can add to this to have a "default" mode 
      lightingUpdateTime = map(capData, 200, 1016, SLOW_LIGHT_UPDATE, FAST_LIGHT_UPDATE);
    }
    sensorMillis = millis() + SENSOR_UPDATE_TIME; // the last thing inside your timer control block is always saving the target time for the next loop around! 
  }
  
  // don't forget your animation loop!
  smartest_chase();
}
