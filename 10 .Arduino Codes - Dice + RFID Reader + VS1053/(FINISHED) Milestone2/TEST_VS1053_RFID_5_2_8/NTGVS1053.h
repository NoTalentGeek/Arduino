#ifndef NTGVS1053_h
#define NTGVS1053_h
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#include "Arduino.h"
#include "NTGRFIDReader.h"

class NTGVS1053{

	public:
		NTGVS1053();
		static bool IsButtonPressed(bool _isButton, int _buttonPin, void (*callBackFunction)());
		static bool IsButtonPressed(bool _isButton, char _charArray[], int _buttonPin, void (*callBackFunction)(char[]));
		static bool IsButtonPressed(bool _isButton, bool _argumentBool, int _buttonPin, void (*callBackFunction)(bool));
		static void PhysicalButton(bool _isPlayingMusic);
		static void Play(char _index[]);
		static void Stop();
		static void PauseUnpause(bool _isPaused);
		static void Repeat();
		static void NextPrevious(bool _isNextButton);
		static void VolUp();
		static void VolDown();
		static int BoundAndInvertValue(int _value, int _minValue, int _maxValue);
		void DebugPrintFileDirectory(File _dir, int _numTabs);
		void DebugSerialButton(bool _isPlayingMusic);
		void NTGVS1053Setup();
		void NTGVS1053Loop();
		
};

#endif