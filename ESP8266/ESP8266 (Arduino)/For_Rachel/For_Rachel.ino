#include<ESP8266WiFi.h>
#include<Ticker.h>

//Define HeartBeat
#define HB 2
Ticker HB_tick;

// This creates a webserver
WiFiServer server(80);

// Make this false if you don't want to see the debug messages (You will want to do this when you attach it to your project
#define verbose true
  
// the setup function runs once when you press reset or power on the board
void setup() {
  pinMode(0,INPUT_PULLUP);

  // Makes the heartbeat pin flash at 0.5 Hz
  HB_init();
  Serial.begin(9600);
  delay(1000);
  Serial.println("\nBooted");
  if(verbose) Serial.println("Initializing WiFi");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("A-Super-Cool-Project");
  delay(1000);
  if(verbose) Serial.print("WiFi initalized (");
  if(verbose) Serial.print(WiFi.softAPIP());
  if(verbose) Serial.println(")");
  if(verbose) Serial.println("Initializing Server");
  server.begin();
  delay(1000);
  if(verbose) Serial.println("Server initalized. Waiting for clients to connect");
  if(verbose) Serial.println(String("Point your browser to http://")+WiFi.softAPIP()+":80/");
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
          //client.println("Refresh: 1");  // refresh the page automatically every 1 sec
          client.println();
          print_HTML(client);
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

// Prints out the HTML webpage to the client (laptop web browser)
void print_HTML(WiFiClient client){
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Run/~Flash is set to ");
  if(digitalRead(0)) client.print("Run (HIGH)");
  else client.print("Flash (LOW)");
  client.println("<br />");       
  client.println("</html>");
}
//Initialize HeartBeat pin as an output and set to toggle at 0.5 Hz
void HB_init(){
  pinMode(HB, OUTPUT);
  HB_tick.attach(0.25,HB_CB);
}

void HB_CB(){
  digitalWrite(HB,!digitalRead(HB));
}
