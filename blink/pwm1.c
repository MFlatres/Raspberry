// Taken from https://raspberrypi.stackexchange.com/a/8907/73777 on 2019-08-01

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
  printf("Raspberry Pi wiringPi test program\n");
  
  if(wiringPiSetupGpio() == -1)
    exit(1);
  
  double frequency = 2.5;  // in Hz
  printf("Required frequency: %f Hz\n", frequency);
  
  pinMode(18, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  
  int divisor = 4095;        // 2 - 4095  ~ divisor, but doesn't have to be a power of 2 - scales linearly? with period
  int period = (int) (19.2e6 / (double)(divisor*frequency));
  int dutyC   = period/2;    // up time in clock cycles - for 50%, this should be period/2
  
  printf("Divisor: %i,  period: %i,  duty cycle: %i\n", divisor, period, dutyC);

  frequency = 19.2e6 / (double)(divisor*period);
  printf("Actual frequency: %f Hz\n", frequency);
  
  pwmSetClock(divisor);
  pwmSetRange(period);
  pwmWrite(18, dutyC);
}
