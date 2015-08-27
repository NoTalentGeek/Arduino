#include <SoftwareSerial.h>
#include "Arduino.h"
#include "NTGRFIDReader.h"

//Initialize software serial connection.
SoftwareSerial softwareSerial(7, 6);
//The amount of the registered tags.
const int tagsAmountOf 					= 2;
//The amount of relevant bytes carried by the tags.
const int bytesNetto 					= 12;
//The total amount of bytes carried by the tags.
int bytesBrutto 						= bytesNetto + 4;
//Database of registered tags.
int tagsKnown[tagsAmountOf][bytesNetto]	= {
	{54, 65, 48, 48, 52, 65, 48, 66, 54, 57, 52, 50},
	{00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}
};
//Newly tagged tags.
int 	tagUnknown[bytesNetto];
char 	tagUnknownChar[bytesNetto];

/*
Variable to registered whether a newly tagged tag is registered in database
	or not.
*/
bool isTagRegistered;
/*
Count the last matched tag index in the database.
If bytesMatchCounter is equal to bytesNetto then the current i is the
	lastMatchedIndex.
*/
int lastMatchedIndex;

NTGRFIDReader::NTGRFIDReader(){}

void NTGRFIDReader::NTGRFIDReaderSetup(){

	//Set serial connection.
	softwareSerial.begin(9600);

}

void NTGRFIDReader::NTGRFIDReaderLoop(bool _isPlaying){

	//Whether there is a tag to read or not.
	bool tagRead 		= false;
	//Tag counter to read byte per byte.
	int tagByteCounter 	= 0;
	//The read byte within the tag (one byte per loop).
	int byteRead;

	if(_isPlaying){
		while(softwareSerial.available() > 0){
			softwareSerial.read();
		}
	}
	else if(!_isPlaying){
		//If there is a tags within radius of RFID reader set the tagRead to true.
		if(softwareSerial.available() == bytesBrutto){ tagRead = true; }
		if(tagRead){

			//Print every available bytes in the tags.
			Serial.print("Card serial number: ");
			while(softwareSerial.available() > 0){

				byteRead = softwareSerial.read();

				//Do not print the irrelevant bytes.
				if(

					byteRead != 2
					&& byteRead != 3
					&& byteRead != 10
					&& byteRead != 13

				){

					//Fill in every byte to variable array of tagUnknown and tagUnknownChar.
					tagUnknown[tagByteCounter] = byteRead;
					tagUnknownChar[tagByteCounter] = byteRead;
					//Increse the array counter.
					tagByteCounter ++;
					Serial.print(byteRead);
					if(tagByteCounter != bytesNetto){ Serial.print(" "); }

				}
				//If byteRead received 3 as an incoming value then end the card reading.
				if(byteRead == 3){

					tagRead = false;
					Serial.println(".");

				}

			}

		}
	}

	//If the length of tagUnknown char is 0 then there is no tag to read.
	if(strlen(tagUnknownChar) == 0){ return; }
	else{

		//Counter for tag in database.
		int i = 0;
		//Counter for bytes per tag in database.
		int j = 0;
		//Count how many bytes are matched with any tags in database.
		int bytesMatchCounter = 0;
		//While loop to check the current tagged tag with tag in the database.
		while(1){

			/*
			If a byte is matched with a byte in index i in database then
				increase the byte counter j and bytesMatchCounter by 1.
			*/
			if(tagUnknown[j] == tagsKnown[i][j]){

				j ++;
				bytesMatchCounter ++;

			}
			/*
			If a byte is not match with a tag in index i then
				change the index (i ++), reset j to count back from 0,
				and reset bytesMatchCounter.
			*/
			else if(tagUnknown[j] != tagsKnown[i][j]){

				i ++;
				j = 0;
				bytesMatchCounter = 0;

			}

			/*
			If all bytes are the same with bytes in index i
				then the currently tagged tag is registered and
				the current i is the lastMatchedIndex.
			*/
			if(bytesMatchCounter == bytesNetto){

				isTagRegistered = true;
				break;

			}
			/*
			This happened when the loop has gone through all registered index.
			Which means, the newly tagged tag is not registered.
			*/
			else if(i == tagsAmountOf - 1){

				isTagRegistered = false;
				break;

			}

		}

		//If the newly tagged tag is registered.
		if(isTagRegistered){

			/*
			Because i started counting from index 0.
			Whether you want to add additional 1 or not it is up to u.
			*/
			lastMatchedIndex = i;
			Serial.print("This card index is ");
			Serial.print(lastMatchedIndex);
			Serial.println(".");

		}
		//If the newly tagged tag is not registered.
		else if(!isTagRegistered){

			Serial.println("This card index is not registered.");

		}

		//Reset the tagUnknownChar so this if is only happened once.
		for(int i = 0; i < bytesNetto; i ++){ tagUnknownChar[i] = 0; }

	}

}

int GetExhibitionIndex(){
	if(isTagRegistered){ return lastMatchedIndex; }
	else if(!isTagRegistered){ return -1; }
}