#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_NeoPixel.h>
#define PIN            6
#define NUMPIXELS      400
int fadeRate = 0;
int MaxBrightness = 255;
int TotalSteps = 2;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
void displaySensorDetails(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup()
{
  strip.begin();
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  Serial.begin(9600);
  Serial.println("Gyroscope Test"); Serial.println("");
  gyro.enableAutoRange(true);
  if (!gyro.begin())
  {
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while (1);
  }
  displaySensorDetails();
}

void loop()
{
  sensors_event_t event;
  gyro.getEvent(&event);
  Serial.print("X: "); Serial.print(map(event.gyro.x, -10, 10, 0, 255)); Serial.print("  ");
  //  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("  ");
  setSection(0, 400 , map(event.gyro.x, 10, 0, 0, 255), map(event.gyro.y, 10, 0, 0, 20), map(event.gyro.z, -10, 10, 0, 255));

}

void setSection(int start, int finish, int Nred, int Ngreen, int Nblue) {
  uint32_t c = strip.getPixelColor(start);
  uint8_t  redCur = (c >> 16) & 0xFF;
  uint8_t  greenCur = (c >>  8) & 0xFF;
  uint8_t  blueCur = c & 0xFF;
  uint8_t redNew = Nred;
  uint8_t greenNew = Ngreen;
  uint8_t blueNew = Nblue;
  for (int i = 1; i < TotalSteps; i++)
  {
    uint8_t red = (((redCur * (TotalSteps - i)) + (redNew * i)) / TotalSteps);
    uint8_t green = (((greenCur * (TotalSteps - i)) + (greenNew * i)) / TotalSteps);
    uint8_t blue = (((blueCur * (TotalSteps - i)) + (blueNew * i)) / TotalSteps);
    for (int j = start; j < finish + 1; j+=3) {
      strip.setPixelColor(j, red, green, blue);
      }
    strip.show();
  }
}
