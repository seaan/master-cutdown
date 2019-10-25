#include "Adafruit_BMP3XX.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define LED_PIN 3
#define HOTWIRE_PIN 5

void cutdown();

Adafruit_BMP3XX bmp;
uint32_t alt[10];
uint32_t current_average, init_pressure;
uint32_t counter = 0;
bool cut=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Master Cutdown -- Two Month 2019");

  // set both pins as output
  pinMode(LED_PIN, OUTPUT);
  pinMode(HOTWIRE_PIN, OUTPUT);

  // setup the LED at 50% duty cycle
  if(bmp.begin()){
    analogWrite(LED_PIN, 255/50);
  }

  // take 50 readings over half a second and use the average as inital pressure
  init_pressure = 0;
  for(int i = 0; i < 50; i++) {
    bmp.performReading();
    init_pressure += bmp.readPressure();
    delay(10);
  }
  init_pressure /= 50;
}

void loop() {
  // put your main code here, to run repeatedly:
  bmp.performReading();

  alt[counter%10] = bmp.readAltitude(init_pressure);
  for(int i=0; i<counter; i++){
    current_average += alt[i];
  }
  current_average /= 10;

  if(current_average - initial_altitude >= 600 && !cut){
    cutdown();
  }
  counter++;
}

void cutdown(){
  analogWrite(LED_PIN, 255/10);

  digitalWrite(HOTWIRE_PIN, HIGH);
  delay(3000);
  digitalWrite(HOTWIRE_PIN, LOW);

  cut = 1;
}