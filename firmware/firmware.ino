// turntablePCB

#include <QTRSensors.h>
#include "table209.h"

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
uint16_t bitValue[SensorCount];
int prevOut;
int counter;
int channel;
int noOut;

int threshold = 1000;

void setup()
{
  Serial.begin(115200);

  pinMode(12, OUTPUT);// fake ground
  digitalWrite(12, LOW);//
  pinMode(10, OUTPUT);//cv
  pinMode(11, OUTPUT);//gate
  pinMode(A1, INPUT);//channel select
  
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) {
    9, 8, 7, 6, 5, 4, 3, 2
  }, SensorCount);
  qtr.setEmitterPin(255);//no emiiter control
}


void loop()
{
  // read raw qtr values
  qtr.read(sensorValues);
  int pot = analogRead(A1) ;

  //simple channle select, with zero as pass all
  if (pot == 0) {
    digitalWrite(11, 1);
  } else {
    channel = pot / 127;
    digitalWrite(11, bitValue[channel]);
  }

  //threshold for bits
  for (char i = 0; i < SensorCount; i++)
  {
    if (sensorValues[i] > threshold)
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
  int wantedpos;
  for (int i = 0; i < tableLength; i++) {
    if (noOut == table209[i]) {
      wantedpos = i;
      break;
    }
  }

  analogWrite(10, wantedpos);


  Serial.print(channel);
  Serial.print('\t');
  Serial.print(noOut);
  Serial.print('\t');
  Serial.print(wantedpos);
  Serial.println();


  /*
     //raw value table for QTR
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
