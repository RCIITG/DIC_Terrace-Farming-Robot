//Wall following with PID implementation
//Robotics Club - DIC Terrace Farming Robot
//Inter IIT Tech Meet 2019
//11-12-19 -> ____

//MACROS - TUNABLE PARAMETERS
#define MAX_DIST 100
#define MIN_DIST 2
#define DISTANCE_PER_SENSORVALUE 0.016f
#define reach_alpha 5
#define body_length 70
#define body_breadth 40
#define guard_offset 2
#define turn_offset 40
#define follow_alpha 50
#define Kd 1

//Sensor pins
//Order - left-back, left-front, front, front-bottom, right-front, right-back, back
const int trig_pins[7] = {21,22,23,24,25,26,27};
const int echo_pins[7] = {31,32,33,34,35,36,37};
//Motor pins
//Main - Left - 2 Motors
const int lmain_dir = 1;
const int lmain_pwm = 2;
const int lmain_brk = 3;
//Main - Right - 2 Motors
const int rmain_dir = 4;
const int rmain_pwm = 5;
const int rmain_brk = 6;
//Subs - Left
const int lsubs_dir = 7;
const int lsubs_pwm = 8;
const int lsubs_brk = 9;
//Subs - Right
const int rsubs_dir = 10;
const int rsubs_pwm = 11;
const int rsubs_brk = 12;

//Functions
void read_ultrasonic();
void update_distances();
void reach_distance(float, int, int);
void follow_wall(float, int, int);

//Global variables
float distances[7];
float leftx, leftx1, leftx2, rightx, rightx1, rightx2;
bool  robot_direction = 0;  //1 for towards red zone
bool  turning = 0;  //1 when the robot turns

void setup() {
  //Start serial
  Serial.begin(9600);
  //Set pinmodes
  //Motor pins
  pinMode(lmain_dir, OUTPUT);
  pinMode(lmain_pwm, OUTPUT);
  pinMode(lmain_brk, OUTPUT);
  pinMode(rmain_dir, OUTPUT);
  pinMode(rmain_pwm, OUTPUT);
  pinMode(rmain_brk, OUTPUT);
  pinMode(lsubs_dir, OUTPUT);
  pinMode(lsubs_pwm, OUTPUT);
  pinMode(lsubs_brk, OUTPUT);
  pinMode(rsubs_dir, OUTPUT);
  pinMode(rsubs_pwm, OUTPUT);
  pinMode(rsubs_brk, OUTPUT);
  //Sensor pins
  for(int i=0; i<7; i++)  {
    pinMode(trig_pins[i], OUTPUT);
    pinMode(echo_pins[i], INPUT);
  }
  //Read ultrasonic sensors for initiation
  read_ultrasonic();
  //Update important distances
  update_distances();
  //Pins initial state
  digitalWrite(lmain_dir, LOW);
  digitalWrite(lmain_pwm, LOW);
  digitalWrite(lmain_brk, HIGH);
  digitalWrite(rmain_dir, LOW);
  digitalWrite(rmain_pwm, LOW);
  digitalWrite(rmain_brk, HIGH);
  digitalWrite(lsubs_dir, LOW);
  digitalWrite(lsubs_pwm, LOW);
  digitalWrite(lsubs_brk, HIGH);
  digitalWrite(rsubs_dir, LOW);
  digitalWrite(rsubs_pwm, LOW);
  digitalWrite(rsubs_brk, HIGH);
}

void loop() {
  //Run when not turning
  if (!turning)  {
    //Reach distance
    reach_distance(15, 100, 255);
    //Follow wall
    follow_wall(15, 100, 255);
  }
  //If turning
  else  perform_turn();
}

//Function to save ultrasonic sensor readings to global variable
void read_ultrasonic() {
  //Sensor value
  float temp_dist;
  //Take average value(3 readings) for each sensor
  for(int sen_num = 0; sen_num < 7; sen_num++)  {
    //When not turning, skip sensors not required in given direction 
    if (!turning) {
      if (robot_direction && (sen_num == 0 || sen_num == 1 || sen_num == 6))  continue; //Left and back sensors not used
      else if (!robot_direction && (sen_num == 4 || sen_num == 5 || sen_num == 6))  continue; //Right and back sensors not used
    }
    for(int read_num = 0; read_num < 3; read_num++) {
      //Ultrasonic reading
      digitalWrite(trig_pins[sen_num],HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pins[sen_num],LOW);
      temp_dist = pulseIn(echo_pins[sen_num],HIGH) * DISTANCE_PER_SENSORVALUE;
      //Limits
      if (temp_dist > MAX_DIST)  temp_dist = MAX_DIST;
      else if (temp_dist < MIN_DIST)  test_dist = MIN_DIST;
      //Update distance
      distances[sen_num] += temp_dist;
    }
    //Average
    distances[sen_num] = distances[sen_num]/3;
  }
}

//Function to update important distances
void update_distances() {
  leftx1 = body_length*distances[0]/sqrt(pow(distances[0]-distances[1], 2) + pow(body_length, 2));
  leftx2 = body_length*distances[1]/sqrt(pow(distances[0]-distances[1], 2) + pow(body_length, 2));
  rightx1 = body_length*distances[5]/sqrt(pow(distances[5]-distances[4], 2) + pow(body_length, 2));
  rightx2 = body_length*distances[4]/sqrt(pow(distances[5]-distances[4], 2) + pow(body_length, 2));
  leftx = (leftx1 + leftx2)/2;
  rightx = (rightx1 + rightx2)/2;
}

//Function to reach particular distance
void reach_distance(float d_req, int v_min, int v_max) {
  //Proportional controller
  float error = 0;
  float Kp = 0;
  float input = 0;
  float x = 0;
  float v0 = (v_max + v_min)/2;
  //Run until guard
  while(1)  {
    //Read ultrasonic sensors
    read_ultrasonic();
    //Update important distances
    update_distances();
    //Define which x to use depending on the direction
    if (robot_direction) x = rightx;
    else x = leftx;
    //Update error term
    error = d_req - x;
    //Check guard condition - needs blending
    if (abs(error) < guard_offset)  break;
    //Update proportional gain
    Kp = (float)(v0-v_min)*(1-exp(reach_alpha*(error*error)))/abs(error);
    //Set input
    input = Kp*error;
    //Provide input to speed
    analogWrite(lmain_pwm, v0+(int)input);
    analogWrite(rmain_pwm, v0-(int)input);
    //Check if turn is necessary
    if (distances[2] < turn_offset) {
      turning = 1;
      break;
    }
  }
}

//Function to follow wall
void follow_wall(float d_req, int v_min, int v_max)  {
  //PD Controller
  float error = 0;
  float Kp = 0;
  float input = 0;
  float v0 = (v_max + v_min)/2;
  float prev_error = 0;
  float x = 0;
  float x1 = 0;
  float x2 = 0;
  //Run until guard
  while(1)  {
    //Read ultrasonic sensors
    read_ultrasonic();
    //Update important distances
    update_distances();
    //Define which x,x1,x2 to use depending on the direction eg-
    if (robot_direction)  {
      x  = rightx;
      x1 = rightx1;
      x2 = rightx2;
    }
    else  {
      x  = leftx;
      x1 = leftx1;
      x2 = leftx2;
    }
    //First check guard condition
    if (abs(d_req - x) < guard_offset) break;
    //Update error term
    error = x1 - x2;
    //Proportional gain
    Kp = (float)(v0-v_min)*(1-exp(follow_alpha*(error*error)))/abs(error);
    //Set input
    input = Kp*error + Kd*(error - prev_error);
    //Set previous error
    prev_error = error;
    //Provide input to speed
    analogWrite(lmain_pwm, v0+(int)input);
    analogWrite(rmain_pwm, v0-(int)input);
    //Check if turn is necessary
    if (distances[2] < turn_offset) {
      turning = 1;
      break;
    }
  }
}

//Function to perform turn
void perform_turn(int turn_vel) {
  //Reverse velocities on opp sides until condition
  while(1)  {
    //Read ultrasonic sensors
    read_ultrasonic();
    //Update important distances
    update_distances();
    //Guard condition - back range, front max, left max, right range
    if(distances[6] < turn_offset, distances[2] >= MAX_DIST, leftx >= MAX_DIST, rightx < turn_offset) {
      turning = 0;
      robot_direction = 1;
      break;
    }
    //Velocities
    analogWrite(lmain_pwm, turn_vel);
    digitalWrite(rmain_dir, HIGH);
    analogWrite(rmain_pwm, turn_vel);
  }
} 
