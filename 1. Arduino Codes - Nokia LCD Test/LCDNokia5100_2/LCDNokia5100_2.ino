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

    //Draw single pixel.
    display.drawPixel(10, 10, BLACK);
    display.display();
    delay(2000);
    display.clearDisplay();

    //Draw things based on premade functions.
    //Draw many lines.
    testDrawLines();
    display.display();
    delay(2000);
    display.clearDisplay();

    //Draw many rectangles.
    testDrawRectangles();
    display.display();
    delay(2000);
    display.clearDisplay();

    //Draw many circles.
    testDrawCircles();
    display.display();
    delay(2000);
    display.clearDisplay();

    testDrawRoundRectangles();
    delay(2000);
    display.clearDisplay();

    testFillRoundRectangles();
    delay(2000);
    display.clearDisplay();

    testDrawTriangles();
    delay(2000);
    display.clearDisplay();

    testFillTriangles();
    delay(2000);
    display.clearDisplay();

    //Draw the first ~12 characters in the font.
    testDrawChar();
    display.display();
    delay(2000);
    display.clearDisplay();

    //Text display test.
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.println("NOTALENTGEEK~");
    display.setTextColor(WHITE, BLACK); // 'inverted' text
    display.println(1234567890);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.print("0x"); display.println(0xDEADBEEF, HEX);
    display.display();
    delay(2000);
    display.clearDisplay();

    //Rotation example.
    display.setTextSize(1);
    //Rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
    display.setRotation(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.println("Rotation");
    display.setTextSize(2);
    display.println("Example!");
    display.display();
    delay(2000);
    //Revert back to no rotation.
    display.setRotation(0);

    //Miniature bitmap display.
    display.clearDisplay();
    display.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, 1);
    display.display();

    //Invert the display.
    display.invertDisplay(true);
    delay(1000); 
    display.invertDisplay(false);
    delay(1000); 

    //Draw a bitmap icon and "animate" movement.
    testDrawAnimateBitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}
void loop(){}
void testDrawAnimateBitmap(const uint8_t *_bitmap, uint8_t _height, uint8_t _width){
    uint8_t bitmaps[NUMFLAKES][3];
    //Generate number number seed.
    randomSeed(666);

    /*
    Initialize all bitmaps.
    Actually these code below is not for initialize bitmaps.
    Instead it gives each possible bitmaps initiali position.
    */
    for(uint8_t i = 0; i < NUMFLAKES; i ++){
        //XPOS and YPOS are constant 0.
        bitmaps[i][XPOS] = random(display.width());
        bitmaps[i][YPOS] = 0;
        bitmaps[i][DELTAY] = random(5) + 1;
    }

    //Infinite loop.
    while(1){
        for(uint8_t i = 0; i < NUMFLAKES; i ++){
            display.drawBitmap(bitmaps[i][XPOS], bitmaps[i][YPOS], logo16_glcd_bmp, _width, _height, BLACK);
        }
        //Display all bitmaps in one screen at all.
        display.display();
        delay(200);
        //Afterwards, erase and move it according the speed of y axis (DELTAY).
        for(uint8_t i = 0; i < NUMFLAKES; i ++){
            display.drawBitmap(bitmaps[i][XPOS], bitmaps[i][YPOS], logo16_glcd_bmp, _width, _height, WHITE);
            bitmaps[i][YPOS] += bitmaps[i][DELTAY];
            //If corresponding bitmap go off screen then reset.
            if(bitmaps[i][YPOS] > display.height()){
                bitmaps[i][XPOS] = random(display.width());
                bitmaps[i][YPOS] = 0;
                bitmaps[i][DELTAY] = random(5) + 1;
            }
        }
    }
}
void testDrawChar(void){
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);

    for(uint8_t i = 0; i < 168; i ++){
        if(i == '\n')continue;
        display.write(i);
    }
    display.display();
}
void testDrawCircles(void){
    for(int16_t i = 0; i < display.height(); i += 2){
        display.drawCircle(display.width()/2, display.height()/2, i, BLACK);
        display.display();
    }
}
void testFillRectangles(void){
    uint8_t color = 1;
    for(int16_t i = 0; i < display.height()/2; i += 3){
        //Alternate colors.
        display.fillRect(i, i, display.width() - (i*2), display.height() - (i*2), (color%2));
        display.display();
        color++;
    }
}
void testDrawTriangles(void){
    for(int16_t i = 0; i < min(display.width(), display.height())/2; i += 5){
        display.drawTriangle(
            (display.width()/2),
            (display.height()/2) - i,
            (display.width()/2) - i,
            (display.height()/2) + i,
            (display.width()/2) + i,
            (display.height()/2) + i,
            BLACK
        );
        display.display();
    }
}
void testFillTriangles(void){
    uint8_t color = BLACK;
    for(int16_t i = 0; i < min(display.width(), display.height())/2; i += 5){
        display.fillTriangle(
            (display.width()/2),
            (display.height()/2) - i,
            (display.width()/2) - i,
            (display.height()/2) + i,
            (display.width()/2) + i,
            (display.height()/2) + i,
            color
        );
        if(color == WHITE){ color = BLACK; }
        else { color = WHITE; }
        display.display();
    }
}
void testDrawRoundRectangles(void){
    for (int16_t i = 0; i < (display.height()/2) - 2; i += 2){
        display.drawRoundRect(
            i,
            i,
            display.width() - (2*i),
            display.height() - (2*i),
            display.height()/4,
            BLACK
        );
        display.display();
    }
}
void testFillRoundRectangles(void){
    uint8_t color = BLACK;
    for (int16_t i = 0; i < (display.height()/2) - 2; i += 2){
        display.fillRoundRect(
            i,
            i,
            display.width() - (2*i),
            display.height() - (2*i),
            display.height()/4,
            color
        );
        if(color == WHITE){ color = BLACK; }
        else { color = WHITE; }
        display.display();
    }
}
void testDrawRectangles(void){
    for (int16_t i = 0; i < (display.height()/2) - 2; i += 2){
        display.drawRect(
            i,
            i,
            display.width() - (2*i),
            display.height() - (2*i),
            BLACK
        );
        display.display();
    }
}
void testDrawLines(){  
    for(int16_t i = 0; i < display.width(); i += 4){
        display.drawLine(0, 0, i, display.height() - 1, BLACK);
        display.display();
    }
    for(int16_t i = 0; i < display.height(); i += 4){
        display.drawLine(0, 0, display.width() - 1, i, BLACK);
        display.display();
    }
    delay(250);

    display.clearDisplay();
    for(int16_t i = 0; i < display.width(); i += 4){
        display.drawLine(0, display.height() - 1, i, 0, BLACK);
        display.display();
    }
    for(int8_t i=display.height()-1; i>=0; i -= 4){
        display.drawLine(0, display.height() - 1, display.width() - 1, i, BLACK);
        display.display();
    }
    delay(250);

    display.clearDisplay();
    for(int16_t i=display.width()-1; i>=0; i -= 4){
        display.drawLine(display.width() - 1, display.height() - 1, i, 0, BLACK);
        display.display();
    }
    for(int16_t i=display.height()-1; i>=0; i -= 4){
        display.drawLine(display.width() - 1, display.height() - 1, 0, i, BLACK);
        display.display();
    }
    delay(250);

    display.clearDisplay();
    for(int16_t i = 0; i < display.height(); i += 4){
        display.drawLine(display.width() - 1, 0, 0, i, BLACK);
        display.display();
    }
    for(int16_t i = 0; i < display.width(); i += 4){
        display.drawLine(display.width() - 1, 0, i, display.height() - 1, BLACK); 
        display.display();
    }
    delay(250);
}
