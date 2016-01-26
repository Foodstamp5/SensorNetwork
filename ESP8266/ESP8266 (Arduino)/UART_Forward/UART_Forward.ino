#include <ESP8266WiFi.h>

char ssid[]     = "Nick";
char password[] = "nicksiot";

char host[] = "test.nmorley.net";
char url[] = "/esp_text.php";
int port = 80;

char id[]="ESP";
String buffer="";

bool verbose=!false;
//if verbose is false, bring TX low on send and high on recieve (on for send, off for recieve)

void setup() {
 ESP.wdtEnable(10000);
 
 
 Serial.begin(9600);
 Serial.println("\nBooted");
 while(!post(String("Booted"),10000));
}

volatile int count=0;

void loop() {
//  ESP.wdtFeed();
//  char temp;
//  if(Serial.available()){
////    temp=Serial.read();
//    temp='-';
//    Serial.println(String("Got char: ")+temp);
//    buffer+=temp;
//    if(temp=='\n'){
//      post(buffer,10000);
//      buffer="";
//    }else Serial.println(String("Buffer = ")+buffer);
//  }
//  delay(1000);
  post(String("Uptime: ")+millis()/1000+"."+millis()/100%10+" s",10000);
  Serial.println(String("Recieved chars: ")+count);
}

void serialEvent(){
  while(Serial.available()){
    ESP.wdtFeed();
    count++;
  }
}

/*
 * Post the string to the server
 */

bool post(String str, int timeout){
  ESP.wdtFeed();
  if(verbose) Serial.println(String("")+"Attemping to post \""+str+'\"');
  for(int i=0;i<str.length();i++) if(str[i]==' ') str[i]='+';
  //Make sure we are connected to wifi status before posting
  while(WiFi.status()!=WL_CONNECTED){
    //Give 10s for connection
    wifi_connect(ssid,password,10000);
  }
  
  if(verbose) Serial.print("connecting to ");
  if(verbose) Serial.print(host);
  if(verbose) Serial.println(String(":")+port);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    ESP.wdtFeed();
    Serial.println("connection failed");
    Serial.println("Retrying");
    return false;
  }
  
  // We now create a URI for the request
  String send_url=String(url)+"?ID="+id+"&STRING="+str;
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
  return true;
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
