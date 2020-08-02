//Serial Rx Tx data sender 
byte data[]={5,1,3,9};

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
}

void loop() {
  Serial.write(data,4); //Write the serial data
  delay(1000);
}
