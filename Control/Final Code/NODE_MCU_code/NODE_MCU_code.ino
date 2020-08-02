//Inter IIT - DIC Event
//ESP8266 NODE MCU
//servo for moisture sensor, stepper for ploughing
//sendind data , handling webpage

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#define stepper_dir      D0       
#define stepper_en       D1    
#define stepper_step     D2
#define moisture_pin     A0  //Analog In        
#define extra_pin_1      D3  
#define extra_pin_2      D4
#define extra_pin_3      D5
#define extra_pin_4      D6

const char* ssid = "DIC_IITG";
const char* password = "12345678";
byte data[]={0,0,0,0,0};
Servo moisture_servo; 

bool center_solenoid_job=0;
bool side_solenoid_job=0;
bool plough_job=0;
bool moisture_job=0;
bool down_job=0;
bool up_job=0;
bool water_job=0;
//{start_stop,seeding,harvest,down,up}

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


"<form action=\"/moisture\" method=\"POST\">"
  "<input type=\"submit\" value=\"Moisture\">"
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

"<div class = \"container3\">"


"<form action=\"/go_down\" method=\"POST\">"
  "<input type=\"submit\" value=\"Down\">"
"</form>"

"<form action=\"/go_up\" method=\"POST\">"
  "<input type=\"submit\" value=\"Up\">"
"</form>"

"</div>"


"</body>"

"</html>";

//==============================================================
//{start_stop,seeding,harvest,up_down}
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
void handleSeeding() {  
  data[1]=!data[1];
 
  server.sendHeader("Location","/");        
  server.send(303);                         
}

void handleHarvesting() {  
  data[2]=!data[2];
  server.sendHeader("Location","/");        
  server.send(303);                         
}

void handleSide() {  
  side_solenoid_job=!side_solenoid_job;
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handleCenter() {  
  center_solenoid_job=!center_solenoid_job;
  server.sendHeader("Location","/");        
  server.send(303);                         
}

void handlePloughing() {  
  plough_job=!plough_job;
  server.sendHeader("Location","/");        
  server.send(303);                         
}

void handleMoisture() { 
  //up_down_motor_go_down
  moisture_job=!moisture_job;
  server.sendHeader("Location","/");        
  server.send(303);                         
}


void handleDown() {  
  down_job=!down_job;
  server.sendHeader("Location","/");        
  server.send(303);                         
}
void handleUp() {  
  up_job=!up_job;
  server.sendHeader("Location","/");        
  server.send(303);                         
}


int moisture_func(){
  //If moisture needs to be checked
  if (moisture_job){
    int moisture_val = 0;
    //Full servo rotation
    for(int i=0; i<=180; i++){
      moisture_servo.write(i);
    }
    //Take 200 readings
    for(int i=0; i<200; i++){
      //Read moisture sensor values
      moisture_val += analogRead(moisture_pin);
      delay(20);
    }
    //Take average reading
    moisture_val = moisture_val/200;
    //Select job to perform
    //Temp-idea - remove later
    //If less moisture - perform water_job
    if (moisture_val < 512)water_job = 1;
    //Complete moisture detection work
    moisture_job = 0;
    for(int i=180; i>=1; i--){
      moisture_servo.write(i);
    }
  }
  //Else no watering
  else water_job = 0;
}

void watering_func(){
//  if (water_job){
//    ControlPump();
//    if(solenoid_side){
//      //Turn on side solenoid
//      digitalWrite(solenoid_side_pin, HIGH);
//    }
//    else{
//      //Turn off side solenoid
//      digitalWrite(solenoid_side_pin, LOW);
//    }
//    if(solenoid_center){
//     //Turn on center solenoid 
//     digitalWrite(solenoid_center_pin, HIGH);
//    }
//    else{
//      //Turn off center solenoid
//      digitalWrite(solenoid_center_pin, LOW);
//    }
//    
//}
//else{
//  //Turn off pump
//  analogWrite(pump_pin_pwm, LOW);
//  digitalWrite(pump_pin1, LOW);
//  digitalWrite(pump_pin2, LOW); 
//}
}

void ploughing_func(){
  if(plough_job){
    
  }
  else{
    
  }
  
}

void ControlPump(){
  if(center_solenoid_job or side_solenoid_job){
    //Turn on pump and on_off solenoid
    
    
  }
  else{
    //Turn off pump
    
  }
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
  server.on("/seeding", HTTP_POST, handleSeeding);
  server.on("/harvesting", HTTP_POST, handleHarvesting);
  
  server.on("/side", HTTP_POST, handleSide);
  server.on("/center", HTTP_POST, handleCenter);
  server.on("/ploughing", HTTP_POST, handlePloughing);
  server.on("/moisture", HTTP_POST, handleMoisture);
  server.on("/go_up", HTTP_POST, handleUp);
  server.on("/go_down", HTTP_POST, handleDown);
  
  
  server.begin();   
               
  Serial.println("HTTP server started");

  moisture_servo.attach(D8);
  pinMode(stepper_dir,OUTPUT);
  pinMode(stepper_en,OUTPUT);
  pinMode(stepper_step,OUTPUT);
  pinMode(moisture_pin,INPUT);
  pinMode(extra_pin_1,OUTPUT);
  pinMode(extra_pin_2,OUTPUT);
  pinMode(extra_pin_3,OUTPUT);
  pinMode(extra_pin_4,OUTPUT);
}
//===============================================================
//                     LOOP
//===============================================================

void loop() {
  server.handleClient();
  Serial.write(data,5);
  moisture_func();
  ploughing_func();
  watering_func(); 

}
