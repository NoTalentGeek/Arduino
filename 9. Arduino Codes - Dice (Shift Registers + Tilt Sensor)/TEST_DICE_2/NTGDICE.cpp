#include <Shifter.h>
#include "Arduino.h"
#include "NTGDICE.h"
#define TILT_SWITCH 	A5
#define RANDOM_SEED 	A0
#define SER 			5
#define RCLK			2
#define SRCLK			6
#define NUM_REGISTERS 	3

Shifter shifter(SER, RCLK, SRCLK, NUM_REGISTERS);
int delayCounter = 100;
int delayCounterMax = 100;
int delayCounterSwitch = 1;

const int shakeDelayDefault = 100;
bool shakeStart 			= false;
bool shakeEnd 				= false;
bool startCountDown 		= false;
int amountOfValueChange 	= 0;
int buttonState;
int buttonStatePrevious;
int rolledNumber			= -1;
int shakeDelay 				= shakeDelayDefault;
int shakeStartThreshold 	= 10;

NTGDICE::NTGDICE(){}

void NTGDICE::NTGDICESetup(){
	pinMode(TILT_SWITCH, INPUT);
}
bool shakeTrigger = false;
int rolledNumberPrevious = -1;
int rolledNumberCounter = 0;
int NTGDICE::NTGDICELoop(){
	if(ShakeDice() != -1 && !shakeTrigger){
		shakeTrigger = true;
	}

	if(shakeTrigger){
		for(int i = 0; i < 20; i ++){
			rolledNumber = random(0, 20);
			shifter.setPin(rolledNumber, HIGH);
			shifter.write();
			delay(100);
			shifter.clear();
			shifter.write();
			if(i == 19){
				shakeTrigger = false;
			}
		}
	}

	if(rolledNumber != -1){
		Blink(rolledNumber);
	}

	if((rolledNumber != rolledNumberPrevious) && rolledNumber != -1){
		rolledNumberPrevious = rolledNumber;
		rolledNumberCounter --;
	}
	if((rolledNumber == rolledNumberPrevious) && rolledNumber != -1){
		rolledNumberCounter ++;
	}

	if(rolledNumberCounter >= 20){
		return rolledNumber;
	}
	else{
		return -1;
	}
}

int NTGDICE::ShakeDice(){
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
	else{ return -1; }
}

void NTGDICE::Blink(int _pinIndex){
	if(delayCounter == delayCounterMax || delayCounter == 0){
		delayCounterSwitch = delayCounterSwitch*-1;
	}
	if(delayCounterSwitch == -1){
		shifter.clear();
		shifter.write();
		shifter.setPin(_pinIndex, HIGH);
		shifter.write();
	}
	else if(delayCounterSwitch == 1){
		shifter.clear();
		shifter.write();
		shifter.setPin(_pinIndex, LOW);
		shifter.write();
	}
	delayCounter = delayCounter + delayCounterSwitch;
}