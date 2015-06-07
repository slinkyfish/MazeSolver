#ifndef _myFunctions_h_
#define myFunctions_h_

extern void toneOn(int length, int period);
extern void toneOff(void);
extern void buzz();
extern DigitalOut buzzer;

extern void motor(bool M1_EN, float M1dirA, float M1dirB, bool M2_EN, float M2_dirA, float M2_dirB);
extern void lCounter();
void rCounter();

#endif
