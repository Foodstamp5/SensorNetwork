#include <Ticker.h>
#include <HeartBeat.h>
#include <SPI_Slave.h>

#define tx 1
#define rx 3

const byte clk=2;
const byte data=rx;
const byte enable=0;
const byte hb=tx;

HeartBeat HB(hb);

SPI_Slave spi(data,clk,enable);

/*
 * Notes
 * 
 * Send MSB first
 * Have data ready on rising edge of clock
 */

#include<ESP8266WiFi.h>
WiFiServer server(80);

void AP_Setup(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Nick-ESP");
  delay(1000);
  server.begin();
  delay(1000);
}

String byte2str(byte in, bool nibble_space=false){
  String result="0b";
  for(int i=0;i<8;i++){
    result+=in&(1<<(7-i));
    if(i==3&&nibble_space) result+=' ';
  }
  return result;
}

void check_for_client(){
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 1 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("SPI data: '");
          client.print(char(spi.data));
          client.print("' (");
          client.print(byte2str(spi.data,true));
          client.println(")");
          client.println("<br />");       
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
      yield();
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
  }
}

void setup() {
  AP_Setup();
}

void loop() {
//  check_for_client();
  digitalRead(0);
  delay(1000);
}
