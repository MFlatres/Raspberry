// Taken from https://raspberrypi.stackexchange.com/a/8907/73777 on 2019-08-01

#include <wiringPi.h>
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// int PWM_CHANNEL;

#define PWM_CHANNEL1 0
#define PWM_CHANNEL2 1

#define pin1 RPI_GPIO_P1_18
#define pin2 RPI_GPIO_P1_13


int main(int argc, char* argv[] ) {
  
  if(!bcm2835_init()) {
    printf("Error initialising bcm2835, aborting...\n");
    exit(1);
  }
  bcm2835_gpio_fsel(pin1, BCM2835_GPIO_FSEL_ALT5);
  bcm2835_gpio_fsel(pin2, BCM2835_GPIO_FSEL_ALT5);
  
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
  printf("%f %f\n", frequency[0], frequency[1]);
  
  if(wiringPiSetupGpio() == -1) {
    printf("wiringPiSetupGpio failed, exiting\n");
    exit(1);
  }

  for(int i=0; i<argc-1; i++){
    
    printf("\n");
    
    int pin;
    int channel;
    
    
    if (i == 0){
      channel= PWM_CHANNEL1;
      pin = pin1;  // PWM0
      //bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_ALT5);
    }
    if (i == 1){
      channel = PWM_CHANNEL2;
      pin = pin2 ;  // PWM1
      //bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_ALT5);
    }
    
      
    printf("Required frequency: %f Hz\n", frequency[i]);
    printf("Loop %i, pin %i\n", i,pin);
    
    
    // WiringPi:
    //int maxDivisor = 4095;
    //int maxPeriod = 4095;
    
    // BCM2835:
    int maxDivisor = 1024;
    int maxPeriod = 1024;
    
    if (frequency[i] >= 1.5){
      //pinMode(pin, PWM_OUTPUT);
      //pwmSetMode(PWM_MODE_MS);  // Call pwmSetMode() *after* pinMode()!
      
      int divisor = maxDivisor / sqrt(frequency[i]);        // 2 - 4095  ~ divisor, but doesn't have to be a power of 2 - scales linearly? with period
      if(divisor > maxDivisor) divisor = maxDivisor;
      divisor = maxDivisor;
      divisor = BCM2835_PWM_CLOCK_DIVIDER_16;
      //divisor = BCM2835_PWM_CLOCK_DIVIDER_1024;  // WARNING: OVERRULING DIVISOR AND PERIOD!!!
      
      int period  = (int) (19.2e6 / (double)(divisor*frequency[i]));
      if(period > maxPeriod) period = maxPeriod;
      period = 1024;  // WARNING: OVERRULING DIVISOR AND PERIOD!!!
      
      int dutyC   = period/2;    // up time in clock cycles - for 50%, this should be period/2
      
      
      printf("Divisor: %i,  period: %i,  duty cycle: %i\n", divisor, period, dutyC);
      
      frequency[i] = 19.2e6 / (double)(divisor*period);
      printf("Actual frequency: %f Hz\n", frequency[i]);
      
      bcm2835_pwm_set_clock(divisor);
      bcm2835_pwm_set_mode(channel, 1, 1);
      bcm2835_pwm_set_range(channel, period);
      bcm2835_pwm_set_data(channel, dutyC);
      
      bcm2835_pwm_set_mode(channel, 0, 1);
      
      //printf("For exit the code press: x\n");
      //scanf("%c",&stop);    
      //
      //if(stop == 'x'){
      //	pwmWrite(pin,0);
      //exit(0);
      //}

      //delay(5000);
      //bcm2835_close();
      //delay(999999999);
    }  
    
    else {
      pinMode(pin, PWM_OUTPUT);
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
  
  delay(5000);
  
}
