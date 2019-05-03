#include <Adafruit_NeoPixel.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      94

#define sint(X) pix.gamma8(pix.sine8(X))




class StairFlight {
  char sensorPinA, sensorPinB;
  long int sensetime;
  char sense_dir;
  Adafruit_NeoPixel pix;
  
  public:
  StairFlight(char pixPin, char bottomPin, char topPin) {
    // Sensor setup
    pinMode(bottomPin, INPUT);
    pinMode(topPin, INPUT);
    sensorPinA = topPin;
    sensorPinB = bottomPin;
    pix = Adafruit_NeoPixel(NUMPIXELS, pixPin, NEO_RGB + NEO_KHZ800);
    sensetime = 0;
    
    pix.begin();
  }
  
  void loop() {
    long int time_sinse_sense = millis() - sensetime;
    
    if ((digitalRead(sensorPinA) || digitalRead(sensorPinB)) && time_sinse_sense > 2000) {
      sensetime = millis();
      sense_dir = 0;
      if (digitalRead(sensorPinA)) {
        sense_dir = 1;
      }
    }

    unsigned long t = millis();
    t = sin((double)t / 10000) * 8000 + 16000;
    
    
    for(int i=0;i<NUMPIXELS;i++){
      int pixnum = i;
      float brightness = 0.05;
    
     // if ( abs((sense_dir?i:(NUMPIXELS-i)) - time_sinse_sense / 50) < 3) {
        brightness = 0.8;
     // }
          
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      //pix.setPixelColor(i, digitalRead(sensorPinA)?20:(digitalRead(sensorPinB)?16384:0));
      pix.setPixelColor(NUMPIXELS-i, pix.Color(sint(t/3+pixnum*8)*brightness,sint(t/4+pixnum*8)*brightness,brightness*sint(t/5+pixnum*8)));
    } 
  
    pix.show();
  }
  
};

StairFlight dpixels = StairFlight(6, 8, 9);
StairFlight upixels = StairFlight(7, 10, 11);


void setup() {
}


void loop() {

  upixels.loop();
  dpixels.loop();
}

