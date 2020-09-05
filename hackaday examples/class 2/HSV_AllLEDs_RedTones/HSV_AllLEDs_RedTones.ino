#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 10

#define DATA_PIN 8
#define CLOCK_PIN 13
#define BRIGHTNESS  64

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() { 
  CHSV myColor = CHSV( random8(0,40), 255, random8());
  for(int j=0; j < NUM_LEDS; j++) {
    leds[j] = myColor;
    }
    FastLED.show();
    delay(200);
}
