#include "Adafruit_BMP3XX.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define LED_PIN 3
#define HOTWIRE_PIN 5

void cutdown();

Adafruit_BMP3XX bmp;
uint32_t alt[10];
uint32_t init_pressure;
uint8_t counter = 0;
bool cut=0;

void setup() {
  uint8_t x = 0;
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Master Cutdown -- Two Month 2019");

  // set both pins as output
  pinMode(LED_PIN, OUTPUT);
  pinMode(HOTWIRE_PIN, OUTPUT);

  while(!bmp.begin() && x++ < 10);
  if(x == 10) while(1); // don't turn on anything if we can't boot the bmp

  // setup the LED at 50% duty cycle

  // take 50 readings over half a second and use the average as inital pressure
  init_pressure = 0;
  for(int i = 0; i < 50; i++) {
    bmp.performReading();
    init_pressure += bmp.readPressure();
    delay(10);
  }
  init_pressure /= 50;

  // fill up buffer with readings so we can use that in averaging
  for(int i = 0; i < 10; i++) {
    bmp.performReading();
    alt[i] = bmp.readAltitude(init_pressure);
    delay(10);
  }
}

void loop() {
  uint32_t average = 0;
  bmp.performReading();
  alt[counter%10] = bmp.readAltitude(init_pressure);

  for(int i=0; i<10; i++) {
    average += alt[i];
  }
  average /= 10;

  if(average >= 600 && !cut){
    cutdown();
  }
  counter++;

  delay(10);
}

void cutdown(){
  analogWrite(LED_PIN, 255/10);

  digitalWrite(HOTWIRE_PIN, HIGH);
  delay(3000);
  digitalWrite(HOTWIRE_PIN, LOW);

  cut = 1;
}