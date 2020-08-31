// Includes code snippets from fastLED's 'ColorPalette' example and ColorWavesWithPalettes by Mark Kriegsman, August 2015

#include <FastLED.h>

#define LED_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define LIGHT_UPDATE_TIME 10 // needs to be in milliseconds. 10 - 30 ms is a good update time for lights.
unsigned long lightingMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

#define PALETTE_UPDATE_TIME 5000 // needs to be in milliseconds. 5 seconds per palette seems nice for a demo.
unsigned long paletteMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

// gradient palette examples!

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( icemap_gp ) {
  0,     0,  0,  0,   //black
128,   0,  0,  255,   //blue
224,   0, 255,  255,   //teal
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( greenmap_gp ) {
  0,     0,  0,  0,    //black
128,   20,  125,  20,  //light green 
224,   20, 200,  0,   // tealish
255,   0,255,0 }; //full green

// our 'palette playlist' array, and a variable to hold the number of palettes in the array
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 currentPalette (CRGB::Black); // this always holds our current color palette. defining it as black causes it to 'fade in' nicely when we power it on
CRGBPalette16 targetPalette( gGradientPalettes[gCurrentPaletteNumber] ); // this should be set to the first palette you want to fade into!
TBlendType    currentBlending = LINEARBLEND; // the two options here are LINEARBLEND and NOBLEND. I basically always use LINEARBLEND

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
}

void loop()
{

    ChangePalettePeriodically(); 

    // this blends our palettes seamlessly when we change the targetPalette above...
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
      lightingMillis = millis() + LIGHT_UPDATE_TIME; // the last thing inside your timer control block is always saving the target time for the next loop around! 
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
      // picks a random palette each time. this pattern is a good way to say 'keep giving me a random number until it is not the current one'
      uint8_t newRand = random(0, gGradientPaletteCount);
      while(newRand == gCurrentPaletteNumber) {
        newRand = random(0, gGradientPaletteCount);
      }
      
      // then update our variable and palette with it
      gCurrentPaletteNumber = newRand;
      targetPalette = gGradientPalettes[gCurrentPaletteNumber];
      paletteMillis = millis() + PALETTE_UPDATE_TIME;
    }
}

// our 'playlist' array of gradient palettes
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  heatmap_gp,
  icemap_gp, 
  greenmap_gp
  };

// total number of palettes in the array
const uint8_t gGradientPaletteCount = 
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
