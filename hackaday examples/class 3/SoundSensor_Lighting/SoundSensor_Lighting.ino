// includes code from DemoReel100, Mark Kriegsman
#include <Adafruit_CircuitPlayground.h> // include circuit playground library
#include <FastLED.h>

#define LED_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  34 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

float sound_value; // this variable will store our sound data

uint8_t gHue = 0; // holds the base color for our animations

#define SLOW_LIGHT_UPDATE 40 // change these based on your experimentation
#define FASTER_LIGHT_UPDATE 30
#define FASTEST_LIGHT_UPDATE 20
int lightingUpdateTime = FASTER_LIGHT_UPDATE; // this time we plan to change this value based on our reading! so let's make it a variable
unsigned long lightingMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

#define SENSOR_UPDATE_TIME 200 // change this based on your experimentation
unsigned long sensorMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

// max sound levels for the three animation modes. 
// you'll need to experiment with the values in the serial monitor
// to see what these need to be for your room / speaking volume / music volume. 
// I had to turn the music up! ;)  
#define AMBIENT 60 // max level measured when no one's talking
#define SPEAKING 70 // max level measured when talking
#define MUSIC 120 // music was around 80-something...

void setup() {
  Serial.begin(9600); // initialize serial
  CircuitPlayground.begin(); // initialize Circuit Playground
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000); 
}

// stuff from DemoReel100!
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 45, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void loop() {

  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  
  if (millis() > sensorMillis) {
    // Take 10 milliseconds of sound data to calculate
    // update the sound_value variable to hold the current reading
    sound_value = CircuitPlayground.mic.soundPressureLevel(10);
  
    Serial.print("Sound Sensor SPL: "); // print "Sound Sensor SPL"
    Serial.println(sound_value); // print the reading from sound_value
    sensorMillis = millis() + SENSOR_UPDATE_TIME;
  }

  if (sound_value < AMBIENT) {
    // Serial.println("ambient noise level");
    // ambient animation 
    sinelon();
    lightingUpdateTime = FASTER_LIGHT_UPDATE;
  } else if (sound_value < SPEAKING) {
    //  Serial.println("speaking noise level");
    // speaking animation
    juggle();
    lightingUpdateTime = SLOW_LIGHT_UPDATE;
  } else if (sound_value < MUSIC) {
    // Serial.println("music noise level");
    // music animation   
    confetti(); 
    lightingUpdateTime = FASTEST_LIGHT_UPDATE;
  }
  
  if (millis() > lightingMillis) {
    FastLED.show(); 
    lightingMillis = millis() + lightingUpdateTime; // the last thing inside your timer control block is always saving the target time for the next loop around! 
  }
}
