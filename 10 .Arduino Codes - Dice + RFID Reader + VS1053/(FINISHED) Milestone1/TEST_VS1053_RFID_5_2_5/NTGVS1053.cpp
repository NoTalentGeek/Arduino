//Import all libraries necessary for this class.
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#include "Arduino.h"
#include "NTGRFIDReader.h"
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


NTGRFIDReader ntgRFIDReader;


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
int mp3Index 			= 1;
int mp3IndexPrevious	= 0;

//Constant variables of the limit of mp3Index.
const unsigned int mp3IndexMin	= 1;
const unsigned int mp3IndexMax	= 35;

/*
Buffer variables to convert integer to String to char array.
50 here is an arbitrary number.
But make sure that this number is long enough to holds any .mp3
	file name inside the SD card.
*/
char mp3IndexBufferCharArray[50];
String mp3IndexBufferString;

//Predetermined MP3 index char array.
char voiceExplanationNumber[] 	= "31.mp3";
char voiceOr[]					= "32.mp3";
char voicePleaseShakeTheDice[]	= "33.mp3";
char voicePleaseVisitAndTap[] 	= "34.mp3";
char voicePrime[] 				= "35.mp3";
char voiceWelcomeToExhibition[]	= "36.mp3";
char voiceWelcomeToMuseumX[] 	= "37.mp3";
char voiceYouNeedToGo[]			= "38.mp3";
char voiceYouRolled[]			= "39.mp3";


bool haveVisitedTargetExhibition = false;
int targetExhibitionDatabaseArray[30][4];
int targetExhibitionArray[2];

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

//Play button function.
void NTGVS1053::Play(char _index[]){ vs1053.playFullFile(_index); }
//Stop playing button function.
void NTGVS1053::Stop(){ vs1053.stopPlaying();}
//Pause/Unpause button function.
void NTGVS1053::PauseUnpause(bool _isPaused){
	if(_isPaused){ vs1053.pausePlaying(false); }
	else if(!_isPaused){ vs1053.pausePlaying(true); }
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
	mp3Index = NTGVS1053::BoundAndInvertValue(mp3Index, mp3IndexMin, mp3IndexMax);
	//Convert the integer to String.
	mp3IndexBufferString = String(mp3Index) + ".mp3";
	//Convert the String into character array.
	mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
	//Play the music based on the mp3 index in character array.
	Play(mp3IndexBufferCharArray);

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

	bool notStartOrBoss;
	int lastIndex;
	int startIndex[5] = {0, 6, 12, 18, 24};
	int bossIndex[5] = {5, 11, 17, 23, 29};
	for(int i = 0; i < 30; i ++){
		for(int j = 0; j < 4; j ++){
			for(int k = 0; k < 5; k ++){
				if(i == bossIndex[k] && i != 29){
					targetExhibitionDatabaseArray[i][0] = startIndex[k];
					targetExhibitionDatabaseArray[i][1] = startIndex[k + 1];
					targetExhibitionDatabaseArray[i][2] = startIndex[k];
					targetExhibitionDatabaseArray[i][3] = 999;
					lastIndex = targetExhibitionDatabaseArray[i][3];
					notStartOrBoss = true;
				}
				else if(i == 29){
					targetExhibitionDatabaseArray[i][0] = 0;
					targetExhibitionDatabaseArray[i][1] = 0;
					targetExhibitionDatabaseArray[i][2] = 0;
					targetExhibitionDatabaseArray[i][3] = 0;
					lastIndex = targetExhibitionDatabaseArray[i][3];
					notStartOrBoss = true;
				}
				else if(i == 0){
					targetExhibitionDatabaseArray[i][j] = j + 1;
					lastIndex = targetExhibitionDatabaseArray[i][j];
					notStartOrBoss = true;
				}
				else{
					notStartOrBoss = false;
				}
			}
			if(i < bossIndex[0] && !notStartOrBoss){
				int index = lastIndex + 1;
				if(index == i){ index ++; }
				if(index > bossIndex[0]){ index = startIndex[0]; }
				targetExhibitionDatabaseArray[i][j] = index;
				lastIndex = index;
			}
			else if(i < bossIndex[1] && !notStartOrBoss){
				int index = lastIndex + 1;
				if(index == i){ index ++; }
				if(index > bossIndex[1]){ index = startIndex[1]; }
				targetExhibitionDatabaseArray[i][j] = index;
				lastIndex = index;
			}
			else if(i < bossIndex[2] && !notStartOrBoss){
				int index = lastIndex + 1;
				if(index == i){ index ++; }
				if(index > bossIndex[2]){ index = startIndex[2]; }
				targetExhibitionDatabaseArray[i][j] = index;
				lastIndex = index;
			}
			else if(i < bossIndex[3] && !notStartOrBoss){
				int index = lastIndex + 1;
				if(index == i){ index ++; }
				if(index > bossIndex[3]){ index = startIndex[3]; }
				targetExhibitionDatabaseArray[i][j] = index;
				lastIndex = index;
			}
			else if(i < bossIndex[4] && !notStartOrBoss){
				int index = lastIndex + 1;
				if(index == i){ index ++; }
				if(index > bossIndex[4]){ index = startIndex[4]; }
				targetExhibitionDatabaseArray[i][j] = index;
				lastIndex = index;
			}
		}
	}

	/*
	for(int i = 0; i < 30; i ++){
		for(int j = 0; j < 4; j ++){
			Serial.print(targetExhibitionDatabaseArray[i][j]);
			Serial.print(" ");
		}
		Serial.println("");
	}
	*/

	ntgRFIDReader.NTGRFIDReaderSetup();
	vs1053.begin();
	vs1053.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
	SD.begin(CARDCS);
	//Set the default volume.
	vs1053.setVolume(20, 20);

	/*
	//Set the default .mp3 file.
	Play(voiceWelcomeToMuseumX); //Welcome to museum X.
	Play(voicePleaseVisitAndTap); //Please visit and tap into exhibition.
	Play(voicePrime); //Prime.
	*/

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
		mp3Index = ntgRFIDReader.NTGRFIDReaderLoop(vs1053.playingMusic);
		if(mp3Index != -1){

			if(mp3Index == 0){
				mp3IndexBufferString = voicePrime;
				mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
			}
			else if(mp3Index != 0){
				mp3IndexBufferString = String(mp3Index) + ".mp3";
				mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
			}

			if(mp3Index == targetExhibitionArray[0] || mp3Index == targetExhibitionArray[1]){ haveVisitedTargetExhibition = true; }
			else if(mp3Index != targetExhibitionArray[0] && mp3Index != targetExhibitionArray[1]){ haveVisitedTargetExhibition = false; }

			Play(voiceWelcomeToExhibition);
			Play(mp3IndexBufferCharArray);

			if(haveVisitedTargetExhibition){

				//Give explanation here.
				randomSeed(analogRead(A0));
				int explanationIndexRandom = random(1, 5);
				//Play(voiceExplanationNumber);
				mp3IndexBufferString = String(explanationIndexRandom) + ".mp3";
				mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
				//Play(mp3IndexBufferCharArray);

				//Roll and assign new target here.
				//Play(voicePleaseShakeTheDice);
				//Play(voiceYouRolled);
				randomSeed(analogRead(A0));
				//int targetExhibitionIndexRandom = random(1, 21);
				int targetExhibitionIndexRandom = 1;
				mp3IndexBufferString = String(targetExhibitionIndexRandom) + ".mp3";
				mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
				//Play(mp3IndexBufferCharArray);
				if(targetExhibitionIndexRandom <= 10){
					targetExhibitionArray[0] = targetExhibitionDatabaseArray[mp3Index][0];
					targetExhibitionArray[1] = targetExhibitionDatabaseArray[mp3Index][1];

				}
				else if(targetExhibitionIndexRandom > 10){
					targetExhibitionArray[0] = targetExhibitionDatabaseArray[mp3Index][2];
					targetExhibitionArray[1] = targetExhibitionDatabaseArray[mp3Index][3];
				}
				Play(voicePleaseVisitAndTap);

				if(targetExhibitionArray[0] != 0){

					mp3IndexBufferString = String(targetExhibitionArray[0]) + ".mp3";
					mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
					Play(mp3IndexBufferCharArray);

				}
				else{
					Play(voicePrime);
				}

				Play(voiceOr);

				if(targetExhibitionArray[1] != 0){

					mp3IndexBufferString = String(targetExhibitionArray[1]) + ".mp3";
					mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
					Play(mp3IndexBufferCharArray);

				}
				else{
					Play(voicePrime);
				}

				haveVisitedTargetExhibition = false;

			}
			else if(!haveVisitedTargetExhibition){

				Play(voicePleaseVisitAndTap);

				if(targetExhibitionArray[0] != 0){

					mp3IndexBufferString = String(targetExhibitionArray[0]) + ".mp3";
					mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
					Play(mp3IndexBufferCharArray);

				}
				else{
					Play(voicePrime);
				}

				Play(voiceOr);

				if(targetExhibitionArray[1] != 0){

					mp3IndexBufferString = String(targetExhibitionArray[1]) + ".mp3";
					mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 50);
					Play(mp3IndexBufferCharArray);

				}
				else{
					Play(voicePrime);
				}
				
			}

		}

	}

}