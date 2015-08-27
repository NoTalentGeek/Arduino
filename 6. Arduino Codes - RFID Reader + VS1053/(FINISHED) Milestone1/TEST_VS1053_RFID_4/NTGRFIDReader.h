#ifndef NTGRFIDReader_h
#define NTGRFIDReader_h
#include <SoftwareSerial.h>
#include "Arduino.h"

class NTGRFIDReader{

	public:
		NTGRFIDReader();
		void NTGRFIDReaderSetup();
		void NTGRFIDReaderLoop();

};

#endif