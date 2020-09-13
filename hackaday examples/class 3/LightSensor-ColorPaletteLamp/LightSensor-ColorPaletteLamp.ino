// Includes code snippets from fastLED's 'ColorPalette' example and ColorWavesWithPalettes by Mark Kriegsman, August 2015
#include <Adafruit_CircuitPlayground.h> // include the adafruit circuit playground library 
#include <FastLED.h>

/* challenge: create a lamp which reacts to ambient light levels.
 *  this one has different color palettes for low, medium, and bright light,
 *  and also changes animation speeds based on the light level.
 *  
 *  measures light every minute (we need to turn off the CiP's lights to do this!),
 *  and picks a new palette based on the last measured light value every 5 seconds.
 */

int light_value; // this variable will store our light sensor value allowing us to do THINGS with it!
int average_light = 150;
int bright_light = 550;

#define LED_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define SLOW_LIGHT_UPDATE 40 // change these based on your experimentation
#define FASTER_LIGHT_UPDATE 30
#define FASTEST_LIGHT_UPDATE 20
int lightingUpdateTime = SLOW_LIGHT_UPDATE; // this time we plan to change this value based on our reading! so let's make it a variable
unsigned long lightingMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

#define PALETTE_UPDATE_TIME 5000 // needs to be in milliseconds. 5 seconds per palette seems nice for a demo.
unsigned long paletteMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

// only check for ambient brightness every minute. reasonable for a lamp, and it avoids flicker...
#define SENSOR_UPDATE_TIME 60000 // I made this delay huge because we need to turn out the lights to measure ambient brightness! measuring too often made the lights flicker
unsigned long sensorMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

// we'll use these defines to choose a random palette for each light level, every 5 seconds. 
// makes a nice, floaty lamp with some variation
#define LOW_LIGHT_MIN_PALETTE 0
#define LOW_LIGHT_MAX_PALETTE 2 // these palettes are in the array at positions 0 - 2
#define MEDIUM_LIGHT_MIN_PALETTE 3
#define MEDIUM_LIGHT_MAX_PALETTE 5 // these palettes are in the array at positions 3 - 5
#define BRIGHT_LIGHT_MIN_PALETTE 6
#define BRIGHT_LIGHT_MAX_PALETTE 8 // these palettes are in the array at positions 6 - 8

// gradient palette examples!

/* my thinking, here: in low light we want subtle colors, and not too many different points
 *  inside the gradient. medium light, same thing but some brighter colors, trending more
 *  toward jewel tone. in bright light we want super active gradients with a lot of 
 *  color points and crazy candy colors!
 *  
 *  I found all of these by visiting Blackheartedwolf's art gradient collection on cpt-city:
 *  http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/index.html
 *  
 *  thanks so much, Blackheartedwolf and cpt-city / paletteknife!
 */  

DEFINE_GRADIENT_PALETTE( low_light1 ) {
  // Gradient palette "bhw3_40_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_40.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.
    0,   1,  1,  1,
   43,   4,  1, 11,
   76,  10,  1,  3,
  109, 161,  4, 29,
  127, 255, 86,123,
  165, 125, 16,160,
  204,  35, 13,223,
  255,  18,  2, 18
 }; 

DEFINE_GRADIENT_PALETTE( low_light2 ) {
  // Gradient palette "bhw3_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
    0,   3, 11, 75,
   48,  12, 44,155,
   96,  72,166,252,
  127,  88, 86,155,
  150,  43, 32, 88,
  168,  19, 29, 52,
  255,   3,  7,  5
}; 

DEFINE_GRADIENT_PALETTE( low_light3 ) {
  // Gradient palette "bhw1_sunset3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_sunset3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
    0, 227,237, 56,
   33, 186, 67,  1,
   71, 163, 21,  1,
   81, 157, 13,  1,
  188,  39, 21, 18,
  234,  12,  7,  4,
  255,  12,  7,  4
 }; 

DEFINE_GRADIENT_PALETTE(  medium_light1 ) {
// Gradient palette "bhw1_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.
    0, 227,101,  3,
  117, 194, 18, 19,
  255,  92,  8,192
 }; 

DEFINE_GRADIENT_PALETTE( medium_light2 ) {
// Gradient palette "bhw2_turq_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_turq.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
    0,   1, 33, 95,
   38,   1,107, 37,
   76,  42,255, 45,
  127, 255,255, 45,
  178,  42,255, 45,
  216,   1,107, 37,
  255,   1, 33, 95
}; 

DEFINE_GRADIENT_PALETTE( medium_light3 ) {
  // Gradient palette "bhw3_61_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_61.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
    0,  14,  1, 27,
   48,  17,  1, 88,
  104,   1, 88,156,
  160,   1, 54, 42,
  219,   9,235, 52,
  255, 139,235,233
 }; 

DEFINE_GRADIENT_PALETTE( bright_light1 ) {
  // Gradient palette "bhw4_099_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_099.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 48 bytes of program space.
    0, 249, 32,145,
   28, 208,  1,  7,
   43, 249,  1, 19,
   56, 126,152, 10,
   73, 234, 23, 84,
   89, 224, 45,119,
  107, 232,127,158,
  127, 244, 13, 89,
  150, 188,  6, 52,
  175, 177, 70, 14,
  221, 194,  1,  8,
  255, 112,  0,  1
 }; 

DEFINE_GRADIENT_PALETTE( bright_light2 ) {
  // Gradient palette "bhw2_whooo_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_whooo.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.
    0,   7,128, 95,
   30,   8,  9, 35,
   51,  21, 24, 25,
   66,  77, 55, 96,
   84, 255,255,125,
  101,  45, 39,  4,
  122, 224, 45,144,
  137,  82,  1, 34,
  160,  39, 47, 35,
  175, 155, 99, 91,
  204,   4, 10, 21,
  224,   5, 66, 68,
  255,  75,166, 78
}; 

DEFINE_GRADIENT_PALETTE( bright_light3 ) {
  // Gradient palette "bhw4_019_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_019.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.
    0, 252,184,125,
   33, 247, 74, 38,
   53, 130, 13, 28,
   79,  98, 99,255,
   99, 137, 71,112,
  127,  54, 22, 83,
  160, 252,184,125,
  191, 130, 13, 28,
  221,  98, 99,255,
  255,  21,  5, 21
 }; 

// our 'palette playlist' array, and a variable to hold the number of palettes in the array
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 currentPalette (CRGB::Black); // this always holds our current color palette. defining it as black causes it to 'fade in' nicely when we power it on
CRGBPalette16 targetPalette( gGradientPalettes[gCurrentPaletteNumber] ); // this should be set to the first palette you want to fade into!
TBlendType    currentBlending = LINEARBLEND; // the two options here are LINEARBLEND and NOBLEND. I basically always use LINEARBLEND

void setup() {
    Serial.begin(9600); // initialize serial
    CircuitPlayground.begin(); // initialize Circuit Playground
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
}

void loop()
{

    if (millis() > sensorMillis) {
      
      // we need to turn the lights off to measure ambient brightness! lol
      for ( int j = 0; j < NUM_LEDS; j++ ) {
        leds[j].maximizeBrightness(0);
      }
      FastLED.show();
      light_value = CircuitPlayground.lightSensor(); // update "value" to store the light sensor reading

      Serial.print("Light Sensor: "); // print out "light sensor"
      // then turn the lights back on
      for ( int j = 0; j < NUM_LEDS; j++ ) {
        leds[j].maximizeBrightness(BRIGHTNESS);
      } 
      FastLED.show();
      Serial.println(light_value); // print the current value for the light sensor
      sensorMillis = millis() + SENSOR_UPDATE_TIME;
    }

    ChangePalettePeriodically(); // this checks light level every N ms and selects a palette accordingly
  
    // this blends our palettes seamlessly when we change the targetPalette according to brightness...
    uint8_t maxChanges = 24;
    EVERY_N_MILLISECONDS(20) {
      nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges); // blends palettes once they've changed
    }

   if (millis() > lightingMillis) // if the time has come, animate the lights 
    {

      // here's how we fill our LEDs with colors from the palette
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; // motion speed. change "+ 1" to larger values to make it 'flow' through faster 
      
    /*  // or you can flip it and reverse it...
       static uint8_t startIndex = 255;
       startIndex = startIndex - 1; // motion speed 
      */
      FillLEDsFromPaletteColors(startIndex);
    
      FastLED.show();
      lightingMillis = millis() + lightingUpdateTime; // the last thing inside your timer control block is always saving the target time for the next loop around! 
    }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3; // comment out this line to fade all LEDs instead of shifting through
    }
}

// an example of several different ways to change palettes
void ChangePalettePeriodically() {
   if (millis() > paletteMillis)  // if the time has come, change the palette
    {
      
      int minp = 0;
      int maxp = 1;

      if (light_value >= bright_light) {
        minp = BRIGHT_LIGHT_MIN_PALETTE;
        maxp = BRIGHT_LIGHT_MAX_PALETTE;
        lightingUpdateTime = FASTEST_LIGHT_UPDATE;
      } else if (light_value < bright_light && light_value > average_light) {
        minp = MEDIUM_LIGHT_MIN_PALETTE;
        maxp = MEDIUM_LIGHT_MAX_PALETTE;
        lightingUpdateTime = FASTER_LIGHT_UPDATE;
      } else {
        minp = LOW_LIGHT_MIN_PALETTE;
        maxp = LOW_LIGHT_MAX_PALETTE;
        lightingUpdateTime = SLOW_LIGHT_UPDATE;
      }
      
      // picks a random palette each time. this pattern is a good way to say 'keep giving me a random number until it is not the current one'
      uint8_t newRand = random(minp, maxp);
      while(newRand == gCurrentPaletteNumber) {
        newRand = random(minp, maxp);
      }
      
      // then update our variable and palette with it
      gCurrentPaletteNumber = newRand;
      targetPalette = gGradientPalettes[gCurrentPaletteNumber];
      paletteMillis = millis() + PALETTE_UPDATE_TIME;
    }
}

// our 'playlist' array of gradient palettes
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  low_light1,
  low_light2,
  low_light3,
  medium_light1,
  medium_light2,
  medium_light3,
  bright_light1,
  bright_light2,
  bright_light3
  };

// total number of palettes in the array
const uint8_t gGradientPaletteCount = 
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
