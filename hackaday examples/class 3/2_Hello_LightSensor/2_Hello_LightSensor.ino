#include <Adafruit_CircuitPlayground.h> // include the adafruit circuit playground library 

int value; // this variable will store our light sensor value allowing us to do THINGS with it!

void setup() {
  Serial.begin(9600); // initialize serial 
  CircuitPlayground.begin(); // initialize Circuit Playground
}

void loop() {
  value = CircuitPlayground.lightSensor(); // update "value" to store the light sensor reading
  
  Serial.print("Light Sensor: "); // print out "light sensor"
  Serial.println(value); // print the current value for the light sensor 
  
  delay(1000); // delay of 1 second (1000 milliseconds) between readings 
}
