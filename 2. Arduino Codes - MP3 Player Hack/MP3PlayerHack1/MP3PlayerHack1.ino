int mPin1 = 11;
int mPin2 = 12;
int val = 0;

void setup(){

	pinMode(mPin1, OUTPUT);
	pinMode(mPin2, OUTPUT);
	Serial.begin(9600);
	Serial.flush();

}

void loop(){

	if(Serial.available() > 0){

		val = Serial.read();

		Serial.println(val);

		if(val == 49){ MPOnOff(); }
		else if(val == 50){ MPPlayPause(); }

	}

}

void MPOnOff(){

	Serial.println("On/Off");
	digitalWrite(mPin1, HIGH);
	delay(5000);
	digitalWrite(mPin1, LOW);
	delay(100);

}

void MPPlayPause(){

	Serial.println("Play/Pause");
	digitalWrite(mPin2, HIGH);
	delay(100);
	digitalWrite(mPin2, LOW);
	delay(100);

}
