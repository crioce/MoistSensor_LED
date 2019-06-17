#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>
#define LED_PIN D2
#define LED_COUNT 12
#define SensorPin D1

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int moistCount = 0;
float sensorSum = 0;
float moisture_percentage = 0;
float sensorAverage = 0;
long maxHumid = 1023;
int lastPixelLit = 0;
int noWarning = 0;
int noDanger = 0;
int displayUpdateInterval = 100000;
int loopCount = 0;
long colorOk = strip.Color(0, 255, 0);

void setup()
{
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  strip.fill(colorOk, 0, LED_COUNT);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SensorPin, INPUT);
}

void updateDisplay(float moistValue)
{
  strip.clear();
  
  long colorWarning = strip.Color(255, 165, 0);
  long colorDanger = strip.Color(255, 0, 0);
  float humidPerPixel = (float)maxHumid / (float)LED_COUNT;
  int pixelsLit = (float)moistValue / (float)humidPerPixel;
  
  strip.fill(colorOk, 0, LED_COUNT);

  if (pixelsLit < 1) pixelsLit = 1;

  if (moistValue >= 100)
  {
    noDanger = 0;
    noWarning = 0;
    //strip.fill(colorOk, 0, LED_COUNT);
  }
  else if (moistValue >= 20)
  {
    //strip.fill(colorWarning, 0, pixelsLit);
    noWarning = noWarning + 1;
    if (noWarning > LED_COUNT){
      noWarning = LED_COUNT;
    }
  }
  else 
  {
    //strip.fill(colorDanger, 0, pixelsLit);
    noDanger++;
    if(noDanger > LED_COUNT )
    {
      noDanger = LED_COUNT;
    }
  }
 if(noWarning > 0){
    strip.fill(colorWarning, noDanger, noWarning +noDanger); 
  }
  if(noDanger > 0){
    strip.fill(colorDanger, 0, noDanger); 
  }
  

 Serial.println("---");
  // Serial.println(humidPerPixel);
  // Serial.println(pixelsLit);
 Serial.println(moistValue);
  // Serial.println(millis());
  // delay(500);
  
  strip.show();
  lastPixelLit = pixelsLit;
}

void updateMoist()
{
  // float moisture_percentage;
  sensorSum = sensorSum + analogRead(SensorPin);
  moistCount++;
  if (moistCount >= 100)
  {
    sensorAverage = sensorSum / 100.0;
    moistCount = 0;
    sensorSum = 0;

    moisture_percentage = (sensorAverage / maxHumid) * 100.00 ;

  // Serial.print("Soil Moisture(in Percentage) = ");
// Serial.print(moisture_percentage);
  // Serial.println("%");
  }
}



void loop()
{

  updateMoist();

  if(loopCount >= displayUpdateInterval)
  {
    loopCount = 0;
    updateDisplay(moisture_percentage);
  }

  loopCount++;

}

// if sensorValue < 1000
