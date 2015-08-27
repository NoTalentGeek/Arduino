int PLAY          = 8;
int CTL_NEXT      = 9;
int CTL_PREV      = 10;
int VOL_MINUS     = 11;  
int VOL_PLUS      = 12;  

int LLED          = 13;
int delayMilliS   = 100;

/*
Old functions.
void NextMP3(){

	PORTB = 0x00;
	DDRB = 0;
	DDRD = 0x40;
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //1.
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //2.
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //3.
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //4.
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //5.
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //6.
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //7.
	while((PINB & 0x01) != 0) PORTD = 0x40;
	while((PINB & 0x01) == 0) PORTD = 0; //8.
	DDRD = 0; //PORTD an input again.

}

void PrevMP3(){

	PORTB = 0x00;
	DDRB = 0;
	DDRD = 0xA0;
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //1.
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //2.
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //3.
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //4.
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //5.
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //6.
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //7.
	while((PINB & 0x01)!=0) PORTD = 0x80;
	while((PINB & 0x01)==0) PORTD = 0; //8.
	DDRD = 0; //PORTD an input again.

}
*/

void NextMP3(){

	digitalWrite(CTL_NEXT, HIGH);
	delay(delayMilliS);
	digitalWrite(CTL_NEXT, LOW); 

}

void PrevMP3(){

	digitalWrite(CTL_PREV, HIGH);
	delay(delayMilliS);
	digitalWrite(CTL_PREV, LOW);

}

void PlayPause(){

	pinMode(PLAY, OUTPUT);
	delay(2000);
	pinMode(PLAY, INPUT);
	delay(8000);

}

void FlashTheLEDs(){

	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);
	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);
	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);
	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);
	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);
	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);
	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);
	digitalWrite(LLED,HIGH);
	delay(delayMilliS);
	digitalWrite(LLED,LOW);
	delay(delayMilliS);

}

void setup(){               
 
	Serial.begin(9600);

	pinMode(PLAY, INPUT);
	digitalWrite(PLAY, LOW);

	pinMode(VOL_MINUS, INPUT);
	digitalWrite(VOL_MINUS, LOW);

	pinMode(VOL_PLUS, INPUT);
	digitalWrite(VOL_PLUS, LOW);

	pinMode(CTL_PREV, OUTPUT);
	digitalWrite(CTL_PREV, LOW);

	pinMode(CTL_NEXT, OUTPUT);
	digitalWrite(CTL_NEXT, LOW);

	pinMode(LLED,OUTPUT);

}

void loop(){

	if(Serial.available() > 0){

		if(Serial.read() == 49){ PlayPause(); }
		if(Serial.read() == 50){ NextMP3(); }
		if(Serial.read() == 51){ PrevMP3(); }

	}

}
