// turntablePCB

#include <QTRSensors.h>

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
int prevOut;

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
    if (sensorValues[i] > 500)
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
  analogWrite(10, noOut);
  if (noOut != prevOut) {
    Serial.print(noOut);
    Serial.print('\t');

    for (uint8_t i = 0; i < SensorCount; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }

    Serial.println();
  }
  prevOut = noOut;

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
