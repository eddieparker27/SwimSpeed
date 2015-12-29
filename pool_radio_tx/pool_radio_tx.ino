unsigned int SHORT_TIME = 350; /* Microsecs */
unsigned int LONG_TIME = 1050; /* Microsecs */
unsigned long DELAY_BETWEEN_MESSAGES = 43; /* Milliseconds */

void setup()
{
  Serial.begin(9600);
  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void transmitt( int pin,
                char* code,
                unsigned int short_time,
                unsigned int long_time)
{
  char *c = code;
  unsigned int first_time = 0;
  unsigned int second_time = 0;
  while(*c != 0)
  {
    if (*c == '1')
    {
      first_time = long_time;
      second_time = short_time;            
    }
    else
    {
      first_time = short_time;
      second_time = long_time;
    }
    //noInterrupts();
    digitalWrite(pin, HIGH);
    delayMicroseconds(first_time);
    digitalWrite(pin, LOW);
    delayMicroseconds(second_time);
    //interrupts();
    c++;
  }
}

char *slower =  "0111100010100001011010000101";
char *onoff =   "0111100010001001011010000101";

char *garbage = "0101010101010101010101010101";

void loop()
{
  delay(5000);
  Serial.print("Sending...");
  digitalWrite(11, HIGH);
  for(int i = 0; i < 40; i++)
  {
    delay(DELAY_BETWEEN_MESSAGES);
    transmitt(10, onoff, SHORT_TIME, LONG_TIME);
  }
  digitalWrite(11, LOW);
  Serial.println("...sent");
}
