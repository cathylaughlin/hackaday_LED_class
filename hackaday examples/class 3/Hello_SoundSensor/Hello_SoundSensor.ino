#include <Adafruit_CircuitPlayground.h> // include circuit playground library

float sound_value; // this variable will store our sound data

void setup() {
  Serial.begin(9600); // initialize serial
  CircuitPlayground.begin(); // initialize Circuit Playground
}

void loop() {
  // Take 10 milliseconds of sound data to calculate
  // update the sound_value variable to hold the current reading
  sound_value = CircuitPlayground.mic.soundPressureLevel(10);
  
  Serial.print("Sound Sensor SPL: "); // print "Sound Sensor SPL"
  Serial.println(sound_value); // print the reading from sound_value

  delay(90); // short 90 millisecond delay 
}
