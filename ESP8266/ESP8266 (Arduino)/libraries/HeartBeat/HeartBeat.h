#ifndef HEARTBEAT_H
#define HEARTBEAT_H


#include <Arduino.h>
#include <Ticker.h>

/*
 * Makes the pin toggle every so many seconds
 */

class HeartBeat{
 public:
  HeartBeat(byte pin, float seconds=0.5);
  
  void toggle();
  
 private:
  Ticker tick;
  byte _pin;
  float _seconds;
};


// GLOBAL_HEARTBEAT points to the last created heartbeat
HeartBeat* GLOBAL_HEARTBEAT=NULL;
void GlobalHeartBeat(){
	GLOBAL_HEARTBEAT->toggle();
}

HeartBeat::HeartBeat(byte pin, float seconds){
  if(GLOBAL_HEARTBEAT) delete GLOBAL_HEARTBEAT;
  GLOBAL_HEARTBEAT=this;
  _pin=pin;
  _seconds=seconds;
  pinMode(_pin,OUTPUT);
  tick.attach(_seconds,GlobalHeartBeat);
} 

void HeartBeat::toggle(){
  digitalWrite(_pin,!digitalRead(_pin));
}

#endif