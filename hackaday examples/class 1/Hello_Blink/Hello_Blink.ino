#include <Adafruit_CircuitPlayground.h>

void setup() {
  CircuitPlayground.begin();
}

void loop() {
  CircuitPlayground.redLED(HIGH);
  delay(2000);
  CircuitPlayground.redLED(LOW);
  delay(2000);
}
