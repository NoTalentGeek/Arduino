//Import all libraries necessary for this class.
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#include "Arduino.h"
#include "NTGVS1053.h"
//Define VS1053 pins.
#define BREAKOUT_RESET	9
#define BREAKOUT_CS		10
#define BREAKOUT_DCS	8
#define SHIELD_RESET	-1
#define SHIELD_CS		7
#define SHIELD_DCS		6
#define CARDCS			4
#define DREQ			3
//Define push buttons pins.
#define PLAY_STOP		A0
#define PAUSE_UNPAUSE	A1
#define NEXT			A2
#define PREVIOUS		A3
#define	VOL_UP			A4
#define VOL_DOWN		A5

//Object variable for the VS1053.
Adafruit_VS1053_FilePlayer vs1053 = Adafruit_VS1053_FilePlayer(
	BREAKOUT_RESET,
	BREAKOUT_CS,
	BREAKOUT_DCS,
	DREQ,
	CARDCS
);

/*
mp3Index is the index of an .mp3 file inside the SD card.
mp3IndexPrevious is a temporary variable to detect change in
	mp3Index.
*/
unsigned int mp3Index = 1;
unsigned int mp3IndexPrevious = 0;

//Constant variables of the limit of mp3Index.
const unsigned int mp3IndexMin = 1;
const unsigned int mp3IndexMax = 3;

/*
Buffer variables to convert integer to String to char array.
50 here is an arbitrary number.
But make sure that this number is long enough to holds any .mp3
	file name inside the SD card.
*/
char mp3IndexBufferCharArray[50];
String mp3IndexBufferString;

//Push button variables.
bool isButtonPlayStop		= false;
bool isButtonPauseUnpause	= false;
bool isButtonNext			= false;
bool isButtonPrevious		= false;
bool isButtonVolUp			= false;
bool isButtonVolDown		= false;

NTGVS1053::NTGVS1053(){}

//Polymorph functions of physical buttons control.
bool NTGVS1053::IsButtonPressed(bool _isButton, int _buttonPin, void (*callBackFunction)()){
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
bool NTGVS1053::IsButtonPressed(bool _isButton, char _charArray[], int _buttonPin, void (*callBackFunction)(char[])){
	int buttonValue = analogRead(_buttonPin);
	if(buttonValue == 1023){
		if(!_isButton){
			(*callBackFunction)(_charArray);
			_isButton = true;
		}
	}
	else if(buttonValue == 0){
		_isButton = false;
	}
	return _isButton;
}
bool NTGVS1053::IsButtonPressed(bool _isButton, bool _argumentBool, int _buttonPin, void (*callBackFunction)(bool)){
	int buttonValue = analogRead(_buttonPin);
	if(buttonValue == 1023){
		if(!_isButton){
			(*callBackFunction)(_argumentBool);
			_isButton = true;
		}
	}
	else if(buttonValue == 0){
		_isButton = false;
	}
	return _isButton;
}

//Function to set and control all physical buttons.
void NTGVS1053::PhysicalButton(bool _isPlayingMusic){

	if(_isPlayingMusic){ isButtonPlayStop = IsButtonPressed(isButtonPlayStop, PLAY_STOP, &Stop); }
	else if(!_isPlayingMusic){ isButtonPlayStop = IsButtonPressed(isButtonPlayStop, mp3IndexBufferCharArray, PLAY_STOP, &Play); }
	isButtonPauseUnpause	= IsButtonPressed(isButtonPauseUnpause, vs1053.paused(), PAUSE_UNPAUSE, &PauseUnpause);
	isButtonNext			= IsButtonPressed(isButtonNext, true, NEXT, &NextPrevious);
	isButtonPrevious		= IsButtonPressed(isButtonPrevious, false, PREVIOUS, &NextPrevious);
	isButtonVolUp			= IsButtonPressed(isButtonVolUp, VOL_UP, &VolUp);
	isButtonVolDown			= IsButtonPressed(isButtonVolDown, VOL_DOWN, &VolDown);

}

void NTGVS1053::Play(char _index[]){

	/*
	Checking the audio file availbility when the audio file is not found then
		halt the system.
	*/
	if(!vs1053.startPlayingFile(_index)){
		Serial.println("Could not play " + String(_index));
		while(1);
	}
	Serial.println("Play button pressed.");
	Serial.println(String(_index) + " is currently being played.");

}
//Stop playing button function.
void NTGVS1053::Stop(){

	vs1053.stopPlaying();
	Serial.println("Stop button just pressed.");

}
//Pause/Unpause button function.
void NTGVS1053::PauseUnpause(bool _isPaused){

	if(_isPaused){
		vs1053.pausePlaying(false);
		Serial.println("Unpause button just pressed.");
	}
	else if(!_isPaused){
		vs1053.pausePlaying(true);
		Serial.println("Pause button just pressed.");
	}

}
//Next/Previous button function.
void NTGVS1053::NextPrevious(bool _isNextButton){

	//Stop the currently played audio file.
	vs1053.stopPlaying();
	//Add or substract one point from mp3Index.
	if(_isNextButton){ mp3Index ++; }
	else if(!_isNextButton){ mp3Index --; }
	/*
	Just incase the mp3Index is beyond the available file, so we need to
		bound the value of mp3Index.
	*/
	mp3Index = BoundAndInvertValue(mp3Index, mp3IndexMin, mp3IndexMax);
	//Convert the integer to String.
	mp3IndexBufferString = String(mp3Index) + ".mp3";
	//Convert the String into character array.
	mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
	//Play the music based on the mp3 index in character array.
	Play(mp3IndexBufferCharArray);
	if(_isNextButton){ Serial.println("Next button just pressed."); }
	else if(!_isNextButton){ Serial.println("Previous button just pressed."); }

}
//PENDING.
void NTGVS1053::VolUp(){}
//PENDING.
void NTGVS1053::VolDown(){}

/*
If there is a value that go beyond maximum or minimum limit
	revert it back to its minimum or maximum limit.
*/
int NTGVS1053::BoundAndInvertValue(int _value, int _minValue, int _maxValue){

	if(_value > _maxValue){ _value = _minValue; }
	else if(_value < _minValue){ _value = _maxValue; }
	return _value;
	
}

/*
Original PrintFileDirectory from the Adafruit VS1053 library.
The purpose of this function is to show every file inside the Serial console.
*/
void NTGVS1053::DebugPrintFileDirectory(File _dir, int _numTabs){

	while(true){
		File entry = _dir.openNextFile();
		if(!entry){ break; }
		for(uint8_t i = 0; i < _numTabs; i ++){
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if(entry.isDirectory()){
			Serial.println("/");
			DebugPrintFileDirectory(entry, _numTabs + 1);
		}
		else{
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}

}

//For this prototype I use Serial input to activate the button.
void NTGVS1053::DebugSerialButton(bool _isPlayingMusic){

	if(Serial.available() > 0){
		int mode = Serial.read();
		if(mode == 49){
			if(_isPlayingMusic){ Stop(); }
			else if(!_isPlayingMusic){
				Play(mp3IndexBufferCharArray);
			}
		}
		else if(mode == 50){
			PauseUnpause(vs1053.paused());
		}
		else if(mode == 51){ NextPrevious(true); }
		else if(mode == 52){ NextPrevious(false); }
		else if(mode == 53){ /*PENDING: Volume button.*/ }
		else if(mode == 54){ /*PENDING: Volume button.*/ }
	}

}

void NTGVS1053::NTGVS1053Setup(){

	Serial.println(
		"VS1053 class setup."
	);

	//Verify all components.
	if(!vs1053.begin()){
		Serial.println("VS1053 is not connected.");
		while(1);
	}
	Serial.println("VS1053 is connected.");
	if(!vs1053.useInterrupt(VS1053_FILEPLAYER_PIN_INT)){
		Serial.println("VS1053 DREQ pin is not connected to interrupt pin.");
		while(1);
	}
	Serial.println("VS1053 DREQ pin is connected to interrupt pin.");
	if(!SD.begin(CARDCS)){
		Serial.println(
			"SD card slot is not connected."
		);
		while(1);
	}
	Serial.println("SD card slot is connected.");

	//Print the directory inside the Sd card into Serial console.
	DebugPrintFileDirectory(SD.open("/"), 0);
	//Set the default volume.
	vs1053.setVolume(20, 20);
	//Set the default .mp3 file.
	mp3IndexBufferString = String(mp3Index) + ".mp3";
	mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
	Play(mp3IndexBufferCharArray);

}

/*
The loop() override function is basically deivided between these two while loops.
The first while loop happen when there is an audio file playing and the second
	while loop happen when there is no audio file playing.
*/
void NTGVS1053::NTGVS1053Loop(){

	while(vs1053.playingMusic){
		PhysicalButton(vs1053.playingMusic);
		DebugSerialButton(vs1053.playingMusic);
	}
	while(!vs1053.playingMusic){
		PhysicalButton(vs1053.playingMusic);
		DebugSerialButton(vs1053.playingMusic);
	}

}