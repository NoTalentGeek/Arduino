#ifndef NTGDICE_h
#define NTGDICE_h
#include <Shifter.h>
#include "Arduino.h"
class NTGDICE{
	public:
		NTGDICE();
		int ShakeDice();
		void Blink(int _pinIndex);
		void RandomBlink();
		void NTGDICESetup();
		int NTGDICELoop();
};
#endif