//Inter IIT - DIC Event
//AutoNavigation, pid, Movement
//Arduino Mega 2560

#define kp 1.50                                            
#define ki 0.00
#define kd 1.00
#define k_dist 3

#define dist_to_wall 40.0
#define error 2.5
#define pwm 100
#define weight 0.5
#define angle_thresh -1.5

float inter_distance = dist_to_wall ; 
int region=1;
int dist1,dist2;

volatile float sensor_val[7];
float error_angle = 0;
float error_dist = 0;
float total_error = 0;
int angle_prev,angle_current;
bool control_movement;

float DISTANCE_PER_SENSORVALUE=.017;
int MAX_SENSOR_READING=4000;
int LARGE_SENSOR_READING=15000;
int DEPTH_SENSOR_DIFFERENCE=20;
int CONSTANT_DIST=20;
int THRESH_DIST=5;
int THRESH_VALUE=10;
int MAX_CORREC=15;
int THRESH_ANGLE=1;
int constant_speed=100;
int speed_avg=120;
int calibration=50;
int FLAG=1;



//================================================================================
//                                   Define pins
//================================================================================
//Dc Motors pins, front motor drivers has same pins as back motors for respective sides(left,right)
//Front Left Motor
#define flm_dir 32      //Digital
#define flm_en 14     //Digital
#define flm_pwm 7   //PWM
//Front Right Motor  
#define frm_dir 17 
#define frm_en 16
#define frm_pwm 3
//Middle
#define mlm_dir 21 
#define mlm_en 20
#define mlm_pwm 1 
#define mrm_dir 19
#define mrm_en 18
#define mrm_pwm 2 
//Back
 

//Ping Sensor pins according to connection on board
//side,left back sensor 1
#define trigPin1 22
#define echoPin1 23
//side,left sensor 2
#define trigPin2 29
#define echoPin2 31
//side,right  sensor 1
#define trigPin3 37
#define echoPin3 39
//side,right sensor 2
#define trigPin4 45
#define echoPin4 47
//Front Sensor facing downward
#define trigPin5 53
#define echoPin5 52
//back Sensor facing downward
#define trigPin6 46
#define echoPin6 44
//Front Sensor facing straight
#define trigPin7 38
#define echoPin7 36
    

//Recieve task controls from ESP in data
byte data[7]; 

void StopAll(){
  digitalWrite(flm_dir,LOW);
  digitalWrite(flm_en,LOW);
  digitalWrite(frm_dir,LOW);
  digitalWrite(frm_en,LOW);
  digitalWrite(mlm_dir,LOW);
  digitalWrite(mlm_en,LOW);
  digitalWrite(mrm_dir,LOW);
  digitalWrite(mrm_en,LOW);
  
}

void MoveForward(){
  digitalWrite(,LOW);
  digitalWrite(flm_en,HIGH);
  digitalWrite(frm_dir,LOW);
  digitalWrite(frm_en,HIGH);
  
}

void U_Turn(){
  //Uturn
  
}

float Read_Sensor(int sensor_number){
float sensor_value, dist ;
switch(sensor_number){
  case 1:
    digitalWrite(trigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1,LOW);
    sensor_value=pulseIn(echoPin1,HIGH);
    break;
  case 2:
    digitalWrite(trigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2,LOW);
    sensor_value=pulseIn(echoPin2,HIGH);
    break;
  case 3:
    digitalWrite(trigPin3,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin3,LOW);
    sensor_value=pulseIn(echoPin3,HIGH);
    break;
  case 4:
    digitalWrite(trigPin4,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin4,LOW);
    sensor_value=pulseIn(echoPin4,HIGH);
    break;
  case 5:
    digitalWrite(trigPin5,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin5,LOW);
    sensor_value=pulseIn(echoPin5,HIGH);
    break; 
  case 6:
    digitalWrite(trigPin6,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin6,LOW);
    sensor_value=pulseIn(echoPin6,HIGH);
    break; 
  case 7:
    digitalWrite(trigPin7,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin7,LOW);
    sensor_value=pulseIn(echoPin7,HIGH);
    break;        
}

if (sensor_value==0.0||sensor_value>=LARGE_SENSOR_READING){
    sensor_value=MAX_SENSOR_READING;
  }
  dist=sensor_value*DISTANCE_PER_SENSORVALUE;
  return dist;
}

float Dist_From_Wall(float val1, float val2){
    if(min(val1, val2) < max(val1 , val2) * 0.5 )
      return min(val1 , val2);
    return (val1 + val2)/2;
}

int Check_Region(float val1, float val2){
  int dist = Dist_From_Wall(val1 , val2);
  if(abs(dist-CONSTANT_DIST)>THRESH_DIST){
    if(dist>CONSTANT_DIST){
       return 2;
    }
    else{
      return 0;
    }
  }
  else
     return 1;
  
}



int Pid_Angle(float val1,float val2)
{
  angle_prev = angle_current;
  angle_current = val1-val2;
  total_error += (angle_current - angle_prev);
  if(angle_current == 0)
    return (0);
  else
         error_angle = kp * angle_current + ki * total_error + kd * (angle_current - angle_prev);        
//         Serial.print("correction: ");
//         Serial.println(error_angle);
         return(error_angle);
  }

float pid_dist(){
  float error_dist = k_dist * ((weight-1) *( Dist_From_Wall(sensor_val[0],sensor_val[1]) - CONSTANT_DIST ));

  return(error_dist);
} 

// int Check_Region(float val1, float val2){
//  int dist =Dist_From_Wall(val1 , val2);
//  if(dist < inter_distance -THRESH_DIST)
//    return 0;
//  if(dist > inter_distance +THRESH_DIST)
//    return 2;
//  else return 1;
//}
//



bool IsEdge(){
  sensor_val[4] = Read_Sensor(5);   //Front Sensor facing downward
  sensor_val[5] = Read_Sensor(6);   //Back Sensor facing Downward

  if(abs(sensor_val[4]-sensor_val[5])>DEPTH_SENSOR_DIFFERENCE){
    return(1);
  }
  else
  return(0);
  
}


//==================================================================================
//                            SETUP
//==================================================================================
void setup() {
  Serial.begin(9600);
  //Set up pins
  pinMode(flm_dir,OUTPUT);
  pinMode(flm_en,OUTPUT);
  pinMode(flm_pwm,OUTPUT);
  pinMode(frm_dir,OUTPUT);
  pinMode(frm_en,OUTPUT);
  pinMode(frm_pwm,OUTPUT);
  pinMode(mlm_dir,OUTPUT);
  pinMode(mlm_en,OUTPUT);
  pinMode(mlm_pwm,OUTPUT);
  pinMode(mrm_dir,OUTPUT);
  pinMode(mrm_en,OUTPUT);
  pinMode(mrm_pwm,OUTPUT);
  

  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(trigPin3,OUTPUT);
  pinMode(echoPin3,INPUT);
  pinMode(trigPin4,OUTPUT);
  pinMode(echoPin4,INPUT);
  pinMode(trigPin5,OUTPUT);
  pinMode(echoPin5,INPUT);
  pinMode(trigPin6,OUTPUT);
  pinMode(echoPin6,INPUT);
  pinMode(trigPin7,OUTPUT);
  pinMode(echoPin7,INPUT);


  //Initialize motors
  StopAll();
  
}

//======================================================================================
//                                         LOOP
//======================================================================================

void loop() {
  Serial.readBytes(data,7);
  control_movement =(int)data[0];
  
  if(control_movement){

    if(IsEdge()){
     //Edge detected 
     StopAll();
     total_error=0;
     U_Turn();
     FLAG=0;
    }
    
    else if(FLAG){
    
    //Move Parallel to wall
      MoveForward();
    
      sensor_val[0] = Read_Sensor(1);
      sensor_val[1] = Read_Sensor(2);
      angle_prev = angle_current;
      angle_current= sensor_val[1] - sensor_val[0];
      region=Check_Region(sensor_val[0], sensor_val[1]);
      Serial.print(region);
      if(region==0 or region==2){
        //analogWrite(blm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[0],sensor_val[1]) + pid_dist());
        analogWrite(flm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[0],sensor_val[1]) + pid_dist());
        
        //analogWrite(brm_pwm, speed_avg- Pid_Angle(sensor_val[0],sensor_val[1]) - pid_dist());
        analogWrite(frm_pwm, speed_avg- Pid_Angle(sensor_val[0],sensor_val[1]) - pid_dist());
      }
      else{
        if(abs(angle_current)>THRESH_ANGLE){
        //analogWrite(blm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[0],sensor_val[1]));
        analogWrite(flm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[0],sensor_val[1]));
        
        //analogWrite(brm_pwm, speed_avg- Pid_Angle(sensor_val[0],sensor_val[1]) );
        analogWrite(frm_pwm, speed_avg- Pid_Angle(sensor_val[0],sensor_val[1]) );
          }
        else{
          //analogWrite(blm_pwm, (speed_avg +calibration));
        analogWrite(flm_pwm, (speed_avg +calibration));
        
        //analogWrite(brm_pwm, speed_avg );
        analogWrite(frm_pwm, speed_avg);
      
        }
      }
    }
    
    else{
      //FLAG 0
      // Going back to starting point after u turn 
      sensor_val[0] = Read_Sensor(3);
      sensor_val[1] = Read_Sensor(4);
      angle_prev = angle_current;
      angle_current= sensor_val[4] - sensor_val[3];
      region=Check_Region(sensor_val[4], sensor_val[3]);
      Serial.print(region);
      if(region==0 or region==2){
        //analogWrite(blm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[3],sensor_val[4]) + pid_dist());
        analogWrite(flm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[3],sensor_val[4]) + pid_dist());
        
        //analogWrite(brm_pwm, speed_avg- Pid_Angle(sensor_val[3],sensor_val[4]) - pid_dist());
        analogWrite(frm_pwm, speed_avg- Pid_Angle(sensor_val[3],sensor_val[4]) - pid_dist());
      }
      else{
        if(abs(angle_current)>THRESH_ANGLE){
        //analogWrite(blm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[3],sensor_val[4]));
        analogWrite(flm_pwm, (speed_avg +calibration) + Pid_Angle(sensor_val[3],sensor_val[4]));
        
        //analogWrite(brm_pwm, speed_avg- Pid_Angle(sensor_val[3],sensor_val[4]) );
        analogWrite(frm_pwm, speed_avg- Pid_Angle(sensor_val[3],sensor_val[4]) );
          }
        else{
          //analogWrite(blm_pwm, (speed_avg +calibration));
        analogWrite(flm_pwm, (speed_avg +calibration));
        
        //analogWrite(brm_pwm, speed_avg );
        analogWrite(frm_pwm, speed_avg);
      
        }
      }
       
    }
    
    
  }
  else{
     StopAll(); 
  }
 
  
}
