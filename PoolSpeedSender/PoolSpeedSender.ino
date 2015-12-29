/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int sensorPin = A0;    // select the input pin for the potentiometer

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(20);
  
}

long NUM_SAMPLES = (long)20000;

void loop() 
{
  // read the value from the sensor:
  long avgSensorValue = 0;
  for(long i = 0; i < NUM_SAMPLES; i++)
  {
    avgSensorValue += analogRead(sensorPin);
  }
  avgSensorValue /= NUM_SAMPLES;
 
  int shifter = avgSensorValue;
  int counter = 10;
  while(shifter != 0)
  {
     counter -= (shifter & 0x01);
     shifter >>= 1;
  }
  unsigned long sendVal = counter;
  sendVal <<= 10;
  sendVal |= avgSensorValue;
  
  Serial.print("avgSensorValue...");
  Serial.println(avgSensorValue);
  
  Serial.print("Sending...");
  Serial.println(sendVal);
  
  mySwitch.send(sendVal, 14);
}
