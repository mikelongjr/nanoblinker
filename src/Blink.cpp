/*
 * Blink
 * Uses software generated PWM to make an LED pulse
 */

#include <Arduino.h>

unsigned long timer0,timer1,dutyTime,cycleTime;
unsigned long pwmTime,pwmDuty,pulseUpdateTime;
unsigned long pulseCycleTime,pwmAdv;
bool ledON,countUP;
// #ifndef LED_BUILTIN
//   #define LED_BUILTIN PC13
// #endif

void setup()
{
  // setup serial monotor
  Serial.begin(115200);
  Serial.println("Hello!");
  // initialize LED digital pin as an output.
  pinMode(PIN_A0, OUTPUT);
  timer0 = micros();        // syncronize counters to start
  timer1 = timer0;
  cycleTime = 0;            // time elapsed since last loop
  pwmTime = 20000;          // PWM signal width.. microseconds 
  pwmDuty = 0;              // PWM duty cycle.. min = 0, max = pwmTime
  dutyTime = 0;             // PWM ON time during duty cycle
  pwmAdv = 1;               // increment for pwmDuty pulse (brightness) update
  pulseCycleTime = 0;       // counter for brighness update
  pulseUpdateTime = 100;     // time between brightness updates (microseconds) 
  ledON = false;            // is LED on?
  countUP = true;           // brightness counter direction
}

void loop()
{
    cycleTime = timer1-timer0;  // get time since last loop started
    dutyTime += cycleTime;      // update the duty cycle timer
    pulseCycleTime += cycleTime;  // update the PWM cycle time

    // if the duty timer passes the PWM duty cycle time
    // turn the led OFF
    if(dutyTime > pwmDuty && ledON)  
    {
      digitalWrite(PIN_A0, LOW);
      ledON = false;
    }
  
    // if the duty timer passes the total PWM time then
    // reset the timer and turn the led ON
    if(dutyTime > pwmTime)
    {
      dutyTime = 0;
      digitalWrite(PIN_A0,HIGH);
      // example of how to write debug info to the serial terminal
      // char outString[30];
      // sprintf(outString, "pwmTime = %d", cycleTime);
      // Serial.println(outString);
      ledON = true;
    }

    // led pulsing is done by just incrementing the duty time
    // every pulseUpdateTime microseconds by pwmAdv microseconds
    // reversing the counter when it hits 0 or pwmTime (off to fully bright)
    if(pulseCycleTime > pulseUpdateTime)
    {
      pulseCycleTime = 0;
      // cycle up
      if(countUP)
      {
        pwmDuty += pwmAdv;
      }
      // cycle down
      else
      {
        pwmDuty += -pwmAdv;
      }
      // fully bright.. count down
      if(pwmDuty == pwmTime)
      {
        countUP = false;
      }
      // off.. count up
      else if(pwmDuty <= 0)
      {
        countUP = true;
      }
    }

    // increment the timer for the next loop
    timer0 = timer1;
    timer1 = micros();
}
