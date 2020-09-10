#include <FastLED.h>

#define DATA_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

unsigned int lightingMillis = 0;
int LIGHT_UPDATE_TIME = 20; 

#define BRIGHTNESS          96

uint8_t gHue = 0; // put this above setup(), as a global variable. ‘Global’ means that it’s not defined within any function, and is thus accessible from any function. 
uint8_t pos = 0; 




void setup() {
  // put your setup code here, to run once:
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[pos] = CHSV( gHue, 255, 255);
  FastLED.show();

}

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
    Serial.println(bonus); // peek into what's happening if you want!  
    gHue = gHue + bonus;
    FastLED.show();
    lightingMillis = millis() + LIGHT_UPDATE_TIME; // the last thing inside your timer control block is always saving the target time for the next loop around! 
  }
}

void loop() {
  smartest_chase();
}
