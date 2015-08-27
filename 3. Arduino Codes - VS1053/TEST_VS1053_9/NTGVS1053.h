#ifndef NTGVS1053_h
#define NTGVS1053_h
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#include "Arduino.h"

class NTGVS1053{

	public:
		NTGVS1053();
		bool IsButtonPressed(bool _isButton, int _buttonPin, void (*callBackFunction)());
		bool IsButtonPressed(bool _isButton, char _charArray[], int _buttonPin, void (*callBackFunction)(char[]));
		bool IsButtonPressed(bool _isButton, bool _argumentBool, int _buttonPin, void (*callBackFunction)(bool));
		void PhysicalButton(bool _isPlayingMusic);
		void Play(char _index[]);
		void Stop();
		void PauseUnpause(bool _isPaused);
		void NextPrevious(bool _isNextButton);
		void VolUp();
		void VolDown();
		int BoundAndInvertValue(int _value, int _minValue, int _maxValue);
		void DebugPrintFileDirectory(File _dir, int _numTabs);
		void DebugSerialButton(bool _isPlayingMusic);
		void NTGVS1053Setup();
		void NTGVS1053Loop();
};

#endif