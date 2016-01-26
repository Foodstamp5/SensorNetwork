class lcd{
 public:
  void init(byte sda, byte scl, byte addr);
  void digitalWrite(byte pin, bool state);
  
 private:
  byte data=0;
};

void lcd::init(byte sda, byte scl, byte addr){
  Wire.begin(sda,scl);
  
  //set i2c pins as outputs
  
  digitalWrite(E,LOW);
  delay(16); //Wait>15ms
  //Later, replace delay by reading busy flag?
  send_nibble(0x3,LOW); //Wakeup
  delay(16); //Wait>5ms
  send_nibble(0x3,LOW); //Wakeup
  delay(16); //Wait>160us
  send_nibble(0x3,LOW); //Wakeup
  delay(16); //Wait>160us
  
  send_nibble(0x2,LOW); //Set 4-bit
  command(0x28); //Set 4-bit, 2 line, 5x8 chars
  command(0x10); //Set cursor
  command(0x0f); //Display on, cursor blinking
  command(0x06); //Increment Cursor to the right, no display scrolling
}

void lcd::digitalWrite(byte pin, bool state){
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(data);
  Wire.endTransmission();
}

char lcd::read_pos(){
  digitalWrite(RS,LOW); //Low=CMD
  digitalWrite(RW,HIGH); //High=Write
  digitalWrite(E,HIGH); //Send pulse
  delay(1);             //Wait >300ns
  digitalWrite(E,LOW);
  DDRD=0x00; // Data is input
  char temp=Data;
  DDRD=0xFF; // Data is output
  return temp;
}

void move(char in){
  command(in|0b10000000);
}

void command(char in){
  send_nibble(in>>4,LOW);     //Send Low nibble
  delay(1);
  send_nibble(in,LOW);   //Send High nibble
}

void write(char in){
  send_nibble(in>>4,HIGH);     //Send Low nibble
  delay(1);
  send_nibble(in,HIGH);  //Send High nibble
}

void send_nibble(char in, char isData){
  //Note that digitalWrite is slow enough that we don't have to wait between nibbles. Must wait 37 us for most commands, 1.52 ms for clear and cursor reset commands
  Data=(in<<4)&0xF0;
  digitalWrite(RS,isData); //High=Data
  digitalWrite(RW,LOW);  //Low=Write
  digitalWrite(E,HIGH);  //Send pulse
  delay(1);              //Wait >300ns
  digitalWrite(E,LOW);
}
