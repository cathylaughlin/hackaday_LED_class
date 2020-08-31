#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 10

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 8
#define CLOCK_PIN 13
#define BRIGHTNESS  64

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    // FastLED.addLeds<SM16703, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1812, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1904, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS2903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<GS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GE8822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886_8BIT, DATA_PIN, RGB>(leds, NUM_LEDS);
    // ## Clocked (SPI) types ##
    // FastLED.addLeds<LPD6803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<SK9822, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical

  FastLED.setBrightness(  BRIGHTNESS );
}
/*
// original example, all LEDs blink
void loop() { 
    //Turn the LED on, then pause
    for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(500);
    // Now turn the LED off, then pause
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
   }
 FastLED.show();
   delay(500);
}
*/
/* 
 //top half of the LEDs blink
 void loop() { 
    // Turn the LED on, then pause
    for (int i = 0; i < NUM_LEDS / 2; i++) {
   leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(500);
    // Now turn the LED off, then pause
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      leds[i] = CRGB::Black;
   }
 FastLED.show();
   delay(500);
}
*/

/*
// bottom half of the LEDS blink 
  void loop() { 
    // Turn the LED on, then pause
    for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
   leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(500);
    // Now turn the LED off, then pause
    for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
   }
 FastLED.show();
   delay(500);
}
*/

/*
// every other bottom half LEDS blink 
  void loop() { 
    // Turn the LED on, then pause
    for (int i = NUM_LEDS / 2; i < NUM_LEDS; i = i+2) {
   leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(500);
    // Now turn the LED off, then pause
    for (int i = NUM_LEDS / 2; i < NUM_LEDS; i = i+2) {
      leds[i] = CRGB::Black;
   }
 FastLED.show();
   delay(500);
}
*/


// HSV rainbow fade, class 2, one LED
void loop() { 
  for(int i=0; i < 256; i++) {
    leds[0] = CHSV(i, 255, 255);
    FastLED.show();
    delay(20);
  }
}

/*
// HSV rainbow fade, class 2, all LEDs
void loop() { 
  for(int i=0; i < 256; i++) {
    for(int j=0; j < NUM_LEDS; j++) {
      leds[j] = CHSV(i, 255, 255);
    }
    FastLED.show();
    delay(20);
  }
}
*/

/*
// HSV random colors in the red / orange range, class 2, all LEDs
void loop() { 
  CHSV myColor = CHSV( random8(0,40), 255, random8());
  for(int j=0; j < NUM_LEDS; j++) {
    leds[j] = myColor;
    }
    FastLED.show();
    delay(200);
}
 */
