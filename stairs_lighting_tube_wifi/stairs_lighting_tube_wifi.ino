#include <Adafruit_NeoPixel.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      94

#define sint(X) pix.gamma8(pix.sine8(X))

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;



class StairFlight {
  char sensorPinA, sensorPinB;
  long int sensetime;
  char sense_dir;
  long ms_;
  Adafruit_NeoPixel pix;
  
  public:
  StairFlight(char pixPin, char bottomPin, char topPin):
    pix(NUMPIXELS, pixPin, NEO_RGB + NEO_KHZ800) {
    // Sensor setup
  //  pinMode(bottomPin, INPUT);
  //  pinMode(topPin, INPUT);
    sensorPinA = topPin;
    sensorPinB = bottomPin;
    sensetime = 0;
    ms_ = 0;
    
    pix.begin();
  }
  
  void loop() {
    long int time_sinse_sense = millis() - sensetime;
    
   /* if ((digitalRead(sensorPinA) || digitalRead(sensorPinB)) && time_sinse_sense > 2000) {
      sensetime = millis();
      sense_dir = 0;
      if (digitalRead(sensorPinA)) {
        sense_dir = 1;
      }
    } */

    unsigned long t = millis();
    t = sin((double)t / 10000) * 8000 + 16000;
    
    
    for(int i=0;i<NUMPIXELS;i++){
      int pixnum = i;
      float brightness = 0.05;
    
     // if ( abs((sense_dir?i:(NUMPIXELS-i)) - time_sinse_sense / 50) < 3) {
        brightness = 0.2;
     // }
          
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      //pix.setPixelColor(i, digitalRead(sensorPinA)?20:(digitalRead(sensorPinB)?16384:0));
      if (i > 25) { //ms_/2000) {
        pix.setPixelColor(NUMPIXELS-i-1, pix.Color(sint(t/3+pixnum*8)*brightness,sint(t/4+pixnum*8)*brightness,brightness*sint(t/5+pixnum*8)));
      } else {
        pix.setPixelColor(NUMPIXELS-i-1, pix.Color(128,0,128));        
      }
    } 
  
    pix.show();
  }

  void set_time(long ms) {
    ms_ = ms;
  }
};

StairFlight dpixels = StairFlight(2, 8, 9);
StairFlight upixels = StairFlight(0, 10, 11);


void setup() {
    Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("eightlegs", "ichiwichinichi");

}

long last_refresh=0;
long time_to_arrival=0;


void loop() {
  // wait for WiFi connection
  
  if ((WiFiMulti.run() == WL_CONNECTED && millis()-last_refresh > 10000 )) {
    last_refresh = millis();
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin("http://www.omattos.com/tube/time.php")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          
          time_to_arrival = (long)(payload.toInt()) * 1000;
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  dpixels.set_time(time_to_arrival- millis() + last_refresh);
  //upixels.loop();
  dpixels.loop();
    
}

