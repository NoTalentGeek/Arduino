#include <Adafruit_VS1053.h>
#include <SD.h>
#include <Shifter.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "NTGDICE.h"
#include "NTGVS1053.h"

NTGVS1053 ntgVS1053;

void setup(){

	ntgVS1053.NTGVS1053Setup();

}
void loop(){

	ntgVS1053.NTGVS1053Loop();

}