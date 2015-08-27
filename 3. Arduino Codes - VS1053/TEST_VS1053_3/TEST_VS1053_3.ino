#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>

//Constant for the break out pin within the Arduino and VS1053.
#define BREAKOUT_RESET 9
#define BREAKOUT_CS 10
#define BREAKOUT_DCS 8
#define SHIELD_RESET -1 //Unused.
#define SHIELD_CS 7
#define SHIELD_DCS 6
#define CARDCS 4
#define DREQ 3 //Interrupt pin.

//Create an object variable for VS1053.
Adafruit_VS1053_FilePlayer vs1053 = Adafruit_VS1053_FilePlayer(

	BREAKOUT_RESET,
	BREAKOUT_CS,
	BREAKOUT_DCS,
	DREQ,
	CARDCS

);

void setup(){

	Serial.begin(9600);
	Serial.println("Adafruit VS1053 Class Initialization");
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

}

void loop(){

	//Self made function to control audio play.
	PlayAudioFile("3.mp3");
	//Do something while the music is playing.
	while(vs1053.playingMusic){}
	Serial.println("VS1053 is done playing music.");

}

void PlayAudioFile(char _index[]){

	if(!vs1053.startPlayingFile(_index)){
		Serial.println("Could not play " + String(_index));
		//Halt the system.
		while(1);
	}
	Serial.println(String(_index) + " is currently being played.");

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