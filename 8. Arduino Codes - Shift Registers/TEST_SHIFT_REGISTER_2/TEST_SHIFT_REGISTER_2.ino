#include <Shifter.h>

#define SER_Pin 5 //SER_IN
#define RCLK_Pin 2 //L_CLOCK
#define SRCLK_Pin 6 //CLOCK

#define NUM_REGISTERS 3 //how many registers are in the chain


//initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS); 

void setup(){

}

void loop(){
  
  for(int i = 0; i < 20; i ++){
    shifter.clear(); //set all pins on the shift register chain to LOW
    shifter.write(); //send changes to the chain and display them
    shifter.setPin(i, HIGH);
    shifter.write(); //send changes to the chain and display them
    delay(100);
  }
  
  
  //notice how you only call write after you make all the changes you want to make
  
  
  
}
