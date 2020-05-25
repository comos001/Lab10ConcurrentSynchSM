/*  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif
 

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
   return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)) );
}
unsigned char GetBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}

unsigned char threeLEDS = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char cnt = 0x00;
unsigned char count = 0x00;
unsigned char speaker = 0x00;

enum TL_States {Led1, Led2, Led3} TL_State;
void TickFct_ThreeLeds() {
	 switch ( TL_State ) { //Transitions
      case Led1:
	 if (cnt <= 3){	
	     TL_State = Led1;
	}
	else{
             cnt = 0;
	     TL_State = Led2;
	}
         break;
      case Led2:
	if (cnt <= 3){ 
         TL_State = Led2;
	}
	else{
   	     cnt = 0;
	     TL_State = Led3;
	} 
         break;
      case Led3:
	if (cnt <= 3){
         TL_State = Led3;
        }
        else{
             cnt = 0;
             TL_State = Led1;
        }
         break;

      default:
         TL_State = Led1;
         break;
   }

   switch (TL_State ) { //State actions
      case Led1:
	threeLEDS = 0x00;
	threeLEDS = SetBit(threeLEDS, 0, 1); 
	cnt++;
	break;
      case Led2:
	threeLEDS = 0x00;
	threeLEDS = SetBit(threeLEDS, 1, 1);
	cnt++;
	break;
      case Led3:
	threeLEDS = 0x00;
	threeLEDS = SetBit(threeLEDS, 2, 1); 
      	cnt++;
	break;
      default:
        break;
   }
}

		
enum BL_States {Start1, LedOff, LedOn } BL_State; 
void TickFct_BlinkLED() {
	 switch ( BL_State ) { //Transitions
	case Start1:
	 BL_State = LedOff;
      case LedOff:
	if (count >= 10){
         count = 0;
	 BL_State = LedOn;
        }
        else {
         BL_State = LedOff;
	}
         break;
      case LedOn:
	 if (count >= 10){
	 count = 0;
         BL_State = Start1;
        }
        else {
         BL_State = LedOn;
	}
      default:
         BL_State = Start1;
         break;
   }

   switch (BL_State ) { //State actions
      case Start1:
	break;
	case LedOff:
        count++;
	blinkingLED = SetBit(blinkingLED, 3, 0);
	break;
      case LedOn:
        blinkingLED = SetBit(blinkingLED, 3, 1);
        count++;
	break;
      default:
        break;
   }
	
}

enum C_state {Combine} Cstate;
void CombineOutput() {
	switch(Cstate){
		case Combine:
		PORTB = blinkingLED | threeLEDS;
		break;
		
		default:
		 Cstate = Led1;	
		break;
	}
}


int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    	

    TimerSet(100);
    TimerOn();
    PORTB = 0x00;
    while (1) {
        while (!TimerFlag);
        TimerFlag = 0;
	TickFct_ThreeLeds();
	TickFct_BlinkLED();
	CombineOutput();   	 
	}
    return 1;    
}

