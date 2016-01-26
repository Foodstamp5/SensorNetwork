byte HB=3;
#include<Ticker.h>
Ticker HeartBeat;
// Toggles the HeartBeat pin
void HeartBeat_ISR(){
  digitalWrite(HB,!digitalRead(HB));  
}

#include <Wire.h>
#include <I2C_8bit_Register.h>
I2C_8bit_Register io(0x27);

void setup()
{
  pinMode(HB,OUTPUT);
  HeartBeat.attach(.5,HeartBeat_ISR);
  Wire.begin(0,2); // join i2c bus (address optional for master)
}

bool state = 0;

void loop()
{
  io.write(0xff);
  delay(500);
  io.write(0x00);
  delay(500);
}
