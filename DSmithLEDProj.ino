/* RESOURCES

 Finding array length
 ---------------------
https://www.digitalocean.com/community/tutorials/find-array-length-in-c-plus-plus

Distance function (adjusted)
---------------------
https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41/circuit-3b-distance-sensor

Piano frequencies
---------------------
https://pages.mtu.edu/~suits/notefreqs.html

*/

/* CHART OF FREQUENCIES FOR NOTES IN C MAJOR
  Note      Frequency (Hz)

  C        262
  D        294
  E        330
  F        349
  G        392
  A        440
  B        494
*/

#include <Adafruit_NeoPixel.h>


// PIN CONFIG
const int TRIG_PIN = 11;    //connects to the trigger pin on the distance sensor
const int ECHO_PIN = 12;    //connects to the echo pin on the distance sensor
const int BUZZER_PIN = 13;  //connects to the buzzer pin
const int LED_PIN = 10;     //connects to the led strip pin
const int NUMPIX = 16;


const int MAXDIST = 7;     //Used to check if the sensor goes over the freq[] limit
const int DELAYVAL = 500;  //Interval Delay

//LED Display Pins
const int DISPLAY_PINS[] = { 2, 3, 4, 5, 6, 7, 8, 9 }; 

// C Major frequencies
//                      C    D    E    F    G    A    B
const int FREQ_C[] = { 262, 294, 330, 349, 392, 440, 493 };


// Note played dependant on array position (note[dist][i])
const int NOTES[7][7] = {
  { 4, 5, 6, 8 },             // C
  { 3, 4, 5, 6, 8, 9 },       // D
  { 4, 5, 6, 7, 8 },          // E
  { 5, 6, 7, 8 },             // F
  { 3, 4, 5, 6, 7, 8 },       // G
  { 3, 5, 6, 7, 8, 9 },       // A
  { 3, 4, 5, 6, 7, 8, 9 },    // B
};


int distance = 0;  //stores the distance measured by the distance sensor
int echoTime;


Adafruit_NeoPixel pixels(NUMPIX, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  setDistanceSensor();
  Serial.begin(9600);  //set up a serial connection with the computer
  pixels.begin();
  setLED();
}

void loop() {
  getDistance();
}

//------------------FUNCTIONS-------------------------------

void setDistanceSensor() {
  pinMode(TRIG_PIN, OUTPUT);  //the trigger pin will output pulses of electricity
  pinMode(ECHO_PIN, INPUT);   //the echo pin will measure the duration of pulses coming back from the distance sensor
}


// setLED/ledSetup just for show
void setLED() {
  for (int i = 0; i < MAXDIST; i++) {
    ledSetup(i);
  }
  for (int j = MAXDIST; j >= 0; j--) {
    ledSetup(j);
  }
}

void ledSetup(int num) {
  pixels.setPixelColor(num, pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
  pixels.show();
  stop();
}

//RETURNS THE DISTANCE MEASURED BY THE SENSOR
int getDistance() {
  //send out an ultrasonic pulse that's x ms long
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  //use the pulse in command to see how long it takes for the pulse to bounce back to the sensor
  echoTime = pulseIn(ECHO_PIN, HIGH);

  //calculate the distance of the object that reflected the pulse
  distance = echoTime / 148.0;

  distance <= MAXDIST ? buzz(distance) : stop();
}


void buzz(int dist) {
  tone(BUZZER_PIN, FREQ_C[dist - 1]);
  colors(dist);
  noteDisplay(dist);
  stop();
}

void noteDisplay(int dist) {
  //sizeof finds the length of an array and returns the value in bytes 
 // (arr[] = {0, 1, 2, 3, 4} = 5 slots, each int is 4 bytes so 5*4 = 20 bytes)
 // 20 / the size on an int (4) = 5 So we get a length of 5
  //
  int subArrayLength = sizeof(NOTES[dist]) / sizeof(int);
  for (int i = 0; i < subArrayLength; i++) {
    digitalWrite(NOTES[dist - 1][i], HIGH);
  }
}

void clearDisplay() {
  // Turn off all segments of the display
  for (int i = 0; i < sizeof(DISPLAY_PINS) / sizeof(int); i++) {
    digitalWrite(DISPLAY_PINS[i], LOW);
  }
}

void colors(int num) {
  //if argument is == maxdistance (7)
  //Runs as normal
  //else we use num-1 to get the value not including last array index
  //dist needs to be -1 to properly get indexes
  if (num == MAXDIST) {
    for (int i = 0; i < NUMPIX; i++) {
      pixels.setPixelColor(i, pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
    }
  } else {
    pixels.setPixelColor((num - 1), pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
  }
  pixels.show();
}

void stop() {
  delay(500);
  noTone(BUZZER_PIN);
  clearDisplay();
  pixels.clear();
  pixels.show();
}