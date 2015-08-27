#ifndef NTGVS1053_h
#define NTGVS1053_h
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#include "Arduino.h"
#include "NTGDICE.h"
#include "NTGRFIDReader.h"

class NTGVS1053{

	public:
		NTGVS1053();
		static bool IsButtonPressed(bool _isButton, int _buttonPin, void (*callBackFunction)());
		static void PhysicalButton(bool _isPlayingMusic);
		static void Play(char _index[]);
		//static void Repeat();
		static void VolUp();
		static void VolDown();
		static int BoundAndInvertValue(int _value, int _minValue, int _maxValue);
		//void ResetRepeatStackCharArray();
		void NTGVS1053Setup();
		void NTGVS1053Loop();
		
};

#endif