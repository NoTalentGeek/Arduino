#define TILT_SWITCH 2
#define RANDOM_SEED A0
const int shakeDelayDefault = 100;
bool shakeStart 			= false;
bool shakeEnd 				= false;
bool startCountDown 		= false;
int amountOfValueChange 	= 0;
int buttonState;
int buttonStatePrevious;
int shakeDelay 				= shakeDelayDefault;
int shakeStartThreshold 	= 30;
void setup(){
	Serial.begin(9600);
	pinMode(TILT_SWITCH, INPUT);
}
void loop(){
	int rolledNumber = ShakeDice();
	Serial.println(rolledNumber);
}
int ShakeDice(){
	buttonState = digitalRead(TILT_SWITCH);
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
		randomSeed(digitalRead(RANDOM_SEED));
		int rolledNumber = random(1, 21);
		return rolledNumber;
	}
	else if(!shakeStart || !shakeEnd){
		return -1;
	}
}