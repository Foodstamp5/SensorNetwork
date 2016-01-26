/*
** Example Arduino sketch for SainSmart I2C LCD2004 adapter for HD44780 LCD screens
** Readily found on eBay or http://www.sainsmart.com/ 
** The LCD2004 module appears to be identical to one marketed by YwRobot
**
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)
**
** sain_lcd_2.ino
** Simplified and modified by Andrew Scott for Arudino 1.0.1, Arudino Uno R3.
** NOTE: I2C pins are A4 SDA, A5 SCL
** Don't forget to use the new LiquidCrystal Library.
*/

//Signify that we are using an ESP
//Needed for LiquidCrystal_I2C
#define _ESP_

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27  // Define I2C Address where the SainSmart LCD is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int time;

void setup(){
  byte sda=0;
  byte scl=2;
  
  lcd_init(sda,scl);
}

void loop(){
  command(0x01); //Clear, go home
  delay(2); //Wait for screen to clear

  //while(true);
  char c='a';
  for(int i=0;i<16;i++){
    write(c);
    c++;
    delay(100);
  }
  move(0x40);
  for(int i=0;i<16;i++){
    write(c);
    c++;
    delay(100);
  }
  for(int i=0;i<16;i++){
    if(i%2) command(0x1B); //Shift left
    else command(0x1F); //Shift right
    delay(1000);
  }
}

