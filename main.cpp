/* Program Updates
 * 31/05/15
 *  - Connected and tested Motors and Bluetooth module
 * 01/06/2015
 *  - Connected and tested Encoders - work OK
 * 06/06/2015
 *  - Added Straight P control
 */
 
#include "mbed.h"
#include "myFunctions.h"

DigitalOut my_led(LED1);		//On-board LED
DigitalIn button(PC_13);		//On-board button
DigitalOut buzzer(D10);		//Notification Buzzer

//Encoder Connections
InterruptIn R_enc1(PC_10);
DigitalIn R_enc2(PC_11);
InterruptIn L_enc1(PC_12);
DigitalIn L_enc2(PD_2);

//Motor connections
DigitalOut RM_En(PC_6);
PwmOut RM_A(PC_8);
PwmOut RM_B(PC_9);
DigitalOut LM_En(PB_15);
PwmOut LM_A(PB_14);
PwmOut LM_B(PB_13);

//Communications Connection
Serial bt(PA_9 , PA_10);	//BT module connected to PA_9 and PA_1

//Encoder Pulse Counts
volatile int lCount = 0;
volatile int rCount = 0;

int Count = 0;
float Error = 0;
float PError = 0;
Ticker PID;
float PIDcounter = 0;
float accError = 0;
float IError = 0;
float oldError = 0;
float DError = 0;
float intError = 0;

float array[500];

Timer myTime;

void straight(float speed){
	if(speed-Error <0) Error = speed;
	if(speed+Error > 1) Error = 1-speed;
	motor(1, speed-Error, 0, 1, speed+Error, 0);
}

int a = 0;
void PID_control(){
	PIDcounter++;		//number of executions of this PID function
	intError = (lCount-rCount);		//save internal error
	
	PError = intError/40;		//proportional / 20 works

	accError += intError;			//Accumulated error
	IError = (accError/PIDcounter)/20;
	
	DError = (intError - oldError)*0.05;
	oldError = intError;
	Error = PError + 0*IError + 0*DError;
	//Limit between +- 0.3)
	if(Error<-0.3) Error = -0.3;
	if(Error>0.3) Error = 0.3;
	//if(abs(Error) ==0.3) my_led = 1; else my_led = 0;
	array[a] = Error;//(lCount-rCount);
	a++;
	if(a>490) a=490;
}




int main() {
	bt.baud(115200);
	LM_A.period_ms(2);	//Chosen to match Arduino
	LM_B.period_ms(2);	//Chosen to match Arduino
	RM_A.period_ms(2);	//Chosen to match Arduino
	RM_B.period_ms(2);	//Chosen to match Arduino
	R_enc1.fall(&rCounter);
	L_enc1.fall(&lCounter);
	PID.attach_us(&PID_control, 10000);
	while(button==1);		//wait for buton press before starting
	wait(0.2);
	while(button==0);		//wait for buton press before starting
	while(1){
		//bt.printf("%d, %d\r\n", lCount, rCount);
		my_led = ! my_led;
		//wait(0.5);
		//leftTurn();
		a=0;
		myTime.start();
		while(myTime < 2){
		straight(0.3);
		}
		while(myTime < 3){
		straight(0.0);
		}
		myTime.stop();
		motor(1, 0, 0, 1, 0, 0);
		wait(1);
		for(int i=0; i<490; i++){
			bt.printf("%f\n", array[i]);
			wait_ms(1);
		}
		wait(10);
		//straight(0.5);
		/*
		for(float a=0; a<1; a+=0.1){
			motor(1, 0, 0, 0, 0.2, 0);
			wait(1);
			motor(1, 0, 0, 1, a, 0);
			wait(1);
			rCount = 0;
			wait(1);
			Count = rCount;
			bt.printf("%g, %d\n\r", a, Count);
		}*/
		
		/*for(int a=0; a<9; a++){
			motor(1, 0.2, 0, 0, 0, 0);
			wait(1);
			motor(1, array[a], 0, 0, 0, 0);
			wait(1);
			lCount = 0;
			wait(1);
			Count = lCount;
			bt.printf("%d, %d\n\r", a, Count);
		}*/
	}
}

//			toneOn(100000, 800);

void motor(bool LM_EN, float LM_dirA, float LM_dirB, bool RM_EN, float RM_dirA, float RM_dirB){
  RM_En = RM_EN;
	RM_A = RM_dirA;
	RM_B = RM_dirB;
	LM_En = LM_EN;
	LM_A = LM_dirA;
	LM_B = LM_dirB;
}

void lCounter(){
    if(L_enc2 == 1) lCount++;
    else lCount--;
}

void rCounter(){
    if(R_enc2 == 1) rCount--;
    else rCount++;
}

/* Pin Usage
 * PA_5  - On board LED
 * PA_9  - Bluetooth Module
 * PA_10 - Bluetooth Module
 * PB_1  - PWM
 * PB_11 - PWM
 * PB_13 - PWM T1 - LM B
 * PB_14 - PWM T1 - LM A
 * PB_15 - PWM T1 - LM Enable
 * PC_6  - PWM T3 - RM Enable
 * PC_8  - PWM T3 - RM A
 * PC_9  - PWM T3 - RM B
*/

//PB_12 may not work - had 7.4V through it!