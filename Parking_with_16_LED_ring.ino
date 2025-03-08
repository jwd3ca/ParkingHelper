// NOTE: use Arduino Pro or Pro Mini
// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// ---------------------------------------------------------------- //

#include <FastLED.h>

#define NUM_LEDS 16      /* The amount of pixels/leds you have */
#define DATA_PIN 4       /* The pin the ring's  data line is connected to (uses D1 not D0 */
#define LED_TYPE WS2812B /* I assume you have WS2812B leds, if not just change it to whatever you have */
#define BRIGHTNESS 4     /* Control the brightness of  leds */
#define SATURATION 255   /* Control the saturation of leds */
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

#define RED CRGB(0xff0000) 
#define GREEN CRGB(0x008800) 
#define BLUE CRGB(0x0000ff) 
#define YELLOW CRGB(0xffff00) 

// defines variables
long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement
int DEBUG = 1;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(115200); // // Serial Communication is starting with 9600 of baudrate speed
  DEBUG && Serial.println("Ultrasonic Sensor HC-SR04i "); // print some text in Serial Monitor
  DEBUG && Serial.println("with Arduino Pro Mini");
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  //FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
}
  
//           |   |    |
//     <- RED| G | Y  |-> BLU 
//           |   |    |
//          MIN GOOD MAX

#define RANGE_MIN  95  
#define RANGE_GOOD 114
#define RANGE_MAX  160

//==========================================================
void loop() {
  distance = calculate_average();
  
 
  DEBUG && Serial.print("Distance: ");
  DEBUG && Serial.print(distance);
  DEBUG && Serial.print(" cm:  ");
  delay(30);
  
  if (distance <= RANGE_MIN) {
    DEBUG && Serial.println("too close");
    ring(RED,0,15);
  }
  
  else if (distance >=  RANGE_MIN && distance < RANGE_GOOD) {
    DEBUG && Serial.println("PERFECT");
    ring(GREEN,0,15);
  } 
  
  else if (distance >=  RANGE_GOOD && distance < RANGE_MAX) {
    DEBUG && Serial.println("warning");
    ring(YELLOW,0,15);
  }

  else if (distance > RANGE_MAX) {
    DEBUG && Serial.println("too far");
    ring(BLUE,0,15);
  }
  delay(100);
}  

//===========================================================
// This function takes 5 measurements and returns the average.
int calculate_average () {
  int sum = 0; int average = 0; int num = 5;

  // Read the echoPin, return the sound wave travel time in microseconds
  for (int i=0; i<num; i++) {
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    sum += (duration * 0.034 / 2);
    delay(10);
  }
  average = sum/num;
  return (average);
}

//===================================================
// function to set ring colour
void ring(CRGB hexcolour, int s, int e) {
  DEBUG && Serial.print("in ring() with ");
  // DEBUG && Serial.println(hexcolour);
  for (int i = s; i <= e; i++) {
    leds[i] = hexcolour; 
    FastLED.show(); 
    delay(5);
  }
}
