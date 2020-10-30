#include <Arduino.h>

// global variables
unsigned long timer0,timer1,dutyTime,cycleTime;
unsigned long pwmTime,pwmDuty,pulseUpdateTime;
unsigned long pulseCycleTime,pwmAdv;
bool ledON,countUP;

// function definitions
void pulseWhite();
