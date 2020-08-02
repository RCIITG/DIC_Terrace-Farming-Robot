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
#define pump_pin_enable     12  //PWM
#define pump_pin1           28  //Digital
#define pump_pin2           29  //Digital
#define solenoid_pin        30  //Digital

//Global variables to check current job
bool movement_job = 0;
bool harvest_job = 0;
bool seeding_job = 0;
bool moisture_job = 0;
bool rise_job = 0;
bool water_job = 0;
bool plough_job = 0;

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

//Function to perform watering
void watering_func(bool flow_dir){
  if (water_job){
    //Runs only if moisture is low
    //Select flow path
    //LOW switches the relay
    if (flow_dir) digitalWrite(solenoid_pin, LOW);
    //Otherwise, relay will switch back
    //Turn on pump
    digitalWrite(pump_pin_enable, HIGH);
    digitalWrite(pump_pin1, HIGH);
    digitalWrite(pump_pin2, LOW);
    //This will turn off only when bot reaches end
  }
  //Else turn off pumps and solenoid
  else{
    digitalWrite(solenoid_pin, HIGH);
    digitalWrite(pump_pin_enable, LOW);
    digitalWrite(pump_pin1, LOW);
  }
}

//Function to perform ploughing
void ploughing_func(){
  //Uses stepper motor
}


void setup() {
  //Set up pins
  pinMode(moisture_pin, INPUT);
  moisture_servo.attach(moisture_servo_pin);
  pinMode(harvest_pin1, OUTPUT);
  pinMode(harvest_pin2, OUTPUT);
  pinMode(seeding_pin1, OUTPUT);
  pinMode(seeding_pin2, OUTPUT);
  pinMode(pump_pin_enable, OUTPUT);
  pinMode(pump_pin1, OUTPUT);
  pinMode(pump_pin2, OUTPUT);
  pinMode(solenoid_pin, OUTPUT);

  //Initialize motors
  moisture_servo.write(0);  //Initialize servo at 0 pos
  digitalWrite(harvest_pin1, LOW);
  digitalWrite(harvest_pin2, LOW);
  digitalWrite(seeding_pin1, LOW);
  digitalWrite(seeding_pin2, LOW);
  digitalWrite(pump_pin_enable, LOW);
  digitalWrite(pump_pin1, LOW);
  digitalWrite(pump_pin2, LOW);
  digitalWrite(solenoid_pin, LOW);
}

void loop() {
  //Flow control variable
  bool flow_dir = 0;
  //Change moisture check
  moisture_job = 1;
  //Change according to orientation or so so  
  //Run all the functions
  moisture_func();
  watering_func(flow_dir);
  harvest_func();
  seeding_func();
  ploughing_func();
}
