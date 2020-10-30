/*
 * Blink
 * Uses software generated PWM to make an LED pulse
 */
#include <Arduino.h>
#include <Blink.h>

void setup()
{
  // setup serial monotor
  Serial.begin(115200);
  Serial.println("Hello!");
  loopCounter = 0;
  loopTime0 = millis();
  loopTime1 = loopTime0;

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

  // color changing LED
  pwmR = pwmTime;
  pwmG = 0;
  pwmB = 0;
  pinMode(pinR,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(pinB,OUTPUT);
  colorCycleTime = 0;
  colorUpdateTime = 200;
  colorAdv = 10;
}

void loop()
{
    cycleTime = timer1-timer0;  // get time since last loop started
    dutyTime += cycleTime;      // update the duty cycle timer
    pulseCycleTime += cycleTime;  // update the PWM cycle time
    colorCycleTime += cycleTime;  // update color cycle time
    // update the white LED
    pulseWhite();
    sweepColor();
    // increment the timer for the next loop
    timer0 = timer1;
    timer1 = micros();
    // output loop count to serial monitor every second
    loopTime1 = millis();
    loopCounter += 1;
    if(loopTime1-loopTime0 >= 1000)
    {
      // example of how to write debug info to the serial terminal
      char outString[60];
      sprintf(outString, "loop count = %d per second", loopCounter);
      Serial.println(outString);
      loopTime0 = loopTime1;
      loopCounter = 0;
    }
}

void sweepColor()
{
  // vary PWM strength between RGB pins to change color

  // sweep the pwm between RGB
  if(colorCycleTime > colorUpdateTime)
  {
    colorCycleTime = 0;
    int advSlow = colorAdv/3;
    // take from Red and add to Green
    if(pwmR > 0 && pwmB == 0)
    {
      if(pwmR < pwmTime/2)
      {
        pwmR += -advSlow;
        pwmG += advSlow;
      }
      else
      {
        pwmR += -colorAdv;
        pwmG += colorAdv;
      }
    }
    // take from Green and add to Blue
    if(pwmG > 0 && pwmR == 0)
    {
      if(pwmG < pwmTime/3)
      {
        pwmG += -advSlow;
        pwmB += advSlow;
      }
      else
      {
        pwmG += -colorAdv;
        pwmB += colorAdv;
      }
    }
    // take from Blue and add to Red
    if(pwmB > 0 && pwmG == 0)
    {
      if(pwmB < pwmTime/2)
      {
        pwmB += -advSlow;
        pwmR += advSlow;
      }
      else
      {
        pwmB += -colorAdv;
        pwmR += colorAdv;
      }
    }
  }

  // set OFF state
  if(dutyTime > pwmR && ledOnR)
  {
    digitalWrite(pinR,LOW);
    ledOnR = false;
  }
  if(dutyTime > pwmG && ledOnG)
  {
    digitalWrite(pinG,LOW);
    ledOnG = false;
  }
  if(dutyTime > pwmB && ledOnB)
  {
    digitalWrite(pinB,LOW);
    ledOnB = false;
  }
  // set ON state
  if(dutyTime < pwmR && !ledOnR)
  {
    digitalWrite(pinR,HIGH);
    ledOnR = true;
  }
  if(dutyTime < pwmG && !ledOnG)
  {
    digitalWrite(pinG,HIGH);
    ledOnG = true;
  }
  if(dutyTime < pwmB && !ledOnB)
  {
    digitalWrite(pinB,HIGH);
    ledOnB = true;
  }

}

void pulseWhite()
{
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

}