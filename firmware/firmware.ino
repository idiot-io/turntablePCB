#include <QTRSensors.h>

// prints the sensor values to the serial
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).


#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2

// sensors 0 through 7 are connected to digital pins 2 through 9, respectively
QTRSensorsRC qtrrc((unsigned char[]) {
  2, 3, 4, 5, 6, 7, 8, 9
},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];


void setup()
{
  Serial.begin(115200); 
  delay(100);
}


void loop()
{
  // read raw sensor values
  qtrrc.read(sensorValues);

  for (char i = 0; i < NUM_SENSORS; i++)
  {
    if (sensorValues[i] > 100) {
      Serial.print("i");
    }else{
      Serial.print("o");
    }
  }
  Serial.println();

  delay(5);
}