//side,left back sensor 1
//Ping sensor reading test with pcb board


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

float DISTANCE_PER_SENSORVALUE=.017;
int MAX_SENSOR_READING=4000;
int LARGE_SENSOR_READING=15000;

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




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(Read_Sensor(1));
  Serial.print("\t");
  Serial.print(Read_Sensor(2));
  Serial.print("\t");
  Serial.print(Read_Sensor(3));
  Serial.print("\t");
  Serial.print(Read_Sensor(4));
  Serial.print("\t");
  Serial.println(Read_Sensor(5));

}
