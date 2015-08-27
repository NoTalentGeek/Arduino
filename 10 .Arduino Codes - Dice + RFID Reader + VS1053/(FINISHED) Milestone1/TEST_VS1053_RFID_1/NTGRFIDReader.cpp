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

NTGRFIDReader::NTGRFIDReader(){}

void NTGRFIDReader::NTGRFIDReaderSetup(){

	//Set serial connection.
	softwareSerial.begin(9600);

}

void NTGRFIDReader::NTGRFIDReaderLoop(){

	Serial.println(softwareSerial.available());
	int test = softwareSerial.read();

}