// Taken from https://raspberrypi.stackexchange.com/a/8907/73777 on 2019-08-01

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char* argv[] ) {



  //  char stop;
  printf("Raspberry Pi wiringPi test program\n");

  
  if(argc < 2) {
    printf("Please give the desired frequency as an argument\n");
    exit(1);
  }
  double frequency1,frequency2;
  frequency1 = strtod(argv[1], NULL);
  frequency2 = 0;
  if(argc == 3) frequency2 = strtod(argv[2], NULL);
  
  
  double frequency[2] = {frequency1, frequency2};
  //printf("%f %f\n", frequency[0], frequency[1]);
  
  if(wiringPiSetupGpio() == -1) {
    printf("wiringPiSetupGpio failed, exiting\n");
    exit(1);
  }
  

  for(int i=0; i<argc-1; i++){
    printf("\n");
    
    int pin;
    if (i == 0) pin=18;  // PWM0
    if (i == 1) pin=12;  // PWM1
    
    
    
    printf("Required frequency: %f Hz\n", frequency[i]);
    printf("Loop %i, pin %i\n", i,pin);
    pinMode(pin, PWM_OUTPUT);
    
    
    
    
    if (frequency[i] >= 1.5){
      pwmSetMode(PWM_MODE_MS);  // Call pwmSetMode() *after* pinMode()!
      int divisor = 4095 / sqrt(frequency[i]);        // 2 - 4095  ~ divisor, but doesn't have to be a power of 2 - scales linearly? with period
      if(divisor > 4095) divisor = 4095;
      int period  = (int) (19.2e6 / (double)(divisor*frequency[i]));
      if(period > 4095) period = 4095;
      int dutyC   = period/2;    // up time in clock cycles - for 50%, this should be period/2
      
      printf("Divisor: %i,  period: %i,  duty cycle: %i\n", divisor, period, dutyC);
      
      frequency[i] = 19.2e6 / (double)(divisor*period);
      printf("Actual frequency: %f Hz\n", frequency[i]);
      
      pwmSetClock(divisor);
      pwmSetRange(period);
      pwmWrite(pin, dutyC);
      
      //printf("For exit the code press: x\n");
      //scanf("%c",&stop);    
      //
      //if(stop == 'x'){
      //	pwmWrite(pin,0);
      //exit(0);
      //}
    }  
    
    else {
      double period[2] = {500 / frequency[0], 500/frequency[1]} ;
      
      //printf("For exit the code press: x\n");
      
      
      while(1) {
	pwmWrite(pin, 0);
	printf("Down\n");
	delay((int)period[i]);
	
	pwmWrite(pin, 4095);
	printf("Up\n");
	delay((int)period[i]);
      }
      
    }
  } 
}

