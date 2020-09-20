/* contains code from mic_FFT_express example file */

#include <Adafruit_CircuitPlayground.h>
#include "Adafruit_ZeroFFT.h"
#include <FastLED.h>

// FastLED defines

#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 16

#define LED_PIN     6
#define NUM_LEDS    MATRIX_WIDTH * MATRIX_HEIGHT
#define BRIGHTNESS  48
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

const bool    kMatrixSerpentineLayout = true;

// FFT defines

//this must be a power of 2
#define DATA_SIZE 256

#define NUM_BINS MATRIX_WIDTH // how many bins? we want the length of the matrix, because each row will be displaying one bin

//the sample rate
#define FS 22000

//the lowest frequency that will register on the meter
#define FREQ_MIN 300 // in modern music most of the cool stuff happens with the bass!

//the highest frequency that will register on the meter
#define FREQ_MAX 2000 //  but we still wanna register the highs...

#define MIN_INDEX FFT_INDEX(FREQ_MIN, FS, DATA_SIZE)
#define MAX_INDEX FFT_INDEX(FREQ_MAX, FS, DATA_SIZE)

#define SCALE_FACTOR 32

int16_t binData[NUM_BINS + 1];
int16_t inputData[DATA_SIZE];

#define LIGHT_UPDATE_TIME 20 // number of ms between frames. too fast is a bad idea with pixel art...
unsigned long lightingMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

// Track low and high levels for dynamic adjustment
int minLvl = 0, maxLvl = 1000;

void loop() {
  int i;
  if (millis() > lightingMillis) {
  
    CircuitPlayground.mic.capture(inputData, DATA_SIZE);

    // Center data on average amplitude
    int32_t avg = 0;
    for(i=0; i<DATA_SIZE; i++) avg += inputData[i];
    avg /= DATA_SIZE;
    // Scale for FFT
    for(i=0; i<DATA_SIZE; i++)
      inputData[i] = (inputData[i] - avg) * SCALE_FACTOR;
  
    //run the FFT
    ZeroFFT(inputData, DATA_SIZE);

    // Sum inputData[] (FFT output) into binData[] bins.
    // Note that binData[] has one element more than the number of
    // rows actually displayed -- this is on purpose, as the last
    // element tends to be zero and not visually interesting.
    memset(binData, 0, sizeof binData); // Clear binData[] buffer
    for(i=MIN_INDEX; i<=MAX_INDEX; i++){
      int ix = map(i, MIN_INDEX, MAX_INDEX, 0, NUM_BINS);
      binData[ix] += inputData[i];
    }

    // Figure the max and min levels for the current frame
    int most = binData[0], least = binData[0];
    for(i=1; i<NUM_BINS; i++) {
      if(binData[i] > most)  most = binData[i];
      if(binData[i] < least) least = binData[i];
    }
    // Always have some minimum space between, else it's too "jumpy"
    if((most - least) < 15) most = least + 15;

    // Dynamic max/min is sort of a fake rolling average...
    maxLvl = (most > maxLvl) ?
      (maxLvl *  3 + most +  1) /  4 :  // Climb fast
      (maxLvl * 31 + most + 15) / 32;   // Fall slow
    minLvl = (least < minLvl) ?
      (minLvl *  3 + least +  3) /  4 : // Fall fast
      (minLvl * 31 + least + 31) / 32;  // Climb slow

    //display the data
    int n;
    int baseHue = 0; // holds our bonus hue. changes as we go up the columns
    int baseColor = 0; // starts with red. changes as we go over the rows
    int bonusColor = 0; // adds a little spice to each successive row
    for(i=0; i<NUM_BINS; i++) {
      bonusColor += 2;
      // Scale pixel data from 2 to MATRIX_HEIGHT. we'll light that many LEDs for this bin. 
      n = map(binData[i], minLvl, maxLvl, 0, MATRIX_HEIGHT);
      if(n < 0)        n = 0;
      else if(n > MATRIX_HEIGHT) n = MATRIX_HEIGHT;

      if (i < MATRIX_WIDTH / 3) {
        baseColor = 150; // blue
       // Serial.println("blue");
      } else if (i >= MATRIX_WIDTH / 3 && i < (MATRIX_WIDTH - (MATRIX_WIDTH / 3))) {
        baseColor = 192; // purps
       // Serial.println("purps");
      } else {
        baseColor = 0; // red
       // Serial.println("red");
      }
    
      baseHue = 0; // this will start at 0 and get increased with each pix up thru the row, to give us a nice gradient
      // now that we've got our value and basecolor for n, draw that many LEDs for this column. we want to color them all spectrum-y as well...
      for(int x = 0; x < n; x++) {
        baseHue += 5;
        int index = XY(i, x); // i is the bin / column number, x is the pixel in the column
        CHSV color = CHSV(baseColor + baseHue + bonusColor, 255, 255);
        leds[index] = color;
      }
      // and clear the others in the column, if any
      for(int x = n; x < MATRIX_HEIGHT; x++) {
        int index = XY(i, x); // i is the bin / column number, x is the pixel in the column
        leds[index] = CRGB::Black;
      }
    }
    // now we've colorized every bin! show this frame and then wait for the next timer
    FastLED.show();
    lightingMillis = millis() + LIGHT_UPDATE_TIME; // the last thing inside your timer control block is always saving the target time for the next loop around! 
  }
}

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * MATRIX_WIDTH) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (MATRIX_WIDTH - 1) - x;
      i = (y * MATRIX_WIDTH) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * MATRIX_WIDTH) + x;
    }
  }
  
  return i;
}
