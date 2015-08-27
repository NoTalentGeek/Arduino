#ifndef NTGDICE_h
#define NTGDICE_h
#include <Shifter.h>
#include "Arduino.h"
class NTGDICE{
	public:
		NTGDICE();
		bool ShakeDice();
		void NTGDICESetup();
		int NTGDICELoop();
};
#endif