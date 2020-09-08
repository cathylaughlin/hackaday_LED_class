#include <FastLED.h>


#define NUM_LEDS 10
#define DATA_PIN 8
#define LIGHT_UPDATE_TIME 20 // needs to be in milliseconds. 20 - 30 ms is a good update time for lights.
unsigned long lightingMillis; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);
}

void loop()
{

  if (millis() > LIGHT_UPDATE_TIME)  //test whether the period has elapsed
  {
    /*Begin Lighting Algorithm*/
    Serial.println("It's Time!");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(255, 255, 255);
    }

    FastLED.show(); // update the lights

    /*End Lighting Algorithm*/
    
    Serial.println("Update Timer");
    Serial.println(millis());
    Serial.println(lightingMillis); 
    
    lightingMillis = millis() + LIGHT_UPDATE_TIME; // the last thing inside your timer control block is always saving the target time for the next loop around!
  }
  
}
