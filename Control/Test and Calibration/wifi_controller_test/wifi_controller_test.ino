// Wifi test
// webpage test
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
//SSID and Password to your ESP Access Point
const char* ssid = "DIC_IITG";
const char* password = "12345678";
byte data[]={0,010};




const char INDEX_HTML[] =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
"<title>ESP8266 Web Form Demo</title>"
"<style>"
"input[type=submit] {"
  "background-color: #4CAF50;"
  "color: white;"
 " padding: 12px 20px;"
  "border: none;"
  "border-radius: 4px;"
  "cursor: pointer;"
"}"

".slider.round:before {"
"border-radius: 50%;"
"}"

".slidecontainer {"
  "width: 100%;"
"}"
".slider {"
  "-webkit-appearance: none;"
  "width: 100%;"
  "height: 25px;"
  "background: #d3d3d3;"
  "outline: none;"
  "opacity: 0.7;"
  "-webkit-transition: .2s;"
  "transition: opacity .2s;"
"}"

".slider:hover {"
  "opacity: 1;"
"}"

".slider::-webkit-slider-thumb {"
  "-webkit-appearance: none;"
  "appearance: none;"
  "width: 25px;"
  "height: 25px;"
  "background: #4CAF50;"
  "cursor: pointer;"
"}"

".slider::-moz-range-thumb {"
 " width: 25px;"
  "height: 25px;"
  "background: #4CAF50;"
  "cursor: pointer;"
"}"
"</style>"
"</head>"
"<body>"
"<p>LED Digital</p>"

 "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>"
 "<p>Analog:"
 "</p>"
 "<div class=\"slidecontainer\">"
 "<input type=\"range\" min=\"1\" max=\"100\" value=\"1\" class=\"slider\" id=\"myRange\">"
 "<form action=\"/SPEED\" method=\"POST\">"
 "<input type=\"text\" id=\"v2\" name=\"demo\">"
 "<input type=\"submit\" value=\"change speed\"></form>"
  "</div>"

 "<p>Value: <span id=\"demo\"name=\"demo\"></span></p>"
"<script>"

"var slider = document.getElementById(\"myRange\");"
"var output = document.getElementById(\"demo\");"

"output.innerHTML = slider.value;"
"var test=slider.value;"
"document.getElementById(\"v2\").value = test;"

"slider.oninput = function() {"
  "output.innerHTML = this.value;"
  "var test=slider.value;"
  "document.getElementById(\"v2\").value = test;"
"}"



"</script>"

"</body>"


"</html>";

 
ESP8266WebServer server(80); //Server on port 80
 
//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200,"text/html",INDEX_HTML);
  //server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
  
  
}

void handleLED() {  
  data[0]=!data[0];   
  data[1]=data[1];// If a POST request is made to URI /LED
  //digitalWrite(BUILTIN_LED,!digitalRead(BUILTIN_LED));      // Change the state of the LED
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}
void handleSpeed(){
  String v = server.arg("demo");
  data[1]=v.toInt();
  data[0]=data[0];
  //data[2:5]=v.toInt()
//  digitalWrite(D3,LOW);
//  digitalWrite(D4,HIGH);
//  Serial.println(v);
//  analogWrite(D2,v.toInt()); 
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);   
}

 
//===============================================================
//                  SETUP
//===============================================================
void setup(void){
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);

IPAddress ip(192, 168, 0, 50); // this 3 lines for a fix IP-address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
WiFi.config(ip, gateway, subnet); // before or after Wifi.Begin(ssid, password);
  
 
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(ip);
 
  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/SPEED", HTTP_POST, handleSpeed);// Call the 'handleLED' function when a POST request is made to URI "/LED"
        //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  server.handleClient();          //Handle client requests
  Serial.write(data,2);
  delay(1000);
}
