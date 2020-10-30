#include <Arduino.h>

// global variables

// timers
unsigned long timer0,timer1,loopTime0,loopTime1;
int loopCounter;
// white pulse code
unsigned long dutyTime,cycleTime;
unsigned long pwmTime,pwmDuty,pulseUpdateTime;
unsigned long pulseCycleTime,pwmAdv;
bool ledON,countUP;

// RGB color changing code
// cycle R->G->B->R moving PWM window through each
unsigned long pwmR,pwmG,pwmB,colorAdv;
unsigned long colorCycleTime,colorUpdateTime;
bool ledOnR,ledOnG,ledOnB;
int pinR = PIN_A3;
int pinG = PIN_A4;
int pinB = PIN_A5;

// function definitions
void pulseWhite();
void sweepColor();