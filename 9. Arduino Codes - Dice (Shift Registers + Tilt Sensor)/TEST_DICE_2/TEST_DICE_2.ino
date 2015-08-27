#include <Shifter.h>
#include "NTGDICE.h"
NTGDICE ntgDice;
int randomNumber;
void setup(){
	Serial.begin(9600);
	ntgDice.NTGDICESetup();
}
void loop(){
	randomNumber = ntgDice.NTGDICELoop();
        Serial.println(analogRead(A5));
	if(randomNumber != -1){
		Serial.println(randomNumber);
	}
}
