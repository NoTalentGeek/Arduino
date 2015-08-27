/******************************
PENDING:
Integration of dice.
Integration of rfid reader.
*******************************/
//Import libraries.
#include    <Adafruit_VS1053.h>
#include    <SD.h>
#include    <SPI.h>

//Define pin for random seed.
#define     PIN_RANDOM_SEED         A0

//Define pins for push button.
#define     PIN_BUTTON_REPEAT       A4
#define     PIN_BUTTON_VOL_UP       A3
#define     PIN_BUTTON_VOL_DOWN     A2

//Define pins for VS1053.
#define     PIN_BREAKOUT_RESET      9
#define     PIN_BREAKOUT_CS         10
#define     PIN_BREAKOUT_DCS        8
#define     PIN_SHIELD_RESET        -1
#define     PIN_SHIELD_CS           7
#define     PIN_SHIELD_DCS          6
#define     PIN_CARDCS              4
#define     PIN_DREQ                3

//Define variable for VS1053.
Adafruit_VS1053_FilePlayer vs1053 = Adafruit_VS1053_FilePlayer(

    PIN_BREAKOUT_RESET,
    PIN_BREAKOUT_CS,
    PIN_BREAKOUT_DCS,
    PIN_DREQ,
    PIN_CARDCS

);

//Define variables for mp3 file indexing.
        unsigned int mp3Index                   = 1;        //The current index of MP3 file.
const   unsigned int mp3IndexMin                = 1;        //The minimum index of MP3 file.
const   unsigned int mp3IndexMax                = 39;       //The maximum index of MP3 file.

//Define variables for converting integer to character array.
char    mp3IndexBufferCharArray[10];
String  mp3IndexBufferString;

//Define variables for predetermined MP3 index.
char voiceExplanationNumber[]                   = "31.mp3";
char voiceOr[]                                  = "32.mp3";
char voicePleaseShakeTheDice[]                  = "33.mp3";
char voicePleaseVisitAndTap[]                   = "34.mp3";
char voicePrime[]                               = "35.mp3";
char voiceWelcomeToExhibition[]                 = "36.mp3";
char voiceWelcomeToMuseumX[]                    = "37.mp3";
char voiceYouNeedToGo[]                         = "38.mp3";
char voiceYouRolled[]                           = "39.mp3";

//Define variables for controlling VS1053 volume.
        unsigned int volume                     = 20;       //The current volume. The lower the point the louder the volume become.
const   unsigned int volumeMin                  = 40;       //The minimum volume VS1053 can produce.
const   unsigned int volumeMax                  = 0;        //The maximum volume VS1053 can produce.
const   unsigned int volumeRes                  = 1;        //The amount of step of decrease or increase in volume, everytime volume button is pressed.

//Define variables for processing exhibitions.
bool    haveVisitedExhibitionTarget             = false;
bool    haveVisitedExhibitionPrime              = false;
int     exhibitionTargetDatabaseArray[30][4];               //Database for every possible connection within the exhibitions.
int     exhibitionTargetArray[2];                           //Two exhibition indexs that user can choose to go.

//Define variables for push button.
bool havePressedButtonRepeat                    = false;
bool havePressedButtonVolDown                   = false;
bool havePressedButtonVolUp                     = false;

/******************************
PENDING:
Finish HavePressedButton().
*******************************/
//Define function for controlling physical buttons to control VS1053.
bool HavePressedButton(

    bool    _havePressedButton,
    int     _pinButton,
    void    (*_callBackFunction)()

){}
void ControlPhysicalButton(){

    havePressedButtonVolDown        = HavePressedButton(havePressedButtonVolDown,   PIN_BUTTON_VOL_DOWN,    &VolDown);
    havePressedButtonVolUp          = HavePressedButton(havePressedButtonVolUp,     PIN_BUTTON_VOL_UP,      &VolUp);

}
void Play( char _index[] ){

    vs1053.startPlayingFile( _index );

    //While VS1053 is playing music the only thing that user can do is decrease or increase the VS1053 volume.
    while( vs1053.playingMusic ){

        havePressedButtonVolDown    = HavePressedButton(havePressedButtonVolDown,   PIN_BUTTON_VOL_DOWN,    &VolDown);
        havePressedButtonVolUp      = HavePressedButton(havePressedButtonVolUp,     PIN_BUTTON_VOL_UP,      &VolUp);

    }

}
/******************************
PENDING:
Finish repeat button.
*******************************/
void VolDown(){
    
    volume = volume + volumeRes;
    if( volume >= volumeMin ){ volume = volumeMin; }
    vs1053.setVolume(volume, volume);

}
void VolUp(){

    volume = volume - volumeRes;
    if( volume <= volumeMax ){ volume = volumeMax; }
    vs1053.setVolume(volume, volume);

}

void setup(){

    bool    isExhibitionIndexStartOrBoss;
    int     exhibitionIndexLast;
    int     exhibitionIndexStartArray[5]    = {0, 6,    12, 18, 24};
    int     exhibitionIndexBossArray[5]     = {5, 11,   17, 23, 29};

    for( int i = 0; i < 30; i ++ ){

        for( int j = 0; j < 4; j ++ ){

            for( int k = 0; k < 5; k ++ ){

                if( i == exhibitionIndexBossArray[k] && i != 29 ){

                    exhibitionTargetDatabaseArray[i][0] = exhibitionIndexStartArray[k];
                    exhibitionTargetDatabaseArray[i][1] = exhibitionIndexStartArray[k + 1];
                    exhibitionTargetDatabaseArray[i][2] = exhibitionIndexStartArray[k];
                    exhibitionTargetDatabaseArray[i][3] = 999;

                    exhibitionIndexLast                 = exhibitionTargetDatabaseArray[i][3];
                    isExhibitionIndexStartOrBoss        = true;

                }
                else if( i == 29 ){

                    exhibitionTargetDatabaseArray[i][0] = 0;
                    exhibitionTargetDatabaseArray[i][1] = 0;
                    exhibitionTargetDatabaseArray[i][2] = 0;
                    exhibitionTargetDatabaseArray[i][3] = 0;

                    exhibitionIndexLast                           = exhibitionTargetDatabaseArray[i][3];
                    isExhibitionIndexStartOrBoss        = true;

                }
                else if( i == 0 ){

                    exhibitionTargetDatabaseArray[i][j] = j + 1;

                    exhibitionIndexLast                 = exhibitionTargetDatabaseArray[i][j];
                    isExhibitionIndexStartOrBoss        = true;

                }
                else{ isExhibitionIndexStartOrBoss      = false; }

                if(

                    i < exhibitionIndexBossArray[k] &&
                    !isExhibitionIndexStartOrBoss

                ){

                    int index                               = exhibitionIndexLast + 1;

                    if(index == i)                          { index ++; }
                    if(index > exhibitionIndexBossArray[k]) { index = exhibitionIndexStartArray[k]; }
                    
                    exhibitionTargetDatabaseArray[i][j]     = index;
                    exhibitionIndexLast                     = index;

                    break;

                }

            }

        }

    }

    vs1053.begin        ();
    vs1053.setVolume    (volume, volume);
    vs1053.useInterrupt (VS1053_FILEPLAYER_PIN_INT);

    SD.begin(PIN_CARDCS);

    /*
    Serial.begin(9600);
    Serial.println("Adafruit VS1053 Library Test");
    if (!vs1053.begin()){
        Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
        while (1);
    }
    Serial.println(F("VS1053 found"));

    vs1053.sineTest(0x44, 500);

    if (!SD.begin(PIN_CARDCS)) {
        Serial.println(F("SD failed, or not present"));
        while (1);
    }
    Serial.println("SD OK!");
    */

    Play(voiceWelcomeToMuseumX);
    Play(voicePleaseVisitAndTap);
    Play(voicePrime);

}

void loop(){

    while( !vs1053.playingMusic ){

        ControlPhysicalButton();

    }

}