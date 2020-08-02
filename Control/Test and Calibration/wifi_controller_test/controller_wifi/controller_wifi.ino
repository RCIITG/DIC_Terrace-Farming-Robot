
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
//SSID and Password to your ESP Access Point
const char* ssid = "DIC_IITG";
const char* password = "12345678";
byte data[]={0,0,0,0,0,0,0};


//{start,pump,side,center,seeding,plough,harvest}

ESP8266WebServer server(80);


const char INDEX_HTML[] =
"<!DOCTYPE html>"
"<html>"
"<head>"
    "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
    "<title>DIC Control</title>"
"<style>"
    "input[type=submit] {"
      "background-color:  #e7e7e7;"
      "padding: 10px 2px;"
      "font-size: 15px;"
      "border: 2px solid #555555;"
      "border-radius: 4px;"
      "cursor: pointer;"
      "width:100px;"
      "text-align: center;"
    "}"
    "input[value=Stop] {"
     "color: #f44336;"
     "font-size: 20px;"
    "}"
    "input[value=Start] {"
     "color:  #4CAF50;"
     "font-size: 20px;"
    "}"
    

     
     ".container{"
        "padding-left:10px;"
        "padding-top: 10px;"
        "display: grid;"
        "grid-template-columns: 1fr 1fr;"
        "grid-column-gap: 18vh;"
     "}"
     ".container2{"
        "padding-left:15px;"
        "display: grid;"
        "grid-template-columns: 1fr 1fr 1fr;"
        "grid-row-gap: 10px;"
        "margin-top: 30vh;"
     "}"
     ".container3{"
        "padding-left:15px;"
        "padding-top:15px;"
        "display: grid;"
        "grid-template-columns: 1fr 1fr 1fr;"
        "grid-row-gap: 10px;"
        
     "}"


    
"</style>"
"</head>"
"<body>"
"<div class = \"container\">"


"<form action=\"/start\" method=\"POST\">"
  "<input type=\"submit\" value=\"Start\">"
"</form>"

"<form action=\"/stop\" method=\"POST\">"
  "<input type=\"submit\" value=\"Stop\">"
"</form>"



"</div>"


"<div class = \"container2\">"


"<form action=\"/pump\" method=\"POST\">"
  "<input type=\"submit\" value=\"Pump\">"
"</form>"

"<form action=\"/side\" method=\"POST\">"
  "<input type=\"submit\" value=\"Side\">"
"</form>"
"<form action=\"/center\" method=\"POST\">"
  "<input type=\"submit\" value=\"Center\">"
"</form>"
"</div>"

"</div>"

"<div class = \"container3\">"


"<form action=\"/seeding\" method=\"POST\">"
  "<input type=\"submit\" value=\"Seeding\">"
"</form>"

"<form action=\"/ploughing\" method=\"POST\">"
  "<input type=\"submit\" value=\"Ploughing\">"
"</form>"
"<form action=\"/harvesting\" method=\"POST\">"
  "<input type=\"submit\" value=\"Harvesting\">"
"</form>"
"</div>"


"</body>"

"</html>";

 
 
 
//==============================================================
//{start,pump,side,center,seeding,plough,harvest}
//==============================================================
void handleRoot() {                        
  server.send(200,"text/html",INDEX_HTML);
 
}
void handleStart() {  
  data[0]=1;
  
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handleStop() {  
  data[0]=0;
  
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handlePump() {  
  data[1]=!data[1];
 
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handleSide() {  
  data[2]=!data[2];
  
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handleCenter() {  
  data[3]=!data[3];
 
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handleSeeding() {  
  data[4]=!data[4];
 
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handlePloughing() {  
  data[5]=!data[5];

  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handleHarvesting() {  
  data[6]=!data[6];
  server.sendHeader("Location","/");        
  server.send(303);                         
}

 
//===============================================================
//                  SETUP
//===============================================================
void setup(void){
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_AP);           
  WiFi.softAP(ssid, password);
 

IPAddress ip(192, 168, 0, 50); 
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
WiFi.config(ip, gateway, subnet); 
  
 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("HotSpt IP:");
  Serial.println(ip);
 
  server.on("/", HTTP_GET, handleRoot);    
  server.on("/start", HTTP_POST, handleStart);     
  server.on("/stop", HTTP_POST, handleStop);
  server.on("/pump", HTTP_POST, handlePump);
  server.on("/side", HTTP_POST, handleSide);
  server.on("/center", HTTP_POST, handleCenter);
  server.on("/seeding", HTTP_POST, handleSeeding);
  server.on("/ploughing", HTTP_POST, handlePloughing);
  server.on("/harvesting", HTTP_POST, handleHarvesting);
  
  
 
  server.begin();                
  Serial.println("HTTP server started");
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  server.handleClient();
  Serial.write(data,7);
  delay(1000);          
}
