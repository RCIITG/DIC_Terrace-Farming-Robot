//Serial Rx-Tx Data reciever test
byte data[4]; 

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
}
//{start,pump,side,center,seeding,plough,harvest}
void loop() {
  Serial.readBytes(data,4); 
  Serial.print(data[0]);
  Serial.print("  ") ;
  Serial.print(data[1]);
  Serial.print("  "); 
  Serial.print(data[2]);
  Serial.print("  ") ;
  Serial.println(data[3]);
 
  
 
  
}
