#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
//These are pins used for the breakout example.
#define	BREAKOUT_RESET 9
#define BREAKOUT_CS 10
#define BREAKOUT_DCS 8
#define SHIELD_RESET -1
#define SHIELD_CS 7
#define SHIELD_DCS 6
#define CARDCS 4
#define DREQ 3 //This particular pin must be an interrupt pin.
//Create a breakout example object.
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(

	BREAKOUT_RESET,
	BREAKOUT_CS,
	BREAKOUT_DCS,
	DREQ,
	CARDCS

);

void setup(){

	Serial.begin(9600);
	Serial.println("Adafruit VS1053 Library Example Sketch");
	//Initialize the music player.
	if(!musicPlayer.begin()){

		Serial.println(
			"Could not find VS1053, please make sure the device is properly attached!"
		);
		while(1);

	}
	Serial.println("VS1053 is active!");
	//Gives it a test sound.
	musicPlayer.sineTest(0x44, 500);
	//Check the connection between VS1053 to SD card slot.
	if(!SD.begin(CARDCS)){

		Serial.println("SD card slot is not detected!");
		while(1);

	}
	Serial.println("SD card slot is active!");
	//Print the list of files.
	printDirectory(SD.open("/"), 0);
	//Set the volume.
	//The smaller the louder the volume will become.
	musicPlayer.setVolume(20, 20);
	//There are options to use pin interrupt or timer interrupt.
	//In this case I will use only pin interrupt.
	if(!musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)){

		Serial.println("DREQ pin is not an interrupt pin");

	}

}
void loop(){

	//PENDING: Make this statement below into a function later on.
	if(!musicPlayer.startPlayingFile("track002.mp3")){

		Serial.println("Could not open track002.mp3");
		while(1);

	}
	Serial.println("Started playing...");
	while(musicPlayer.playingMusic){

		Serial.println("Music is still playing.");

	}
	Serial.println("VS1053 is done playing music.");

}
void printDirectory(File dir, int numTabs){

	while(true){

		File entry = dir.openNextFile();
		if(!entry){

			//No more files.
			break;

		}
		for(uint8_t i = 0; i < numTabs; i ++){

			Serial.print('\t');

		}
		Serial.print(entry.name());
		if(entry.isDirectory()){

			Serial.println("/");
			printDirectory(entry, numTabs + 1);

		}
		else{

			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);

		}

		entry.close();

	}

}