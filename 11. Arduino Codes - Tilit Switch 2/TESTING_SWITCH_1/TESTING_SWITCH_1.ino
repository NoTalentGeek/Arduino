#include    "SimpleTimer.h"
#define     PIN_DICE        A3
#define     PIN_RANDOM_SEED A0

SimpleTimer timerDice;

int         numberDice                  = 0;
bool        numberDiceNew               = false;
int         counterDice                 = 0;
int         counterDiceThreshold        = 10;
bool        counterInitialTrigger       = false;
int         counterInitialDuration      = 1000;
int         counterInitialDurationMax   = 1000;
int         statePin                    = 0;
int         statePinPrevious            = 0;

void ShakeDice(){

    int analogPinValue              = analogRead(PIN_DICE);
    
    if(analogPinValue               > 511)          { statePin = 1; }
    else if(analogPinValue          <= 511)         { statePin = 0; }

    if(statePinPrevious             != statePin)    { counterInitialTrigger = true; }

    if(counterInitialTrigger){

        if(statePinPrevious         != statePin)    { counterDice ++; }

        counterInitialDuration      --;

    }

    if(counterDice                  >= counterDiceThreshold){

        numberDice                  = random(1, 21);
        numberDiceNew               = true;
        counterDice                 = 0;
        counterInitialDuration      = counterInitialDurationMax;
        counterInitialTrigger       = false;

    }

    if(counterInitialDuration       <= 0){

        counterDice                 = 0;
        counterInitialDuration      = counterInitialDurationMax;
        counterInitialTrigger       = false;

    }

    statePinPrevious                = statePin;
    if(numberDiceNew){

        Serial.println              (numberDice);
        numberDiceNew               = false;

    }

}

void setup(){

    Serial.begin    (9600);
    pinMode         (A5, INPUT);
    randomSeed      (analogRead(PIN_RANDOM_SEED));

    timerDice.setInterval(10, ShakeDice);

}

void loop(){

    timerDice.run();

}