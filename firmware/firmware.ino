// turntablePCB

// based on QTRSrawValues example code
#include <QTRSensors.h>

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2

// sensors 0 through 7 are connected to digital pins 2 through 9, respectively
QTRSensorsRC qtrrc((unsigned char[]) {
  2, 3, 4, 5, 6, 7, 8, 9
},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

#include "table.h"
int prevOut;

void setup()
{
  Serial.begin(115200);
  delay(100);

}


void loop()
{
  // read raw sensor values
  qtrrc.read(sensorValues);
  int noOut = 0;
  for (char i = 0; i < NUM_SENSORS; i++)
  {
    if (sensorValues[i] > 100)
      sensorValues[i] = 1;
    else
      sensorValues[i] = 0;
  }

  //magic line of the interwebs !
  //convert binary '00001111' to int
  for (int ii = 0; ii < 8; ii++) {
    noOut = noOut | ((sensorValues[ii] + '0') - '0') << (7 - ii);
  };

  //generate the table

  if (noOut != prevOut) {
    if (noOut == 0) {
      Serial.println("start");
      Serial.println("0");
    } else {
      Serial.println(noOut);
    }
  }
  prevOut = noOut;
  /*
    Serial.print(noOut);
    Serial.print("\t");
    Serial.print(grayTable[noOut]);
    Serial.println();
  */
  delay(5);
}

/*
   This function converts an unsigned binary
   number to reflected binary Gray code.

   The operator >> is shift right. The operator ^ is exclusive or.
*/
unsigned int BinaryToGray(unsigned int num)
{
  return num ^ (num >> 1);
}

/*
   This function converts a reflected binary
   Gray code number to a binary number.
   Each Gray code bit is exclusive-ored with all
   more significant bits.
*/
unsigned int GrayToBinary(unsigned int num)
{
  unsigned int mask = num >> 1;
  while (mask != 0)
  {
    num = num ^ mask;
    mask = mask >> 1;
  }
  return num;
}
