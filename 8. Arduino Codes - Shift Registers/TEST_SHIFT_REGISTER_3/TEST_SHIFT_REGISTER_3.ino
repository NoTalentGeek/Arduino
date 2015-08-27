#include <Shifter.h>
#define SER 			5
#define RCLK			2
#define SRCLK			6
#define NUM_REGISTERS 	3
Shifter shifter(SER, RCLK, SRCLK, NUM_REGISTERS);
int delayCounter = 100;
int delayCounterSwitch = 1;

bool test = true;

void setup(){ Serial.begin(9600); }
void loop(){
	if(Serial.available() > 0){
		int testInt = Serial.read();
		if(testInt == 49){
			test = false;
		}
		else if(testInt == 50){
			test = true;
		}
	}
	Blink(test, 19);
}
void Blink(bool _on, int _pinIndex){
	if(_on){
		if(delayCounter == 100 || delayCounter == 0){
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
	else if(!_on){
		shifter.clear();
		shifter.write();
		delayCounter = 100;
		delayCounterSwitch = 1;
	}
}