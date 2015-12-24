/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>
#include <math.h>

RCSwitch mySwitch = RCSwitch();

unsigned long ms_at_last_msg = 0;
int intlevel = 0;

unsigned long ms_at_last_UI_update = 0;

void setup() 
{
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}


void loop() 
{
  unsigned long ms = millis();
  
  if (mySwitch.available()) 
  {
    
    unsigned long value = mySwitch.getReceivedValue();
    
    if (value == 0) 
    {
      Serial.print("Unknown encoding");
    } 
    
    else 
    {
      if ((mySwitch.getReceivedProtocol() == 2) && (mySwitch.getReceivedBitlength() == 14))
      {
        int bitcount = (value & 0XFC00) >> 10;
        int sensor = (value & 0x3FF);
        int shifter = sensor;
        int counter = 10;
        while(shifter != 0)
        {
           counter -= (shifter & 0x01);
           shifter >>= 1;
        }
        
        if (bitcount == counter)
        {
          // Record time of last valid message
          ms_at_last_msg = millis();
          
          
          float volts = 5.0 * sensor / 1023.0;
          //Serial.print("Volts = ");
          //Serial.println(volts);
          
          if (volts < 1.25)
          {
            volts = 1.25;
          }
          if (volts > 3.05)
          {
            volts = 3.05;
          }
          float level = (volts - 1.25) * 30.0;
          intlevel = round(level);
          
                    
        }
        else
        {
          Serial.print(sensor);
          Serial.print(" : ");
          Serial.print(bitcount);
          Serial.print(" : ");
          Serial.println(counter);
        }
      }
    }
    
    

    mySwitch.resetAvailable();
  }
  else
  {
    unsigned long ms_since_last_msg = ms - ms_at_last_msg;
    
    if (ms_since_last_msg > 10000)
    {
      intlevel = 0;      
    }
  }
  
  unsigned long ms_since_last_UI_update = ms - ms_at_last_UI_update;
  
  if (ms_since_last_UI_update > 1000)
  {
    ms_at_last_UI_update = ms;
    Serial.print("Level = ");
    Serial.println(intlevel);
  } 
    
}
