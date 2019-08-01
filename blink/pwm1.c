// Taken from https://raspberrypi.stackexchange.com/a/8907/73777 on 2019-08-01

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
  printf("Raspberry Pi wiringPi test program\n");
  
  if(wiringPiSetupGpio() == -1)
    exit(1);
  
  pinMode(18, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  
  int divisor = 4095;        // 2 - 4095  ~ divisor, but doesn't have to be a power of 2 - scales linearly? with period
  int period  = 4095;        // period in clock cycles; < 4096			       
  int dutyC   = period/2;    // up time in clock cycles - for 50%, this should be period/2
  
  pwmSetClock(divisor);
  pwmSetRange(period);
  pwmWrite(18, dutyC);
}
