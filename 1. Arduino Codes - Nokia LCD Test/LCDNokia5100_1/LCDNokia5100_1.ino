#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define PIN_SCLK 7
#define PIN_MOSI 6
#define PIN_DC 5
#define PIN_SCE 4
#define PIN_RST 3

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16
/*
Using Software SPI method.
Slower but more flexible in term of pin mapping.
+--------------+--+-----------+
|     LCD      |  |  Arduino  |
+-----------------------------+
|VCC     |Pin 1|to|Pin VCC    |
|GND     |Pin 2|to|Pin GND    |
|SCE     |Pin 3|to|Pin 4      |
|RST     |Pin 4|to|Pin 3      |
|D/C     |Pin 5|to|Pin 5      |
|DIN/MOSI|Pin 6|to|Pin 6      |
|SCLK    |Pin 7|to|Pin 7      |
|LED     |Pin 8|to|Pin VCC    |
+----------------------------*/
Adafruit_PCD8544 display = Adafruit_PCD8544(
    PIN_SCLK,
    PIN_MOSI,
    PIN_DC,
    PIN_SCE,
    PIN_RST
);

//Adafruit's logo.
static const unsigned char PROGMEM logo16_glcd_bmp[] = {
    B00000000, B11000000,
    B00000001, B11000000,
    B00000001, B11000000,
    B00000011, B11100000,
    B11110011, B11100000,
    B11111110, B11111000,
    B01111110, B11111111,
    B00110011, B10011111,
    B00011111, B11111100,
    B00001101, B01110000,
    B00011011, B10100000,
    B00111111, B11100000,
    B00111111, B11110000,
    B01111100, B11110000,
    B01110000, B01110000,
    B00000000, B00110000
};
void setup(){
    Serial.begin(9600);
    //Initialize the LCD.
    display.begin();
    //Change the LCD contrast.
    display.setContrast(50);

    //Display the Adafruit splash screen.
    display.display();
    delay(2000);
    //Clear the screen and memory buffer.
    display.clearDisplay();
   
    // draw a single pixel
    display.drawPixel(32, 32, BLACK);
    display.display();
    delay(2000);
    display.clearDisplay();
}
void loop(){}
