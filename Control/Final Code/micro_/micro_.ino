//Inter IIT - DIC Event
//Arduino Micro
//harvesting seeding, up down mechanism
//Recieving data from esp8266

#define harvest_1_in1 11       
#define harvest_1_in2 10     
#define harvest_1_pwm  13
#define harvest_2_in1  9        
#define harvest_2_in2  8     
#define harvest_2_pwm  12

#define seeding_in1 5       
#define seeding_in2 4     
#define seeding_pwm  7
#define up_down_in1  3        
#define up_down_in2  2     
#define up_down_pwm  6

//Global variables to check current job

bool harvest_job = 0;
bool seeding_job = 0;
bool down_job = 0;
bool up_job = 0;
byte data[5]; 


void GoDown(){
  if(down_job){
    digitalWrite(up_down_in1, HIGH);
    digitalWrite(up_down_in2, LOW);
    analogWrite(up_down_pwm, 150);
    
  }
  else{
    digitalWrite(up_down_in1, LOW);
    digitalWrite(up_down_in2, LOW);
  }
  
}

void GoUp(){
  if(up_job){
   
    digitalWrite(up_down_in1, LOW);
    digitalWrite(up_down_in2, HIGH);
    analogWrite(up_down_pwm, 150);
    
  }
  else{
    digitalWrite(up_down_in1, LOW);
    digitalWrite(up_down_in2, LOW);
  }
 
}

 

void harvest_func(){
  if(harvest_job){
    //Turn on harvester motors
    digitalWrite(harvest_1_in1, HIGH);
    digitalWrite(harvest_1_in2, LOW);
    digitalWrite(harvest_2_in1, HIGH);
    digitalWrite(harvest_2_in2, LOW);
    analogWrite(harvest_1_pwm,200);
    analogWrite(harvest_2_pwm,200);
  }
  //Else turn off motors
  else{
    digitalWrite(harvest_1_in1, LOW);
  digitalWrite(harvest_2_in1, LOW);
  }
}

void seeding_func(){
  if(seeding_job){
    //Turn on seeding motor
    digitalWrite(seeding_in1, HIGH);
    digitalWrite(seeding_in2, LOW);
    analogWrite(seeding_pwm,170);
  }
  //Else turn off motors
  else digitalWrite(seeding_in1, LOW);
}



void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(harvest_1_in1, OUTPUT);
  pinMode(harvest_1_in2, OUTPUT);
  pinMode(harvest_1_pwm, OUTPUT);
  pinMode(harvest_2_in1, OUTPUT);
  pinMode(harvest_2_in2, OUTPUT);
  pinMode(harvest_2_pwm, OUTPUT);
  pinMode(seeding_in1, OUTPUT);
  pinMode(seeding_in2, OUTPUT);
  pinMode(seeding_pwm, OUTPUT);

  digitalWrite(harvest_1_in1, LOW);
  digitalWrite(harvest_1_in2, LOW);
  digitalWrite(harvest_2_in1, LOW);
  digitalWrite(harvest_2_in2, LOW);
  digitalWrite(seeding_in1, LOW);
  digitalWrite(seeding_in2, LOW);
  digitalWrite(up_down_in1, LOW);
  digitalWrite(up_down_in2, LOW);
  

}

void loop() {
  Serial1.readBytes(data,5);
  up_job =(int)data[4];
  down_job =(int)data[3];
  harvest_job =(int)data[2];
  seeding_job = (int) data[1];
  harvest_func();
  seeding_func();
  GoDown();
  GoUp();

}
