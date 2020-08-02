//potentiometer test to control pwm of dc motor for seeding

#define in1 7
#define in2 8
#define value 9
#define output A0
int val;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(value, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(output,INPUT);

}

void loop() {
  val= analogRead(output);
  Serial.println(val);
  digitalWrite(in1,LOW);
  digitalWrite(in2, HIGH);
  analogWrite(value,val/4.1);

}
