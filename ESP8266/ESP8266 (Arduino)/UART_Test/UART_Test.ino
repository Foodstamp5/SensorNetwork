int incomingByte = 0;   // for incoming serial data
//const byte size=sizeof(pins)/sizeof(*pins);

byte HB=2;
#include<Ticker.h>
Ticker HeartBeat;

#include<stdint.h>

byte pin=0;

byte rx=3;
byte tx=1;

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial.setDebugOutput(true);
  pinMode(HB,OUTPUT);
  HeartBeat.attach(.5,HeartBeat_ISR);
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
  Serial.println("\nBooted");
  delay(1000);
//  Serial.end();//jk
}

void loop() {
  Serial.print('\n');
  for(int i=0;i<80;i++) Serial.print('-');
  Serial.print(String("\n")+Serial.available()+" Bytes in buffer");
  Serial.print(String(", ")+ESP.getFreeHeap()+" Bytes of RAM free");
  delay(1000);
}


// Toggles the HeartBeat pin
void HeartBeat_ISR(){
  digitalWrite(HB,!digitalRead(HB));  
}
