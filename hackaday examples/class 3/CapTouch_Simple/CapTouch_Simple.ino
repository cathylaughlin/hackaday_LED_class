    #include <Adafruit_CircuitPlayground.h>

    #define CAP_PIN1 A1 // change this number to read a different pin! 
     
    void setup() {
      // Initialize serial.
      Serial.begin(9600); 
      
      // Initialize Circuit Playground library.
      CircuitPlayground.begin();
    }
     
    void loop() {
      // Print cap touch reading to the serial port.
      Serial.println(CircuitPlayground.readCap(CAP_PIN1));
     
      // Update rate.
      delay(100);
    }
