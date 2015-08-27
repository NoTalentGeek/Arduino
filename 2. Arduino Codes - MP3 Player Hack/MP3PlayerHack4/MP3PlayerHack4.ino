int val = 0;
void setup(){
  Serial.begin(9600);
  pinMode(8,  OUTPUT);
  pinMode(9,  OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}
void loop(){
  if(Serial.available() > 0){
    val = Serial.read();
    Serial.println(val);
    if(val == 49){ Play(); }
    if(val == 50){ Next(); }
    if(val == 51){ Previous(); }
    if(val == 52){ VolPlus(); }
    if(val == 53){ VolMin(); }
  }
}
void Play(){
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);
  delay(100);
  Serial.println("Play");
}
void Next(){
  digitalWrite(9, HIGH);
  delay(100);
  digitalWrite(9, LOW);
  delay(100);
  Serial.println("Next");
}
void Previous(){
  digitalWrite(10, HIGH);
  delay(100);
  digitalWrite(10, LOW);
  delay(100);
  Serial.println("Previous");
}
void VolPlus(){
  digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(11, LOW);
  delay(100);
  Serial.println("VolPlus");
}
void VolMin(){
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  delay(100);
  Serial.println("VolMin");
}
