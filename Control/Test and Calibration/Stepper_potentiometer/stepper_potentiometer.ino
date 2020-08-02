const int stepPin = 3; 
const int dirPin = 2;
// 400 steps motor 
#define nosteps 400

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(A1,INPUT);
  Serial.begin(9600);


  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  
 for(int x = 0; x <= 133; x++) {  // for rotating 120 degree
  // these 4 lines make a single step
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(100); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(100); 
    
    Serial.println(x);
    int val = analogRead(A1);
    Serial.print("value of potentiometer : ");
    Serial.println(val);
    //delay(2000);
  }
  delay(2000); // One second delay
}
void loop(){
  
}
