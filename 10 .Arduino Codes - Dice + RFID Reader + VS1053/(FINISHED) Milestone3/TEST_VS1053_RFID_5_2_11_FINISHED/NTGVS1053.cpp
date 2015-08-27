//Import all libraries necessary for this class.
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#include "Arduino.h"
#include "NTGDICE.h"
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
#define REPEAT			A4
#define	VOL_UP			A3
#define VOL_DOWN		A2
//Define analog pin to give random seed.
#define RANDOM_SEED		A0

NTGDICE ntgDice;
NTGRFIDReader ntgRFIDReader;

//Object variable for the VS1053.
Adafruit_VS1053_FilePlayer vs1053 = Adafruit_VS1053_FilePlayer(
	BREAKOUT_RESET,
	BREAKOUT_CS,
	BREAKOUT_DCS,
	DREQ,
	CARDCS
);

//mp3Index is the index of an .mp3 file inside the SD card.
int mp3Index 					= 1;
//Constant variables of the limit of mp3Index.
const unsigned int mp3IndexMin	= 1;
const unsigned int mp3IndexMax	= 39;

/*
Buffer variables to convert integer to String to char array.
5 here is an arbitrary number.
But make sure that this number is long enough to holds any .mp3
	file name inside the SD card.
*/
char mp3IndexBufferCharArray[10];
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

//Repeat variable.
//bool isRepeat = false;
//char *repeatStackCharArray[10];

//Volume boundaries.
int volume = 20;
const int volumeMinimum 	= 40;
const int volumeMaximum 	= 0;
const int volumeResolution 	= 1;

//Boolean for indicating whether or not player/visitor visited the correct exhibition.
bool haveVisitedTargetExhibition = false;
//Bolean to check whether or not the prime exhibition has been visited or not.
bool isPrimeExhibitionVisited = false;
//This two dimensional array is used to store the target exhibition from the currently properly visited exhibition.
int targetExhibitionDatabaseArray[30][4];
//There is only two target exhibitions of which player/visitor can choose to go.
int targetExhibitionArray[2];

//Push button variables.
bool isButtonRepeat			= false;
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

//Function to set and control all physical buttons.
void NTGVS1053::PhysicalButton(bool _isPlayingMusic){

	//isButtonRepeat			= IsButtonPressed(isButtonVolUp, REPEAT, &Repeat);
	isButtonVolUp			= IsButtonPressed(isButtonVolUp, VOL_UP, &VolUp);
	isButtonVolDown			= IsButtonPressed(isButtonVolDown, VOL_DOWN, &VolDown);

}

//Play button function.
void NTGVS1053::Play(char _index[]){ 
	vs1053.startPlayingFile(_index);
	while(vs1053.playingMusic){
		isButtonVolUp		= IsButtonPressed(isButtonVolUp, VOL_UP, &VolUp);
		isButtonVolDown		= IsButtonPressed(isButtonVolDown, VOL_DOWN, &VolDown);
	}
}
/*
void NTGVS1053::Repeat(){
	for(int i = 0; i < 10; i ++){
		if(repeatStackCharArray[i] != "Empty."){ Play(repeatStackCharArray[i]); }
	}
}
*/
void NTGVS1053::VolUp(){
	volume = volume - volumeResolution;
	if(volume <= volumeMaximum){ volume = volumeMaximum; }
	vs1053.setVolume(volume, volume);
}
void NTGVS1053::VolDown(){ 
	volume = volume + volumeResolution;
	if(volume >= volumeMinimum){ volume = volumeMinimum; }
	vs1053.setVolume(volume, volume);
}

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
//Reset all saved voices char array.
void NTGVS1053::ResetRepeatStackCharArray(){
	isRepeat = true;
	for(int i = 0; i < 10; i ++){
		repeatStackCharArray[i] = "Empty.";
	}
	isRepeat = false;
}
*/

void NTGVS1053::NTGVS1053Setup(){

	//Setup the destination value for each exhibition using this algorithm below.
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

				if(i < bossIndex[k] && !notStartOrBoss){
					int index = lastIndex + 1;
					if(index == i){ index ++; }
					if(index > bossIndex[k]){ index = startIndex[k]; }
					targetExhibitionDatabaseArray[i][j] = index;
					lastIndex = index;
					break;
				}

			}
		}
	}

	/*
	Uncomment this code if you want to show the exhibition destination from the
		Serial console.
	Otherwise, please comment this, due to Serial class take so much space in Arduino.
	*/
	/*
	for(int i = 0; i < 30; i ++){
		for(int j = 0; j < 4; j ++){
			Serial.print(targetExhibitionDatabaseArray[i][j]);
			Serial.print(" ");
		}
		Serial.println("");
	}
	*/
	
	ntgDice.NTGDICESetup();

	//Setup the RFID Reader using my shitty self - made library.
	ntgRFIDReader.NTGRFIDReaderSetup();
	//Initiate connection to VS1053 chip.
	vs1053.begin();
	//Tell the VS1053 chip to use connection to controller interrupt pin.
	vs1053.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
	//Initiate connection to SD Card shield to controller.
	SD.begin(CARDCS);
	//Set the default volume.
	vs1053.setVolume(volume, volume);

	//Set the default .mp3 file.
	//ResetRepeatStackCharArray();
	Play(voiceWelcomeToMuseumX); //Welcome to museum X.
	//repeatStackCharArray[0]		 	= voiceWelcomeToMuseumX;
	Play(voicePleaseVisitAndTap); //Please visit and tap into exhibition.
	//repeatStackCharArray[1] 		= voicePleaseVisitAndTap;
	Play(voicePrime); //Prime.
	//repeatStackCharArray[2] 		= voicePrime;

}

/*
The loop() override function is basically deivided between these two while loops.
The first while loop happen when there is an audio file playing and the second
	while loop happen when there is no audio file playing.
*/
void NTGVS1053::NTGVS1053Loop(){

	/*
	When an audio file in playing you cannot tag the RFID card.
	I do this to keep maintaining space inside the controller.
	In this case I am using Arduino UNO which has only 32 kilo bytes.
	There is a way to implement tagging RFID tag while an audio file is playing.
	1. Audio is playing.
	2. New RFID input into software serial (check NTGRFIDReader class).
	3. Copy every input from the software serial into new array of int.
	4. Turn off the audio playing.
	5. Play new audio file based on the copied input from the software serial.
	I have tried this but the codes go beyond 32 kilo bytes.
	Possible solutions are using better controller than Arduino Uno or using good pointer management
		(of which I do not understand yet).
	While the audio is not playing you can tag an RFID card.
	*/

	while(!vs1053.playingMusic){

		PhysicalButton(vs1053.playingMusic);

		/*
		Get RFID index from NTGRFIDLoop() function.
		This NTGRFIDLoop() function returns an integer number.
		*/
		mp3Index = ntgRFIDReader.NTGRFIDReaderLoop(vs1053.playingMusic);

		/*
		So if NTGRFIDLoop() function returns a -1 then the value from the RFID card is not valid.
		Hence we are not executing these codes below anymore.
		Just try to remember this, if NTGRFIDLoop() returns -1 the RFID card is not valid, if
			NTGRFIDLoop() returns 1 then the RFID card/tag is valid and the program goes through.
		*/
		if(mp3Index != -1){

			//Check if the visited exhibition is a proper detinated exhibition.
			if(mp3Index == targetExhibitionArray[0] || mp3Index == targetExhibitionArray[1]){ haveVisitedTargetExhibition = true; }
			else if(mp3Index != targetExhibitionArray[0] && mp3Index != targetExhibitionArray[1]){ haveVisitedTargetExhibition = false; }

			//Play an audio file to welcome player/visitor to any visited exhibition in the museum.
			//ResetRepeatStackCharArray();
			Play(voiceWelcomeToExhibition);
			//repeatStackCharArray[0]	= voiceWelcomeToExhibition;
			if(mp3Index == 0){
				Play(voicePrime);
			}
			else if(mp3Index != 0){
				mp3IndexBufferString = String(mp3Index) + ".mp3";
				mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 10);
				Play(mp3IndexBufferCharArray);
			}
			//char repeat1[6];
			//strncpy(repeat1, mp3IndexBufferCharArray, 10);
			//repeatStackCharArray[1]	= repeat1;

			//If the player properly visited the next exhibition.
			if(haveVisitedTargetExhibition){

				/*
				Give explanation here.
				Generate random seed from floating analog pin.
				*/
				randomSeed(analogRead(RANDOM_SEED));
				//Generate random integer from 1 to 4 (there is only 4 explanations available per exhibition).
				int explanationIndexRandom = random(1, 5);
				Play(voiceExplanationNumber);
				//repeatStackCharArray[2]	= voiceExplanationNumber;
				//Convert int to String to char array.
				mp3IndexBufferString = String(explanationIndexRandom) + ".mp3";
				mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 10);
				Play(mp3IndexBufferCharArray);
				//char repeat2[6];
				//strncpy(repeat2, mp3IndexBufferCharArray, 10);
				//repeatStackCharArray[3]	= repeat2;

				//Roll and assign new target here.
				Play(voicePleaseShakeTheDice);
				//repeatStackCharArray[4]	= voicePleaseShakeTheDice;

				int diceNumber = -1;
				while(1){
					diceNumber = ntgDice.NTGDICELoop();
					if(diceNumber != -1){ break; }
				}

				Play(voiceYouRolled);
				//repeatStackCharArray[5]	= voiceYouRolled;
				int targetExhibitionIndexRandom = diceNumber + 1;
				//int targetExhibitionIndexRandom = 1;
				mp3IndexBufferString = String(targetExhibitionIndexRandom) + ".mp3";
				mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 10);
				Play(mp3IndexBufferCharArray);
				//char repeat3[6];
				//strncpy(repeat3, mp3IndexBufferCharArray, 10);
				//repeatStackCharArray[6]	= repeat3;

				/*
				Assign four target exhibitions here.
				The dice must be rolled to determine which path character/visitor is taking.
				*/
				if(targetExhibitionIndexRandom <= 10){
					targetExhibitionArray[0] = targetExhibitionDatabaseArray[mp3Index][0];
					targetExhibitionArray[1] = targetExhibitionDatabaseArray[mp3Index][1];
				}
				else if(targetExhibitionIndexRandom > 10){
					targetExhibitionArray[0] = targetExhibitionDatabaseArray[mp3Index][2];
					targetExhibitionArray[1] = targetExhibitionDatabaseArray[mp3Index][3];
				}

				Play(voicePleaseVisitAndTap);
				//repeatStackCharArray[7]	= voicePleaseVisitAndTap;

				for(int i = 0; i < 2; i ++){

					if(targetExhibitionArray[i] != 0){

						mp3IndexBufferString = String(targetExhibitionArray[i]) + ".mp3";
						mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 10);
						Play(mp3IndexBufferCharArray);
						//char repeat4[6];
						//strncpy(repeat4, mp3IndexBufferCharArray, 10);
						//repeatStackCharArray[8]	= repeat4;

					}
					//If exhibition prime is the one of target exhibition.
					else{
						Play(voicePrime);
						//repeatStackCharArray[9]		= voiceOr;
					}

					if(i == 0){
						Play(voiceOr);
						//repeatStackCharArray[10]	= voiceOr;
					}

				}

				//Set the boolean variable to false again so that the visitor can visit the next destinated exhibition.
				haveVisitedTargetExhibition 	= false;
				//If this if statement is executed at least the prime exhibition has been visited.
				isPrimeExhibitionVisited 		= true;

			}
			else if(!haveVisitedTargetExhibition){

				//ResetRepeatStackCharArray();
				Play(voicePleaseVisitAndTap);
				//repeatStackCharArray[0]	= voicePleaseVisitAndTap;

				for(int i = 0; i < 2; i ++){

					if(targetExhibitionArray[i] != 0){

						mp3IndexBufferString = String(targetExhibitionArray[i]) + ".mp3";
						mp3IndexBufferString.toCharArray(mp3IndexBufferCharArray, 10);
						Play(mp3IndexBufferCharArray);
						//char repeat5[6];
						//strncpy(repeat5, mp3IndexBufferCharArray, 10);
						//repeatStackCharArray[1]	= repeat5;

					}
					//If exhibition prime is the one of target exhibition.
					else{
						Play(voicePrime);
						//repeatStackCharArray[2]	= voiceOr;
					}

					if(i == 0){
						Play(voiceOr);
						//repeatStackCharArray[3]	= voiceOr;
					}

				}
				
			}

		}

	}

}