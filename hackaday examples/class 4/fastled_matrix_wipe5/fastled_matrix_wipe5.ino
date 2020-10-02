//Matthew Davis, articulatedartworks.com

#include <Adafruit_CircuitPlayground.h>
#include <FastLED.h>
#include <Wire.h>
#include <SPI.h>

#define NUM_LEDS 256
#define LED_PIN 6 //pin 6 = A1 on curcuit playground express
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS  64
#define CAP_PIN1 A3 // change this number to read a different pin A1 through A7! 
#define THRESHOLD 500 // the min. reading we're counting as a capacitive touch!
#define CLICKTHRESHHOLD 120 //doubletap  sensitivity

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

uint8_t rows = 16;
uint8_t columns = 16;
uint8_t counter = 0;
int wait = 0;
int i = 0;
int j = 0;
int k = 0;
int capData = 0;
uint8_t Hue = 0;
uint8_t Sat = 0;
uint8_t Val = 0;
uint8_t colorIndex = 0;
int effectCounter = 0;
unsigned long capTouchInterval =200; //Capacitive touch measurement really slows things down
unsigned long previousTime =0;
unsigned long currentTime =0;

void setup() { 
  delay(1000);  
//  while (!Serial);  /not sure where this fragment of code came from, don't seem to need it...
  Serial.begin(57600);
  CircuitPlayground.begin();  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  currentBlending = NOBLEND; 
  Serial.println("resetting");
  
//sets up double tap and 2 buttons
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  CircuitPlayground.setAccelTap(2, CLICKTHRESHHOLD);
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), tapTime, FALLING);
  pinMode(CPLAY_LEFTBUTTON, INPUT_PULLDOWN); //REQUIRES CIRCUIT PLAYGROUND LIBRARY
  pinMode(CPLAY_RIGHTBUTTON, INPUT_PULLDOWN); //REQUIRES CIRCUIT PLAYGROUND LIBRARY 
     
  FastLED.clear();  
  FastLED.show(); 
  delay(2000);  
}

void loop() { 
// List of acceptable CRGB Struct colors @ http://fastled.io/docs/3.1/struct_c_r_g_b.html  

  currentTime = millis();

  if (currentTime - previousTime > capTouchInterval){ // used a millis timer to prevent this from slowing things down
// not sure I totally have this right, but it seems to work 
      capData = CircuitPlayground.readCap(CAP_PIN1);  //this is a very slow function
      Serial.println("checking captouch sensor");      
  }  
  if (digitalRead(CPLAY_LEFTBUTTON)) { //REQUIRES CIRCUIT PLAYGROUND LIBRARY
    effectCounter = 1;
    Serial.println("left button");
  }
  if (digitalRead(CPLAY_RIGHTBUTTON)) { //REQUIRES CIRCUIT PLAYGROUND LIBRARY
    effectCounter = 2;
    Serial.println("right button");
  }
  if (capData > THRESHOLD){ //curently set to pin A3
    Serial.println("captouch loop!");
    effectCounter = 3;
    capData = 0;
  }

// double tap accelerometer interrupt sets effectCounter to 4, purple cube effect

  if (effectCounter == 1){
    if (currentTime - previousTime >=30){  // the number determines speed
      SwipeUp(); //set up to have random hue
    } 
  }   
  if (effectCounter == 2){ 
    if(currentTime - previousTime >=30){// the number determines speed
      SwipeRight(); //uses colorpallette with a shifting rainbow
    }  
  }
  if (effectCounter == 3){ 
    if(currentTime - previousTime >=20){// the number determines speed
      SwipeDiag(CRGB::Red); //color is always the same
    }  
  }
  if (effectCounter == 4){ 
    if(currentTime - previousTime >=60){// the number determines speed
      SwipeCube(CRGB::Purple); //color is always the same
    }  
  }
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void tapTime(void) {
   effectCounter = 4;
   Serial.println("doubletap!");
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void SwipeUp(){  //runs a line up the rows with a random color
  if (counter < rows){  //indexes up rows
    if (counter == 0 && i == 0){  // sets a color with random hue 
      Hue = random8();
      Sat = 255;
      Val = 255;
    }
    if (i < columns){  //fills a row of pixels
      leds[i+counter*columns] = CHSV(Hue,Sat,Val);
      i++;    
    }
    else {  //indexes to next row after row is filled
      previousTime = currentTime;
      i = 0;
      counter++;
      fadeToBlackBy(leds,NUM_LEDS,120);
      FastLED.show();
    }
  }
  else {
    effectCounter = 0;
    counter = 0;
  }
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void SwipeRight(){ //runs a line along the columns
  if (counter < columns){  //indexes along columns
    if (i < rows/2){  //fills a column
      leds[counter+i*32] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
      colorIndex +=16;  //Advances one color in the palette
      leds[counter+i*32+31-2*counter] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
      colorIndex +=16;  //Advances one color in the palette
      i++;    
    }
    else {  //indexes to next row after row is filled
      previousTime = currentTime;
      i = 0;
      counter++;
      fadeToBlackBy(leds,NUM_LEDS,40);
      FastLED.show();
    }
  }
  else {
    counter = 0;
    effectCounter = 0;
    colorIndex +=16;  //shifts the rainbow for the next swipe
  }
}   

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//brute force math approach making a diagonal line swipe. xy() function would be MUCH easier to code!
void SwipeDiag(CRGB myColor){//runs a a line diagonally across matrix from lower left  
  if (counter < rows){ //does bottom left half
    if ((counter & 1) != 0){ // odd numbers, will always light an even num of pixels
      if (i < counter){
        leds[i*16+counter-i] = myColor;
        leds[i*16+32-counter+i] = myColor;
        i = i+2;
      } 
      else {  //indexes to next line with odd number of pixels
        previousTime = currentTime;
        i = 0;
        counter++;
        fadeToBlackBy(leds,NUM_LEDS,120);
        FastLED.show();   
      }
    } 
    else { //even numbers including zero, will always light an odd num of pixels
      leds[counter] = myColor;
      i = i+2;        
      if (i > 1 && i <=counter){
        leds[i*17-counter-2] = myColor;
        leds[i*15+counter] = myColor;
      }  
      else {  //indexes to next line with even number of pixels
        previousTime = currentTime;
        i = 0;
        counter++;
        fadeToBlackBy(leds,NUM_LEDS,120);
        FastLED.show();
      }    
    }
  }

  else if (counter >= 16 && counter <=30){ //does top right half
    if ((counter & 1) != 0){ // odd numbers, will always light an even num of pixels
      if(i < ((31-counter)/2)){
        j= 8-i;
        leds[j*32+14-counter-(2*i)] = myColor;
        leds[j*32-46+counter+(2*i)] = myColor;                
        i++;
      }
      else {  //indexes to next line with odd number of pixels
        previousTime = currentTime;
        i = 0;
        counter++;
        fadeToBlackBy(leds,NUM_LEDS,120);
        FastLED.show();
      }
    }
    else { //even numbers including zero, will always light an odd num of pixels
      leds[(((counter-16)/2)*32)+16] = myColor;
      if(i < ((30-counter)/2)){
        j= 8-i;
        leds[j*32+14-counter-(2*i)] = myColor;
        leds[j*32-46+counter+(2*i)] = myColor;        
        i++;
      }
      else {  //indexes to next line with odd number of pixels
        previousTime = currentTime;
        i = 0;
        j = 0;
        counter++;       
        fadeToBlackBy(leds,NUM_LEDS,120);
        FastLED.show();
      }
    }
  }  

  else { // runs when effect has been completed
    effectCounter = 0;
    counter = 0;
  }
} 

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//brute force math approach making a shrinking box. xy() function would make it MUCH easier to code!
void SwipeCube(CRGB myColor){  //cube shrinks from the perimeter to the center
  if (counter < rows/2){// counts up a number of rows
    if ((counter & 1) != 0){  //does the 2nd largest square and then every other one after
      if (j < (7-counter)){  //this does the columns
        leds[32+(j*32)+counter+((counter-1)*16)] = myColor;  //left side
        leds[63+(j*32)-counter+((counter-1)*16)] = myColor;  //left side         
        leds[47+(j*32)-counter+((counter-1)*16)] = myColor;  //right side
        leds[48+(j*32)+counter+((counter-1)*16)] = myColor;  //right side
        j++; 
      }     
      if (i < (16-2*counter)){ //this does the rows
        leds[30*(counter+1)/2-i] = myColor;
        leds[255-(30*(counter+1)/2-i)] = myColor;
        i++;
      }
      else { //only enters after the rows and columns are filled
        previousTime = currentTime;
        i = 0;
        j = 0;
        counter++;
        fadeToBlackBy(leds,NUM_LEDS,120);
        FastLED.show();             
      }
    }
    else{  //does the largest square and then every other one after           
      if (j < (7-counter)){  //this does the columns  
        leds[31+(15*counter)+(j*32)] = myColor;  //left side
        leds[32+(17*counter)+(j*32)] = myColor;  //left side
        leds[16+(17*counter)+(j*32)] = myColor;  //right side
        leds[47+(15*counter)+(j*32)] = myColor;  //right side    
        j++; 
      }
      if (i < (16-2*counter)){  //this does the rows
        leds[16*counter+counter+i] = myColor; 
        leds[255-(16*counter+counter+i)] = myColor;              
        i++;
      }     
      else {  //only enters after the rows and columns are filled
        previousTime = currentTime;
        i = 0;
        j = 0;
        counter++;
        fadeToBlackBy(leds,NUM_LEDS,120);
        FastLED.show();        
      }  
    } 
  }
  else {
    effectCounter = 0;
    counter = 0; 
  }
}
