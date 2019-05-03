#include <Adafruit_NeoPixel.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel dpixels = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_RGB + NEO_KHZ800);

void setup() {
  dpixels.begin();
  for(int i=0;i<NUMPIXELS;i++){
    dpixels.setPixelColor(i, 0);
  }
  while (millis() < 2000)  
    dpixels.show();
  pinMode(22, OUTPUT);     
  pinMode(24, INPUT);     
  digitalWrite(22, LOW);   // turn the LED on (HIGH is the voltage level)
}

#define sint(X) dpixels.gamma8(dpixels.sine8(X))

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  unsigned long t = millis();
  t = sin((double)t / 8000) * 8000 + 16000;
  
  float brightness = 1.0;

  for(int i=0;i<NUMPIXELS;i++){
    int pixnum = i;
    
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    dpixels.setPixelColor(i   , dpixels.Color(sint(t/3+pixnum*8)*brightness,sint(t/4+pixnum*8)*brightness,brightness*sint(t/5+pixnum*8))); 
  } 

  dpixels.show();
}

