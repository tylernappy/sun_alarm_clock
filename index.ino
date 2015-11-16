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
unsigned long sunriseMilliseconds;

int redMax = 255;
int greenMax = 204;
int blueMax = 0;

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
    sunriseMilliseconds = command.toInt();
    timeBegin = millis();
    timeUntilFinish = timeBegin + sunriseMilliseconds;
    t = timeBegin;
    while (t <= timeUntilFinish) {
        int red = map(t, 0, sunriseMilliseconds, 0, redMax);
        int green = map(t, 0, sunriseMilliseconds, 0, greenMax);
        int blue = map(t, 0, sunriseMilliseconds, 0, blueMax);
        for(int i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(red, green, blue));
        }
        strip.show();
        t = millis();
        Serial.print("Time: ");
        Serial.println(t);
        Serial.print("rgb: ");
        Serial.println(red);
        Serial.println(green);
        Serial.println(blue);
        Spark.process();
    }
    return 1;
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
        strip.setPixelColor(i, strip.Color(redMax, greenMax, blueMax));
    }
    strip.show();
    delay(2000);
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(2000);
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(redMax, greenMax, blueMax));
    }
    strip.show();
    delay(2000);
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(30);
}
