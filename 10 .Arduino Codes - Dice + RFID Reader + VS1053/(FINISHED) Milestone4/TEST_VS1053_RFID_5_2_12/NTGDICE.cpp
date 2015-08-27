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

const int shakeDelayDefault = 100;
bool shakeStart 			= false;
bool shakeEnd 				= false;
bool startCountDown 		= false;
int amountOfValueChange 	= 0;
int buttonState;
int buttonStatePrevious = 0;
int rolledNumber			= -1;
int shakeDelay 				= shakeDelayDefault;
int shakeStartThreshold 	= 30;

NTGDICE::NTGDICE(){}

void NTGDICE::NTGDICESetup(){
}
bool shakeTrigger = false;
int rolledNumberPrevious = -1;
int rolledNumberCounter = 0;
int NTGDICE::NTGDICELoop(){
	bool dice = ShakeDice();
	//Serial.println(dice);
	if(dice && !shakeTrigger){
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
                return rolledNumber = random(0, 20);
				shakeTrigger = false;
			}
		}
	}

	if(rolledNumber != -1){
		shifter.setPin(rolledNumber, HIGH);
		shifter.write();
	}

	if((rolledNumber != rolledNumberPrevious) && rolledNumber != -1){
		rolledNumberPrevious = rolledNumber;
		rolledNumberCounter --;
	}
	if((rolledNumber == rolledNumberPrevious) && rolledNumber != -1){
		rolledNumberCounter ++;
	}

	if(rolledNumberCounter >= 20){
		rolledNumberCounter = 0;
		int rolledNumberTemp = rolledNumber;
		rolledNumber = -1;
		return rolledNumberTemp;
	}
	else{
		return -1;
	}
}

bool NTGDICE::ShakeDice(){
	//buttonState = analogRead(TILT_SWITCH);
	if(analogRead(TILT_SWITCH) != buttonStatePrevious){ amountOfValueChange ++; startCountDown = true; }
	if(startCountDown){
		if(buttonState != buttonStatePrevious){
			amountOfValueChange ++;
			shakeDelay = shakeDelayDefault;
		}
		shakeDelay --;
	}
Serial.print(analogRead(TILT_SWITCH));
Serial.print(" ");
        Serial.println(amountOfValueChange);
	buttonStatePrevious = analogRead(TILT_SWITCH);
	if(amountOfValueChange > shakeStartThreshold){ return shakeStart = true; }
/*
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
		return true;
	}
*/
	else{ return false; }
}
