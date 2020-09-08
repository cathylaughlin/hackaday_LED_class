/* includes code from Smoothing.ino (in the public domain), written by David A. Mellis & modified by Tom Igoe:

  http://www.arduino.cc/en/Tutorial/Smoothing

*/
#include <Adafruit_CircuitPlayground.h>

#define CAP_PIN1 A1 // change this number to read a different pin! 
#define THRESHOLD 150 // the min. deviation from the running average that we're counting as a touch!
#define MAX_TOUCH_VALUE 1000 // if the touch exceeds this value, we're counting it, since it's close to the max the pin can measure

#define SENSOR_UPDATE_TIME 200
unsigned long sensorMillis = 0; // all variables that hold millis() MUST be an unsigned long. Otherwise you can accidentally overflow the variable!

// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
     
void setup() {
  // Initialize serial.
  Serial.begin(9600); 
  
  // Initialize Circuit Playground library.
  CircuitPlayground.begin();

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  delay(1000); 
}

int takeReading() {
  int saveIndex = 0;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = CircuitPlayground.readCap(CAP_PIN1);
  // and save our index so we can return the value
  saveIndex = readIndex; 
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(average);
  // then return the latest reading so we can use it
  return (readings[saveIndex]);
}

void loop() {
  if (millis() > sensorMillis) {
    // saves the cap touch data to a variable "capData"
    int capData = takeReading();
    // print the data and average
    Serial.print("Capacitive Touch Reading: "); 
    Serial.println(capData); 
    Serial.print("Running average: "); 
    Serial.println(average); 

    // do something IF the touch pad is being touched! 
    // in this case we check if the value is close to the max, OR if it is greater than the average + threshold.
    if (capData > MAX_TOUCH_VALUE || capData > average + THRESHOLD){
      Serial.println("TOUCHED!");
    } else {
      // do nothing - you can replace this to have a "default" mode 
    }
    sensorMillis = millis() + SENSOR_UPDATE_TIME; // the last thing inside your timer control block is always saving the target time for the next loop around! 
  }
}
