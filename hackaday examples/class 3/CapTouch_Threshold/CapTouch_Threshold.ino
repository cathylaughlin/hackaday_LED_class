    #include <Adafruit_CircuitPlayground.h>

    #define CAP_PIN1 A1 // change this number to read a different pin! 
    #define THRESHOLD 500 // the min. reading we're counting as a touch!
     
    void setup() {
      // Initialize serial.
      Serial.begin(9600); 
      
      // Initialize Circuit Playground library.
      CircuitPlayground.begin();

      delay(1000); 
    }
     
    void loop() {
      // saves the cap touch data to a variable "capData"
      int capData = CircuitPlayground.readCap(CAP_PIN1);
      // print the data
      Serial.print("Capacitive Touch Reading: "); 
      Serial.println(capData); 

      // do something IF the touch pad is being touched! 
      if (capData > THRESHOLD){
        Serial.println("TOUCHED!");
      }
      else{
        // do nothing - you can replace this to have a "default" mode 
      }
   
     
      delay(200); // time between readings
    }
