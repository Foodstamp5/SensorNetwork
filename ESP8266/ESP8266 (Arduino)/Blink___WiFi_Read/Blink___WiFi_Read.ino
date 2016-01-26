#include<ESP8266WiFi.h>
#include<Ticker.h>

//Define HeartBeat
#define HB 2
Ticker HB_tick;

WiFiServer server(80);
#define verbose false
  
// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(0,INPUT_PULLUP);
  
  HB_init();
  Serial.begin(9600);
  delay(1000);
  Serial.println("\nBooted");
  Serial.println("Initializing WiFi");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Nick-ESP");
  delay(1000);
  Serial.print("WiFi initalized (");
  Serial.print(WiFi.softAPIP());
  Serial.println(")");
  Serial.println("Initializing Server");
  server.begin();
  delay(1000);
  Serial.println("Server initalized. Waiting for clients to connect");
}

// the loop function runs over and over again forever
void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    if(verbose) Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if(verbose) Serial.write(c);
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
          client.print("Run/~Flash is set to ");
          if(digitalRead(0)) client.print("Run (HIGH)");
          else client.print("Flash (LOW)");
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
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
    if(verbose) Serial.println("client disonnected");
  }
}

//Initialize HeartBeat pin as an output and set to toggle at 0.5 Hz
void HB_init(){
  pinMode(HB, OUTPUT);
  HB_tick.attach(0.25,HB_CB);
}

void HB_CB(){
  digitalWrite(HB,!digitalRead(HB));
}
