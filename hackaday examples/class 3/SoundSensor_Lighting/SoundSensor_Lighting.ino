#include <Adafruit_CircuitPlayground.h> // include circuit playground library
#include <FastLED.h>

#define LED_PIN     8 // internal Circuit Playground LED pin
#define NUM_LEDS    10 // there are 10 of them 
#define BRIGHTNESS  64 // it's a good idea to turn down the brightness! otherwise your LEDs are too bright to photograph / film...
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

float sound_value; // this variable will store our sound data

uint8_t gHue = 0; // put this above setup(), as a global variable. ‘Global’ means that it’s not defined within any function, and is thus accessible from any function. 
uint8_t pos = 0; // pixel position for the lighting

#define SLOW_LIGHT_UPDATE 60
#define FAST_LIGHT_UPDATE 20
int lightingUpdateTime = SLOW_LIGHT_UPDATE; // this time we plan to change this value based on our reading! so let's make it a variable
unsigned long lightingMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

#define SENSOR_UPDATE_TIME 200
unsigned long sensorMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

// max sound levels for the three animation modes. you'll need to experiment with the values in the serial monitor
// to see what these need to be for your room / speaking volume / music volume. I had to turn the music up! ;)  
#define AMBIENT 60 // max level measured when no one's talking
#define SPEAKING 65 // max level measured when talking
#define MUSIC 80 // music was around 70-something...

void setup() {
  Serial.begin(9600); // initialize serial
  CircuitPlayground.begin(); // initialize Circuit Playground
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000); 
}

void loop() {
  if (millis() > sensorMillis) {
    // Take 10 milliseconds of sound data to calculate
    // update the sound_value variable to hold the current reading
    sound_value = CircuitPlayground.mic.soundPressureLevel(10);
  
    Serial.print("Sound Sensor SPL: "); // print "Sound Sensor SPL"
    Serial.println(sound_value); // print the reading from sound_value
    sensorMillis = millis() + SENSOR_UPDATE_TIME;
  }

  if (sound_value < AMBIENT) {
    // ambient animation 
  } else if (sound_value < SPEAKING) {
    // speaking animation
  } else if (sound_value < MUSIC) {
    // music animation 
  }
}
