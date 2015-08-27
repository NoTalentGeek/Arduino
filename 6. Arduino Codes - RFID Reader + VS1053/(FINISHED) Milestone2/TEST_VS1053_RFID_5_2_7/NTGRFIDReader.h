#ifndef NTGRFIDReader_h
#define NTGRFIDReader_h
#include <SoftwareSerial.h>
#include "Arduino.h"

class NTGRFIDReader{

	public:
		NTGRFIDReader();
		void NTGRFIDReaderSetup();
		int NTGRFIDReaderLoop(bool _isPlaying);

};

#endif