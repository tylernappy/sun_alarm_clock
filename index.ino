#include <string.h>
#include "application.h"
// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D6
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

unsigned long t;
unsigned long timeUntilFinish;
unsigned long timeBegin;
unsigned long sunrise;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting up...");

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    initialize();

    Spark.function("startAlarm", initiateAlarm);
    Spark.function("turnOff", initiateTurnOff);
}

void loop() {
    delay(30);
}

int initiateAlarm(String command) {
    sunrise = command.toInt();
    lightClock(sunrise);
    return 1;
}

void lightClock(unsigned long sunriseMilliseconds) {
    timeBegin = millis();
    timeUntilFinish = timeBegin + sunriseMilliseconds;
    t = timeBegin;
    while (t <= timeUntilFinish) {
        int brightness = map(t, 0, sunriseMilliseconds, 0, 255);
        for(int i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));
        }
        strip.show();
        t = millis();
        Serial.print("Time: ");
        Serial.println(t);
        Serial.print("Brightness: ");
        Serial.println(brightness);
        Spark.process();
    }
}

int initiateTurnOff(String command) {
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
    return 1;
}

void initialize() {
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    delay(2000);
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(2000);
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    delay(2000);
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(30);
}
