//Inter IIT - DIC Event
//Arduino Micro
//harvesting seeding, up down mechanism

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





void setup() {
  Serial.begin(9600);
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
  
  digitalWrite(harvest_1_in1, HIGH);
    digitalWrite(harvest_1_in2, LOW);
    digitalWrite(harvest_2_in1, HIGH);
    digitalWrite(harvest_2_in2, LOW);
    analogWrite(harvest_1_pwm,200);
    analogWrite(harvest_2_pwm,200);

    digitalWrite(seeding_in1, HIGH);
    digitalWrite(seeding_in2, LOW);
    analogWrite(seeding_pwm,200);

    digitalWrite(up_down_in1, HIGH);
    digitalWrite(up_down_in2, LOW);
    analogWrite(up_down_pwm, 200);

}
