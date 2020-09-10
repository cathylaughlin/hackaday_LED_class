#include <FastLED.h>

#define DATA_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

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

void loop(){
  
}
