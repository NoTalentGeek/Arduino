#ifndef NTGRFIDReader_h
#define NTGRFIDReader_h
#include <SoftwareSerial.h>
#include "Arduino.h"

class NTGVS1053{

	public:
		NTGVS1053();
		bool IsButtonPressed(bool _isButton, int _buttonPin, void (*callBackFunction)());
		bool IsButtonPressed(bool _isButton, char _charArray[], int _buttonPin, void (*callBackFunction)(char[]));
		bool IsButtonPressed(bool _isButton, bool _argumentBool, int _buttonPin, void (*callBackFunction)(bool));
		void PhysicalButton(bool _isPlayingMusic);
		static void Play(char _index[]);
		static void Stop();
		static void PauseUnpause(bool _isPaused);
		static void NextPrevious(bool _isNextButton);
		static void VolUp();
		static void VolDown();
		static int BoundAndInvertValue(int _value, int _minValue, int _maxValue);
		void DebugSerialButton(bool _isPlayingMusic);
		void NTGVS1053Setup();
		void NTGVS1053Loop();
		
};
class NTGRFIDReader{

	public:
		NTGRFIDReader();
		void NTGRFIDReaderSetup();
		void NTGRFIDReaderLoop(bool _isPlaying, NTGVS1053& _ntgVS1053);

};

#endif