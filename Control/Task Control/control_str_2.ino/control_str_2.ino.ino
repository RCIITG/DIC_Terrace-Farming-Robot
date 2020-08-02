//Inter IIT - DIC Event
//Control structure and functions
//Arduino Mega 2560

//Libraries
#include <Servo.h>

//Define pins
#define moisture_pin        A0  //Analog In
#define moisture_servo_pin  13  //PWM
#define harvest_pin1        23  //Digital
#define harvest_pin2        24  //Digital
#define seeding_pin1        25  //Digital
#define seeding_pin2        26  //Digital      
#define pump_pin_pwm        12  //PWM
#define pump_pin1           28  //Digital
#define pump_pin2           29  //Digital
#define solenoid_side_pin   30  //Digital
#define solenoid_center_pin 31  //Digital

//Global variables to check current job
bool movement_job = 0;
bool rise_job = 0;
bool water_job = 0;  //update internally

bool moisture_job = 0;
bool solenoid_side = 0;
bool solenoid_center = 0;
bool harvest_job = 0;
bool seeding_job = 0;
bool plough_job = 0;

//Recieve task controls from ESP in data
byte data[7];  

//Set up servo object
Servo moisture_servo; 
 
//Function to perform harvesting
void harvest_func(){
  if(harvest_job){
    //Turn on harvester motor
    digitalWrite(harvest_pin1, HIGH);
    digitalWrite(harvest_pin2, LOW);
  }
  //Else turn off motors
  else digitalWrite(harvest_pin1, LOW);
}

//Function to perform seeding
void seeding_func(){
  if(seeding_job){
    //Turn on seeding motor
    digitalWrite(seeding_pin1, HIGH);
    digitalWrite(seeding_pin2, LOW);
  }
  //Else turn off motors
  else digitalWrite(seeding_pin1, LOW);
}

//Function to check moisture
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
  }
  //Else no watering
  else water_job = 0;
}

void ControlPump(){
  if(solenoid_side or solenoid_center){
    //Turn on pump
    analogWrite(pump_pin_pwm, HIGH);
    digitalWrite(pump_pin1, HIGH);
    digitalWrite(pump_pin2, LOW); 
  }
  else{
    //Turn off pump
    analogWrite(pump_pin_pwm, HIGH);
    digitalWrite(pump_pin1, LOW);
    digitalWrite(pump_pin2, LOW); 
  }
}

//Function to perform watering
void watering_func(){
  if (water_job){
    ControlPump();
    if(solenoid_side){
      //Turn on side solenoid
      digitalWrite(solenoid_side_pin, HIGH);
    }
    else{
      //Turn off side solenoid
      digitalWrite(solenoid_side_pin, LOW);
    }
    if(solenoid_center){
     //Turn on center solenoid 
     digitalWrite(solenoid_center_pin, HIGH);
    }
    else{
      //Turn off center solenoid
      digitalWrite(solenoid_center_pin, LOW);
    }
    
}
else{
  //Turn off pump
  analogWrite(pump_pin_pwm, LOW);
  digitalWrite(pump_pin1, LOW);
  digitalWrite(pump_pin2, LOW); 
}
}

//Function to perform ploughing
void ploughing_func(){
  //Uses stepper motor
}


void setup() {
  Serial.begin(9600);
  //Set up pins
  pinMode(moisture_pin, INPUT);
  moisture_servo.attach(moisture_servo_pin);
  pinMode(harvest_pin1, OUTPUT);
  pinMode(harvest_pin2, OUTPUT);
  pinMode(seeding_pin1, OUTPUT);
  pinMode(seeding_pin2, OUTPUT);
  pinMode(pump_pin_pwm, OUTPUT);
  pinMode(pump_pin1, OUTPUT);
  pinMode(pump_pin2, OUTPUT);
  pinMode(solenoid_side_pin, OUTPUT);
  pinMode(solenoid_center_pin, OUTPUT);

  //Initialize motors
  moisture_servo.write(0);  //Initialize servo at 0 pos
  digitalWrite(harvest_pin1, LOW);
  digitalWrite(harvest_pin2, LOW);
  digitalWrite(seeding_pin1, LOW);
  digitalWrite(seeding_pin2, LOW);
  analogWrite(pump_pin_pwm, LOW);
  digitalWrite(pump_pin1, LOW);
  digitalWrite(pump_pin2, LOW);
  digitalWrite(solenoid_side_pin, LOW);
  digitalWrite(solenoid_center_pin, LOW);
}

void loop() {
  Serial.readBytes(data,7);
  moisture_job =(int)data[1];
  solenoid_side =(int)data[2];
  solenoid_center =(int)data[3];
  harvest_job =(int)data[4];
  seeding_job = (int) data[5];
  ploughing_job = (int) data[6];
  //Run all the functions
  moisture_func();
  watering_func();
  harvest_func();
  seeding_func();
  ploughing_func();
}
