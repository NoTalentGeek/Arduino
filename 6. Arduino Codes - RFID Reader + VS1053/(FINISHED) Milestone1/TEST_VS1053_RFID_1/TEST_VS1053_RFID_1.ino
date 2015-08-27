#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "NTGRFIDReader.h"
#include "NTGVS1053.h"

NTGRFIDReader ntgRFIDReader1;
NTGVS1053 ntgVS1053;

void setup(){

	Serial.begin(9600);
	ntgRFIDReader1.NTGRFIDReaderSetup();
	//ntgVS1053.NTGVS1053Setup();

}
void loop(){

	ntgRFIDReader1.NTGRFIDReaderLoop();
	//ntgVS1053.NTGVS1053Loop();

}