#include <Adafruit_CircuitPlayground.h> // include the adafruit circuit playground library 

float tempC; // this variable will store our temp sensor value in Celsius
float tempF; // this variable will store our temp sensor value in Fahrenheit

void setup() {
  Serial.begin(9600); // initialize serial
  CircuitPlayground.begin(); // initialize the circuit playground
}

void loop() {
  tempC = CircuitPlayground.temperature(); // update the tempC variable to store the current reading in C!
  tempF = CircuitPlayground.temperatureF(); // update the tempC variable to store the current reading in F!

  Serial.print("tempC: "); // print "tempC:"
  Serial.print(tempC); // print the value of the tempC variable
  Serial.print("  tempF: "); // print "tempF:"
  Serial.println(tempF); // print the value of the tempF variable

  delay(1000); // one second (1000 millisecond) delay between readings 
}
