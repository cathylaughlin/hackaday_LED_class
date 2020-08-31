#include <FastLED.h>

#define LED_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.

// gradient palette examples!

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( icemap_gp ) {
  0,     0,  0,  0,   //black
128,   0,  0,  255,   //blue
224,   0, 255,  255,   //almost ice white
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( purpsmap_gp ) {
  0,     0,  0,  0,   //black
128,   125,  0,  125,   //light purps
224,   255, 0,  255,   //purps
255,   255,255,255 }; //full white

CRGBPalette16 currentPalette (CRGB::Black); // this always holds our current color palette. defining it as black causes it to 'fade in' nicely when we power it on
CRGBPalette16 targetPalette( heatmap_gp ); // this should be set to the first palette you want to fade into!
TBlendType    currentBlending; // and the current blending type. see setup() for more on this

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = heatmap_gp; // or any palette you’ve defined above
    currentBlending = LINEARBLEND; // the two options here are LINEARBLEND and NOBLEND. I basically always use LINEARBLEND
}

void loop()
{
    ChangePalettePeriodically(); 

    // this blends our palettes seamlessly when we change the targetPalette above...
    uint8_t maxChanges = 24;
    EVERY_N_MILLISECONDS(20) {
      nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges); // blends palettes once they've changed
    }

    // here's how we fill our LEDs with colors from the palette
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; // motion speed. change "+ 1" to larger values to make it 'flow' through faster 
    
   /*  // or you can flip it and reverse it...
       static uint8_t startIndex = 255;
       startIndex = startIndex - 1; // motion speed 
    */
    FillLEDsFromPaletteColors(startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3; // comment out this line to fade all LEDs instead of shifting through
    }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { targetPalette = heatmap_gp;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { targetPalette = icemap_gp;   currentBlending = LINEARBLEND;  }
        if( secondHand == 20)  { targetPalette = purpsmap_gp;   currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { targetPalette = heatmap_gp;         currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { targetPalette = icemap_gp;   currentBlending = LINEARBLEND;  }
        if( secondHand == 50)  { targetPalette = purpsmap_gp;   currentBlending = LINEARBLEND; }
 
    }
}

// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
