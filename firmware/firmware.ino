// turntablePCB

#include <QTRSensors.h>
#include "table209.h"

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
uint16_t bitValue[SensorCount];
int prevOut;
int counter;
void setup()
{
  pinMode(10, OUTPUT);
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) {
    9, 8, 7, 6, 5, 4, 3, 2
  }, SensorCount);
  qtr.setEmitterPin(255);

  Serial.begin(115200);
}


void loop()
{
  // read raw sensor values
  qtr.read(sensorValues);

  int noOut = 0;
  for (char i = 0; i < SensorCount; i++)
  {
    if (sensorValues[i] > 1000)
      bitValue[i] = 1;
    else
      bitValue[i] = 0;
  }

  //magic line of the interwebs !
  //convert binary '00001111' to int
  for (int ii = 0; ii < 8; ii++) {
    noOut = noOut | ((bitValue[ii] + '0') - '0') << (7 - ii);
  };

  //generate the table

  //if (noOut != prevOut) {
  Serial.print(noOut);
  Serial.print('\t');

  int wantedpos;
  for (int i = 0; i < tableLength; i++) {
    if (noOut == table209[i]) {
      wantedpos = i;
      break;
    }
  }

  Serial.print(wantedpos);
  analogWrite(10, wantedpos);
  Serial.print('\t');
  Serial.println();
  /*
    for (uint8_t i = 0; i < SensorCount; i++)
    {
    //Serial.print(sensorValues[i]);
    Serial.print(bitValue[i]);

    Serial.print('\t');
    }

    Serial.println();
    //}
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
