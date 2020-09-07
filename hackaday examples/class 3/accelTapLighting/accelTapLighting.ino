#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>
#include <FastLED.h>

#define LED_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define LIGHT_UPDATE_TIME 20 // needs to be in milliseconds. 10 - 30 ms is a good update time for lights.
#define SLOW_LIGHT_UPDATE_TIME 30
unsigned long lightingMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!
#define TAP_LIGHT_TIME 1000 // one second
volatile unsigned long tapMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

uint8_t gHue = 0; // put this above setup(), as a global variable. ‘Global’ means that it’s not defined within any function, and is thus accessible from any function. 
uint8_t pos = 0;
volatile uint8_t tapped = 0;


// Adjust this number for the sensitivity of the 'click' force
// this strongly depend on the range! for 16G, try 5-10
// for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
#define CLICKTHRESHHOLD 120

void setup(void) {
  while (!Serial);
  
  Serial.begin(9600);
  CircuitPlayground.begin();

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  
  // 0 = turn off click detection & interrupt
  // 1 = single click only interrupt output
  // 2 = double click only interrupt output, detect single click
  // Adjust threshhold, higher numbers are less sensitive
  CircuitPlayground.setAccelTap(2, CLICKTHRESHHOLD);
  
  // have a procedure called when a tap is detected
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), tapTime, FALLING);
}

void tapTime(void) {
  // do something :)
   Serial.print("Tap! ");
   Serial.println(millis()); // the time
   tapped = 1;
   tapMillis = millis();
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
    lightingMillis = millis() + SLOW_LIGHT_UPDATE_TIME; // the last thing inside your timer control block is always saving the target time for the next loop around! 
  }
}

void loop() {
  if (tapped == 1) {
    smartest_chase();
    if (millis() > tapMillis + TAP_LIGHT_TIME) {
      // if it's been TAP_LIGHT_TIME since we started the interrupt,
      // stop it, clear the lights, and set tapped to 0 so we wait for the next tap.
      Serial.print("stop ");
      Serial.println(millis()); 
      tapped = 0;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
    }
  }

  
/*
 // *or* uncomment this for manual polling of tap data
  uint8_t click = CircuitPlayground.getAccelTap();
  if (click == 0) return;
  if (! (click & 0x30)) return;
  Serial.print("Click detected (0x"); Serial.print(click, HEX); Serial.print("): ");
  if (click & 0x10) Serial.print(" single click");
  if (click & 0x20) Serial.print(" double click");
  Serial.println();

  delay(100);
  return;
*/
}
