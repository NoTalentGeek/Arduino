#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#define BREAKOUT_RESET 9
#define BREAKOUT_CS 10
#define BREAKOUT_DCS 8
//Unused pin.
#define SHIELD_RESET -1
#define SHIELD_CS 7
#define SHIELD_DCS 6
#define CARDCS 4
//Interrupt pin.
#define DREQ 3
//Create an object for VS1053.
Adafruit_VS1053_FilePlayer vs1053= Adafruit_VS1053_FilePlayer(

	BREAKOUT_RESET,
	BREAKOUT_CS,
	BREAKOUT_DCS,
	DREQ,
	CARDCS

);
unsigned int mp3Index = 1;
const unsigned int mp3IndexMax = 3;
const unsigned int mp3IndexMin = 1;
unsigned int mp3IndexPrevious = 0;
char mp3IndexCharBuffer[50];
String mp3IndexStringBuffer;
//DEBUG: Trigger variable.
bool debugMP3DonePlayingMusicTrigger = true;
void setup(){

	//Start the Serial console baud rate.
	Serial.begin(9600);
	Serial.println("Adafruit VS1053 setup.");
	//Initiate the music player.
	if(!vs1053.begin()){
		Serial.println(
			"Could not find VS1053, please make sure the device is properly connected"
		);
		//Halt the system.
		while(1);
	}
	//If the if statement is not processed then it is sure that the VS1053 is properly connected.
	Serial.println("VS1053 is active.");
	//Test sound.
	vs1053.sineTest(0x44, 500);
	//Check the connection between the controller to the SD card shield.
	if(!SD.begin(CARDCS)){
		Serial.println(
			"SD card shield is not detected, please make sure the device is properly connected."
		);
		//Halt the system.
		while(1);
	}
	//If the if statement is not processed then it is for sure that the SD card shield is properly connected.
	Serial.println("SD card shield is active.");
	//Check all the files inside the micro SD card.
	PrintFileDirectory(SD.open("/"), 0);
	/*
	Set the volume.
	The smaller value the louder the volume will be.
	*/
	vs1053.setVolume(20, 20);
	/*
	There are options to use pin interrupt or timer interrupt.
	But in this case I will only use the pin interrupt.
	*/
	if(!vs1053.useInterrupt(VS1053_FILEPLAYER_PIN_INT)){
		Serial.println("DREQ pin is not connected to an interrupt pin.");
	}
	//Play the default song.
	mp3IndexStringBuffer = String(mp3Index) + ".mp3";
	mp3IndexStringBuffer.toCharArray(mp3IndexCharBuffer, 50);
	//Self made function to control audio play.
	PlayAudioFile(mp3IndexCharBuffer);


}
void loop(){

	//DEBUG: Imaginary serial button.
	int mode;
	if(mp3Index != mp3IndexPrevious){
		//Feed the value to mp3IndexCharBuffer everytime the mp3IndexValue changed.
		mp3IndexStringBuffer = String(mp3Index) + ".mp3";
		mp3IndexStringBuffer.toCharArray(mp3IndexCharBuffer, 50);
		//Set the value to false so that this if statement does not loop.
		mp3IndexPrevious = mp3Index;
	}
	//Do something when the music is playing.
	while(vs1053.playingMusic){

		//Read the Serial input from the console.
		if(Serial.available() > 0){

			mode = Serial.read();
			//DEBUG: Do not use these imaginary button in the finished product.
			if(mode == 49){
				vs1053.stopPlaying();
				Serial.println("Stop button pressed.");
			}
			else if(mode == 50){
				if(!vs1053.paused()){
					vs1053.pausePlaying(true);
					Serial.println("Pause button pressed");
				}
			}
			else if(mode == 51){

				mp3Index ++;
				mp3Index = FromMaxToMin(mp3Index, mp3IndexMax, mp3IndexMin);
				Serial.println(mp3Index);
				//Feed the value to mp3IndexCharBuffer everytime the mp3IndexValue changed.
				mp3IndexStringBuffer = String(mp3Index) + ".mp3";
				mp3IndexStringBuffer.toCharArray(mp3IndexCharBuffer, 50);
				PlayAudioFile(mp3IndexCharBuffer);

			}
			else if(mode == 52){

				mp3Index --;
				mp3Index = FromMaxToMin(mp3Index, mp3IndexMax, mp3IndexMin);
				//Feed the value to mp3IndexCharBuffer everytime the mp3IndexValue changed.
				mp3IndexStringBuffer = String(mp3Index) + ".mp3";
				mp3IndexStringBuffer.toCharArray(mp3IndexCharBuffer, 50);
				PlayAudioFile(mp3IndexCharBuffer);

			}

		}

	}
	//Do something when the music is not playing.
	while(!vs1053.playingMusic){

		//DEBUG: Do not use these imaginary button in the finished product.
		if(Serial.available() > 0){

			mode = Serial.read();
			if(mode == 49){
				PlayAudioFile(mp3IndexCharBuffer);
				Serial.println("Play button pressed.");
			}
			else if(mode == 50){
				if(vs1053.paused()){
					vs1053.pausePlaying(false);
					Serial.println("Unpause button pressed");
				}
			}
			else if(mode == 51){

				mp3Index ++;
				mp3Index = FromMaxToMin(mp3Index, mp3IndexMax, mp3IndexMin);
				Serial.println(mp3Index);
				//Feed the value to mp3IndexCharBuffer everytime the mp3IndexValue changed.
				mp3IndexStringBuffer = String(mp3Index) + ".mp3";
				mp3IndexStringBuffer.toCharArray(mp3IndexCharBuffer, 50);
				PlayAudioFile(mp3IndexCharBuffer);

			}
			else if(mode == 52){

				mp3Index --;
				mp3Index = FromMaxToMin(mp3Index, mp3IndexMax, mp3IndexMin);
				//Feed the value to mp3IndexCharBuffer everytime the mp3IndexValue changed.
				mp3IndexStringBuffer = String(mp3Index) + ".mp3";
				mp3IndexStringBuffer.toCharArray(mp3IndexCharBuffer, 50);
				PlayAudioFile(mp3IndexCharBuffer);

			}

		}

	}

}

void PlayAudioFile(char _index[]){

	if(!vs1053.startPlayingFile(_index)){
		Serial.println("Could not play " + String(_index));
		//Halt the system.
		while(1);
	}
	Serial.println(String(_index) + " is currently being played.");

}

int FromMaxToMin(int _variable, int _maxValue, int _minValue){

	if(_variable > _maxValue){ _variable = _minValue; }
	else if(_variable < _minValue){ _variable = _maxValue; }
	return _variable;

}


//A function from the example to show what and how many files in the SD card.
void PrintFileDirectory(File _dir, int _numTabs){

	while(true){
		File entry = _dir.openNextFile();
		if(!entry){
			//No more files.
			break;
		}
		for(uint8_t i = 0; i < _numTabs; i ++){
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if(entry.isDirectory()){
			Serial.println("/");
			PrintFileDirectory(entry, _numTabs + 1);
		}
		else{
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}

}