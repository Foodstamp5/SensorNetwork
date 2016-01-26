/*
 * This code blinks the leds in pins[] left and right in a kit pattern.
 * It serves as a great hello world
 */

byte pins[]={0,2,1,3};
const byte size=sizeof(pins)/sizeof(*pins);

// The LEDs on the board are active low!
const bool on=0;
const bool off=1;

// How long to wait between frames
int time=100; //ms

// the setup function runs once when you press reset or power the board
void setup() {

  // Make each pin an output
  for(int i=0;i<size;i++) pinMode(pins[i],OUTPUT);
  
  //Light up only the first LED
  digitalWrite(pins[0],on);
  for(int i=1;i<size;i++) digitalWrite(pins[i],off);

  // Wait before the next frame
  delay(time);
}

// the loop function runs over and over again forever
// note that all LEDs are active low
void loop() {
  
  //go to the right
  for(int i=0;i<size-1;i++){
    digitalWrite(pins[i],off);
    digitalWrite(pins[i+1],on);
    delay(time);
  }
  
  //go to the left
  for(int i=size-1;i>0;i--){
    digitalWrite(pins[i],off);
    digitalWrite(pins[i-1],on);
    delay(time);
  }
}

