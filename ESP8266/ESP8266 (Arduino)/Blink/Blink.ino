byte rx=3;
byte tx=1;
byte HB=2;

#include<Ticker.h>
Ticker HeartBeat;

/*
 * This program is using the Ticker library to do an asynchronous HeartBeat at 1 Hz
 */
 

void setup() { 
  //Enable the WatchDog Timer
  ESP.wdtEnable(20000);
  
  //Initialize our HeartBeat
  pinMode(HB,OUTPUT);
  HeartBeat.attach(.5,HeartBeat_ISR);
  
  //Say that we are booted
  Serial.begin(9600);
  Serial.println("\nBooted");
  
//  Serial.println(String("Flash: ")+ESP.getFlashChipSize);
//  Serial.println(String("FlashID: ")+ESP.getFlashChipId();
//  Serial.println(String("ChipID: ")+ESP.getChipId();
}

void loop() {
  //Just feed the WDT since the HeartBeat is done in the Ticker
  ESP.wdtFeed();
  delay(1000);
}

// Toggles the HeartBeat pin
void HeartBeat_ISR(){
  digitalWrite(HB,!digitalRead(HB));  
}
