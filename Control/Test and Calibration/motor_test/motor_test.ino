//Motor_Test

#define flm_dir 32     //Digital
#define flm_en 14     //Digital
#define flm_pwm 7   //PWM
//Front Right Motor  
#define frm_dir 17 
#define frm_en 16
#define frm_pwm 3
//Middle
#define mlm_dir 21 
#define mlm_en 20
#define mlm_pwm 5
#define mrm_dir 19
#define mrm_en 18
#define mrm_pwm 2 

int Speed_Main =140;

int Calibration=80;


void setup() {
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
  digitalWrite(flm_dir,LOW);
  digitalWrite(flm_en,LOW);
  digitalWrite(frm_dir,LOW);
  digitalWrite(frm_en,LOW);
  digitalWrite(mlm_dir,LOW);
  digitalWrite(mlm_en,LOW);
  digitalWrite(mrm_dir,LOW);
  digitalWrite(mrm_en,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  digitalWrite(flm_dir,HIGH);
  digitalWrite(flm_en,LOW);
  analogWrite(flm_pwm,Speed_Main+Calibration);
  digitalWrite(frm_dir,HIGH);
  digitalWrite(frm_en,LOW);
  analogWrite(frm_pwm,Speed_Main);
  
 

}
