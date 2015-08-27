int serialRead 		= 0;

int pinPlayPause 	= 8;
int pinNext 		= 9;
int pinPrevious 	= 10;
int pinVolUp 		= 11;
int pinVolDown 		= 12;

int delayMilliS 	= 100;

void setup(){

	Serial.begin(9600);
	int i = pinPlayPause;
	while(i <= pinVolDown){

		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
		i ++;

	}

}

void loop(){

	if(Serial.available() > 0){

		serialRead = Serial.read();
		println(serialRead);

		//1 keyboard input on Serial Monitor.
		if(serialRead == 49){ MP3Button(pinPlayPause, "PlayPause"); }
		//2 keyboard input on Serial Monitor.
		if(serialRead == 50){ MP3Button(pinNext, "Next"); }
		//3 keyboard input on Serial Monitor.
		if(serialRead == 51){ MP3Button(pinPrevious, "Previous"); }
		//4 keyboard input on Serial Monitor.
		if(serialRead == 52){ MP3Button(pinVolPlus, "VolPlus"); }
		//5 keyboard input on Serial Monitor.
		if(serialRead == 53){ MP3Button(pinVolMin, "VolMin"); }

	}

}

void MP3Button(int _pin, String _buttonFunction){

	digitalWrite(_pin, HIGH);
	delay(delayMilliS);
	digitalWrite(_pin, LOW);
	delay(delayMilliS);
	Serial.println(_buttonFunction);

}
