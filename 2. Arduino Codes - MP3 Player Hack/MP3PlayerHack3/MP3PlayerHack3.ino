int val = 0;
void setup(){
  Serial.begin(9600);
  pinMode(8, OUTPUT);
}
void loop(){
  if(Serial.available() > 0){
    val = Serial.read();
    Serial.println(val);
    if(val == 49){
      Pin8();
    }
  }
}
void Pin8(){
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);
  delay(100);
  Serial.println("TEST");
}
