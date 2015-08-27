#define PLAY_STOP		A0
#define PAUSE_UNPAUSE	A1
#define NEXT			A2
#define PREVIOUS		A3
#define	VOL_UP			A4
#define VOL_DOWN		A5
bool isButtonPlayStop		= false;
bool isButtonPauseUnpause	= false;
bool isButtonNext			= false;
bool isButtonPrevious		= false;
bool isButtonVolUp			= false;
bool isButtonVolDown		= false;
bool IsButtonPressed(bool _isButton, int _buttonPin, void (*callBackFunction)()){
	int buttonValue = analogRead(_buttonPin);
	if(buttonValue == 1023){
		if(!_isButton){
			(*callBackFunction)();
			_isButton = true;
		}
	}
	else if(buttonValue == 0){
		_isButton = false;
	}
	return _isButton;
}
void TestCallbackFunction(){

	Serial.println("Button just pressed.");

}
void setup(){
	Serial.begin(9600);
}
void loop(){
	isButtonPlayStop		= IsButtonPressed(isButtonPlayStop, PLAY_STOP, &TestCallbackFunction);
	isButtonPauseUnpause	= IsButtonPressed(isButtonPauseUnpause, PAUSE_UNPAUSE, &TestCallbackFunction);
	isButtonNext			= IsButtonPressed(isButtonNext, NEXT, &TestCallbackFunction);
	isButtonPrevious		= IsButtonPressed(isButtonPrevious, PREVIOUS, &TestCallbackFunction);
	isButtonVolUp			= IsButtonPressed(isButtonVolUp, VOL_UP, &TestCallbackFunction);
	isButtonVolDown			= IsButtonPressed(isButtonVolDown, VOL_DOWN, &TestCallbackFunction);
}