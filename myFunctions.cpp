#include "mbed.h"
#include "myFunctions.h"

Timeout timer;
Ticker buzzerTone;

void toneOn(int length, int period) {			//
    timer.attach_us(toneOff, length);		//Turn off after 'length'
    buzzerTone.attach_us(buzz, period/2);	//Start tone at desired pitch
}

void toneOff(void) {
    buzzerTone.detach();								//Turns the buzzer off
}

void buzz(){
    buzzer = !buzzer;										//Change buzzer state
}