#include <SoftwareSerial.h>
#include "NTGRFIDReader.h"

NTGRFIDReader ntgRFIDReader;

void setup(){

	Serial.begin(9600);
	ntgRFIDReader.NTGRFIDReaderSetup();

}

void loop(){

	ntgRFIDReader.NTGRFIDReaderLoop();

}