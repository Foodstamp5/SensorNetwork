/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

char ssid[]     = "Nick";
char password[] = "nicksiot";

char host[] = "test.nmorley.net";
char url[] = "/esp.php";
int port = 80;

char id[]="ESP";
char mark='$';
bool invert=true;  //This is because the LEDs are active low

byte output_pins[]={0,2};
const byte output_size=sizeof(output_pins)/sizeof(*output_pins);
byte output_GPIO[output_size];

byte input_pins[]={};
const byte input_size=sizeof(input_pins)/sizeof(*input_pins);
byte input_GPIO[input_size];

byte RX=3;
byte TX=1;

bool verbose=!false;
//if verbose is false, bring TX low on send and high on recieve (on for send, off for recieve)

void setup() {
  ESP.wdtEnable(10000);
  //Set GPIO pins as outputs
  for(int i=0;i<output_size;i++) pinMode(output_pins[i],OUTPUT);
  
  //Set GPIO pins as inputs
  for(int i=0;i<input_size;i++) pinMode(input_pins[i],INPUT_PULLUP);
  
  //Init serial
  Serial.begin(9600);  
  Serial.println("\nBooted");
}

void loop() {
  ESP.wdtFeed();
  //Make sure we are connected to wifi status before requesting
  while(WiFi.status()!=WL_CONNECTED){
    //Give 10s for connection
    wifi_connect(ssid,password,10000);
  }
  
  //Poll server for output data
  get_status(output_GPIO,output_pins,output_size,host,port,url,id,mark,10000,verbose);
  
  //Write fetched data
  for(int i=0;i<output_size;i++) digitalWrite(output_pins[i],output_GPIO[i]^invert);
}

/*
 * Connects to a wifi AP secured with WPA/WPA2
 * Returns the success value when the wifi is connected or the timeout is reached
 * A timeout if 0 will not return until the wifi is connected
 */
 
bool wifi_connect(char* ssid, char* password, int timeout){
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  //Connect with timeout
  if(timeout){
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(1000);
      ESP.wdtFeed();
      timeout-=1000;
      if(timeout<=0){
        Serial.println("\nWiFi not connected"); 
        return false;
      }
    }
  }else{ //Wait for Wifi connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      ESP.wdtFeed();
      Serial.print(".");
    }
  } 
  
  //At this point the wifi is connected
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

/*
 * Pushes input data to my server
 * Note that a timeout of zero will not wait indefinitely like wifi_connect
 */
 
bool post_status(byte* input_GPIO, byte* input_pins, const byte input_size, char* host, int port, char* url, char* id, char mark, int timeout,bool verbose){
  if(verbose) Serial.print("connecting to ");
  if(verbose) Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
   ESP.wdtFeed();
    Serial.println("connection failed");
    Serial.println("Retrying");
    return false;
  }
  
  // We now create a URI for the request
  String send_url=String(url)+"?ID="+id+"&GPIO"+input_pins[0]+"="+input_GPIO[0];
  if(verbose) Serial.println(String("Requesting URL: http://")+host+send_url);
  
  // This will send the request to the server
  client.print(String("GET ") + send_url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  // wait for the server to repond
  while(!client.available()){
    delay(10);
    ESP.wdtFeed();
    timeout-=10;
    if(timeout<=0){
      Serial.println("Connection timed out");
      return false;
    }
  }
  
  if(verbose) Serial.println("Server responded");
}

/*
 * Polls my server for the output data
 * Note that a timeout of zero will not wait indefinitely like wifi_connect
 */

bool get_status(byte* output_GPIO, byte* output_pins, const byte output_size, char* host, int port, char* url, char* id, char mark, int timeout,bool verbose){
  if(verbose) Serial.print("connecting to ");
  if(verbose) Serial.println(host);
  
  //Rewrite to do everything in one request by replacing PIN with GPIOx=1 or array?
  for(int i=0;i<output_size;i++){
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    if (!client.connect(host, port)) {
        ESP.wdtFeed();
        Serial.println("connection failed");
        Serial.println("Retrying");
        return false;
    }
    ESP.wdtFeed();
    if(verbose) Serial.println(String("Round #")+(i+1)+" of "+output_size);
    // We now create a URI for the request
    String send_url=String(url)+"?ID="+id+"&GPIO"+output_pins[i]+"=R";
    if(verbose) Serial.println(output_GPIO[i]);
    if(verbose) Serial.println(String("Requesting URL: http://")+host+send_url);
    if(!verbose) digitalWrite(TX,LOW^invert);
    
    // This will send the request to the server
    client.print(String("GET ") + send_url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
                 
    // wait for the server to repond
    while(!client.available()){
      delay(10);
      ESP.wdtFeed();
      timeout-=10;
      if(timeout<=0){
        Serial.println("Connection timed out");
        return false;
      }
    }
    
    if(verbose) Serial.println("Server responded");
    if(!verbose) digitalWrite(TX,HIGH^invert);
    
    //Find the GPIO data
  
    //jump to $ (designator mark)
    client.readStringUntil(mark);
    if(verbose) Serial.println("Recieved data");
    char data;
    data=client.read();
    if(verbose) Serial.print(String("Found char: "));
    if(verbose) Serial.print(data,HEX);
    if(verbose) Serial.println(String('(')+data+')');
    if(data==0xFF){
      Serial.println(String("Can't find id: ")+id);
      Serial.println("Retrying");
      return false;
    }else{
      if(verbose) Serial.println(String("Storing ")+(data=='1')+" to GPIO"+output_pins[i]);
      output_GPIO[i]=(data=='1');
      client.read();  //Discard ','
    }
  }
  if(verbose) Serial.println("Done group");

  if(verbose) Serial.println("Disconnecting from server");
//  client.stop();
  if(verbose) Serial.println("Refreshing");
  return true;
}
