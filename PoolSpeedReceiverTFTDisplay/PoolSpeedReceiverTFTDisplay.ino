/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>
#include <math.h>
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

// char array to print to the screen
char sensorPrintout1[4];
char sensorPrintout2[4];

/* Create an instance of RCSwitch */
RCSwitch mySwitch = RCSwitch();

unsigned long ms_at_last_msg = 0;
int intlevel = 0;

unsigned long ms_at_last_UI_update = 0;

char* sensorPrintout = sensorPrintout1;
char* sensorPrintoutPrev = sensorPrintout2;
char* temp;

void setup() 
{
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
  
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 255);
  
  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255,255,255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Pool Speed\n ",20,5);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(10);
  
  sensorPrintout1[ 0 ] = 0;
  sensorPrintout1[ 1 ] = 0;
}

int last_level = -1;


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
    
    if (intlevel != last_level)
    {    
      last_level = intlevel;
      String sensorVal = String(intlevel);
   
      // convert the reading to a char array
      sensorVal.toCharArray(sensorPrintout, 4);
      
      if (sensorPrintout[ 1 ] == 0)
      {        
        sensorPrintout[ 1 ] = sensorPrintout[ 0 ];
        sensorPrintout[ 0 ] = '0';
        sensorPrintout[ 2 ] = 0;
      }
        
    
      // erase the text you just wrote
      TFTscreen.stroke(0,0,255);
      TFTscreen.text(sensorPrintoutPrev, 25, 35);
    
      // set the font color
      TFTscreen.stroke(255,255,255);
      // print the sensor value
      TFTscreen.text(sensorPrintout, 25, 35);
      
      temp = sensorPrintout;
      sensorPrintout = sensorPrintoutPrev;
      sensorPrintoutPrev = temp;
    }
    
  } 
    
}
