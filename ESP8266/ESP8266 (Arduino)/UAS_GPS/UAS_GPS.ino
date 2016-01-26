//Define tx,rx pins
byte rx=3;
byte tx=1;

//Define HeartBeat pin
byte HB=tx;

#include<Wire.h>

//Define I2C pins
byte sda=0;
byte scl=2;

#include<Ticker.h>

/*
 * This program is using the Ticker library to do an asynchronous HeartBeat at 1 Hz
 */
 
Ticker HeartBeat;

void setup() { 
  //Enable the WatchDog Timer
  ESP.wdtEnable(20000);
  
  //Initialize our HeartBeat
  pinMode(HB,OUTPUT);
  HeartBeat.attach(.5,HeartBeat_ISR);
  
  //Initialize I2C pins
  pinMode(SDA,OUTPUT);
  pinMode(SCL,OUTPUT);
  Wire.begin(sda,scl);
  
  //Say that we are booted
//  Serial.begin(9600);
//  Serial.println("\nBooted");
  
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
