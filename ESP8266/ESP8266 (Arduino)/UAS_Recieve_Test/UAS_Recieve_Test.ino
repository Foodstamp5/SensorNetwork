/*
 *  This sketch parses GPS data from the other ESP and prints it to the UART
 *
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[]     = "CSUFUAV";
char password[] = "csufuavisthebest";
bool useStaticIP(true);
IPAddress staticIP(192,168,1,100);
IPAddress dns(192,168,1,2);
IPAddress gateway(192,168,1,2);

WiFiUDP UDP;
int port(1450); //Port to listen for packet
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //Buffer to hold incoming packet,

void setup() {
  ESP.wdtEnable(10000);
  
  //Init serial
  Serial.begin(9600);  
  Serial.println("\nBooted");
  
  //Init UDP
  UDP.begin(port);
}

void loop() {
  ESP.wdtFeed();
  
  //Make sure we are connected to wifi
  while(WiFi.status()!=WL_CONNECTED){
    //Give 10s for connection
    wifi_connect(ssid,password,10000);
  }
  
  //Check for UDP packet
  if(int packetSize=UDP.parsePacket()) {
    Serial.print("\nReceived packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = UDP.remoteIP();
    for (int i =0; i < 4; i++){
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(UDP.remotePort());

    // read the packet into packetBufffer
    UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    Serial.println("Parsed:");
    float Lat,Long,Alt;
    parseGPS(packetBuffer,&Lat,&Long,&Alt);
    Serial.println(String("Lat:  ")+Lat);
    Serial.println(String("Long: ")+Long);
    Serial.println(String("Alt:  ")+Alt);
  }
}

/*
 * Helper function for parseGPS
 * Replaces the next comma with '\0' and returns a pointer to the next number
 * If there is no comma, it will do nothing and return null
 */
char* find_comma(char* ptr){
  while(*ptr!=','&&*ptr!='\0'){
    ptr++;
  }
  if(*ptr=='\0') return NULL;
  else{
    *ptr='\0';
    return ptr+1;
  }
}

/*
 * Parse the GPS packet into Lat, Long, & Alt
 * Any value that cannot be parse will be set to 0.0
 * If there are not two commas seperating the three values, all values will be set to 0.0 and return false
 * 
 * Example of null-terminated packetBuffer: $36.815021,-119.749496,118.4
 */

bool parseGPS(char* packetBuffer, float* Lat, float* Long, float* Alt){
  //Discard leading '$'
  if(*packetBuffer=='$') packetBuffer++;
  
  //Parse Latitude
  char* ptr=find_comma(packetBuffer);  //Replace next comma with '\0' so we can parse
  if(ptr=='\0') goto error;            //Something went wrong
  *Lat=atof(packetBuffer);             //Parse float
  packetBuffer=ptr;                    //Move to next number
  
  //Parse Longitude
  ptr=find_comma(packetBuffer);        //Replace next comma with '\0' so we can parse
  if(ptr=='\0') goto error;            //Something went wrong
  *Long=atof(packetBuffer);            //Parse float
  packetBuffer=ptr;                    //Move to next number
  
  //Parse Altitude
  *Alt=atof(packetBuffer);             //Parse float
  
  return true;
  
  error:
  *Lat=0;
  *Long=0;
  *Alt=0;
  return false;
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
  if(useStaticIP) WiFi.config(staticIP,dns,gateway);
  
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
