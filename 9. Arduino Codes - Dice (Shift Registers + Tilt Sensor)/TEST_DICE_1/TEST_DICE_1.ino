#include <Shifter.h>
#define TILT_SWITCH 	A5
#define RANDOM_SEED 	A0
#define SER 			5
#define RCLK			2
#define SRCLK			6
#define NUM_REGISTERS 	3

Shifter shifter(SER, RCLK, SRCLK, NUM_REGISTERS);
int delayCounter1 = 100;
int delayCounter1Max = 100;
int delayCounter1Switch = 1;

const int shakeDelayDefault = 100;
bool shakeStart 			= false;
bool shakeEnd 				= false;
bool startCountDown 		= false;
int amountOfValueChange 	= 0;
int buttonState;
int buttonStatePrevious;
int rolledNumber			= -1;
int shakeDelay 				= shakeDelayDefault;
int shakeStartThreshold 	= 30;

bool delayCounter2Start = false;
int delayCounter2 = 1000;
int delayCounter2Max = 1000;
int delayCounter2Switch = 1;

void setup(){
	Serial.begin(9600);
	pinMode(TILT_SWITCH, INPUT);
}

void loop(){

	if(!delayCounter2Start && rolledNumber == -1){
		rolledNumber = ShakeDice();
	}
	if(rolledNumber != -1){
		delayCounter2Start = true;
		if(delayCounter2 == delayCounter2Max || delayCounter2 == 0){
			delayCounter2Switch = delayCounter2Switch*-1;
		}
		if(delayCounter2Switch == 1){
			rolledNumber = -1;
			delayCounter2Start = false;
			delayCounter2Switch = -1;
			delayCounter2 = delayCounter2Max;
		}
		delayCounter2 = delayCounter2 + delayCounter2Switch;
	}
	Blink(delayCounter2Start, rolledNumber);
	Serial.println(rolledNumber);

}


int ShakeDice(){
	buttonState = analogRead(TILT_SWITCH);
	if(buttonState != buttonStatePrevious){ startCountDown = true; }
	if(startCountDown){
		if(buttonState != buttonStatePrevious){
			amountOfValueChange ++;
			shakeDelay = shakeDelayDefault;
		}
		shakeDelay --;
	}
	if(amountOfValueChange > shakeStartThreshold){ shakeStart = true; }
	if(shakeDelay <= 0){
		shakeDelay = shakeDelayDefault;
		amountOfValueChange = 0;
		shakeEnd = true;
		startCountDown = false;
	}
	buttonStatePrevious = buttonState;
	if(shakeStart && shakeEnd){
		shakeStart = false;
		shakeEnd = false;
		randomSeed(analogRead(RANDOM_SEED));
		int rng = random(0, 20);
		return rng;
	}
	else if(!shakeStart || !shakeEnd){
		return -1;
	}
}

void Blink(bool _on, int _pinIndex){
	if(_on){
		if(delayCounter1 == delayCounter1Max || delayCounter1 == 0){
			delayCounter1Switch = delayCounter1Switch*-1;
		}
		if(delayCounter1Switch == -1){
			shifter.clear();
			shifter.write();
			shifter.setPin(_pinIndex, HIGH);
			shifter.write();
		}
		else if(delayCounter1Switch == 1){
			shifter.clear();
			shifter.write();
			shifter.setPin(_pinIndex, LOW);
			shifter.write();
		}
		delayCounter1 = delayCounter1 + delayCounter1Switch;
	}
	else if(!_on){
		shifter.clear();
		shifter.write();
		delayCounter1 = delayCounter1Max;
		delayCounter1Switch = 1;
	}
}