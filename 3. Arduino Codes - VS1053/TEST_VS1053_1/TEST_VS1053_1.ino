//Include SPI, MP3, and SD library.
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
//Define the pins used.
//Default pins.
/*
#define CLK 			13		//SPI Clock, shared with SD card.
#define MISO 			12		//Input data, FROM VS1053/SD card.
#define MOSI 			11		//Output data, TO VS1053/SD card.
*/
//These are pins for breakout example.
#define BREAKOUT_RESET 	9 		//VS1053 reset pin (output).
#define BREAKOUT_CS 	10 		//VS1053 chip select pin (output).
#define BREAKOUT_DCS 	8 		//VS1052 data/command select pin (output).
//These are pins for music maker shield.
#define SHIELD_RESET 	-1 		//VS1053 reset pin (unused).
#define SHIELD_CS 		7 		//VS1053 chip select pin (output).
#define SHIELD_DCS 		6 		//VS1053 data/command select pin.
//These are common pins between breakout and shield.
#define CARDCS 			4 		//Card chip select pin.
//DREQ should be attached to an arduino interrupt pin.
#define DREQ 			3 		//VS1053 data request, ideally interrupt pin.

//Create VS1053 example object.
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(

	BREAKOUT_RESET,
	BREAKOUT_CS,
	BREAKOUT_DCS,
	DREQ,
	CARDCS

);

void setup(){

	Serial.begin(9600);

	//Check whether VS1053 is properly connected.
	if(!musicPlayer.begin()){

		Serial.println(

			"Could not finf VS1053, please make sure that VS1053 is properly connected."

		);

		//If VS1053 is not properly connected then stop the program here.
		while(1);

	}
	Serial.println("VS1053 is active!");

	//Initialize the SD card slot.
	SD.begin(CARDCS);

	//Set the volume for left and right channel, the smaller the number the bigger the
	////division, hence louder volume.
	musicPlayer.setVolume(20, 20);

	//Timer interrupts are not suggested.
	//It is better to use DREQ interrupt.
	//musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMERO_INT); //Timer interrupt.

	//If DREQ is on interrupt pin (Arduino Uno's d2 or d3) we can do
	////background audio play.
	musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT); //DREQ interrupt.

	//Example of audio play.
	//This audio file will play until finish before proceed to loop().
	Serial.println("Playing track001.mp3.");
	musicPlayer.playFullFile("track001.mp3");
	//If interrupt is enabled then we can play another file in the background.
	/*
	Serial.println("Playing track002.mp3.");
	musicPlayer.startPlayingFile("track002.mp3");
	Serial.println("Playing track003.mp3.");
	musicPlayer.startPlayingFile("track003.mp3");
	*/

}

void loop(){

	//Serial.println("Playing track003.mp3.");
	//musicPlayer.startPlayingFile("track003.mp3");

	//File is playing in the background.
	if(musicPlayer.stopped()){

		Serial.println("Done playing music.");

		//Halt the system here.
		while(1);

	}

	if(Serial.available() > 0){

		char serialRead = Serial.read();
		Serial.println("TEST");

		//s in Serial Console to stop playing music.
		if(serialRead == 's'){

			Serial.println("Stop.");
			musicPlayer.stopPlaying();

		}

		//p in Serial Console to pause/unpause music.
		if(serialRead == 'p'){

			if(!musicPlayer.paused()){

				Serial.println("Pause");
				musicPlayer.pausePlaying(true);

			}
			else{

				Serial.println("Resumed.");
				musicPlayer.pausePlaying(false);

			}

		}

	}

	delay(100);

}
