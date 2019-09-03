//#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pigpio.h>


//------------------------------------------------------------------------
// Initialisation of output: two different channels for two different motors

#define PWM_CHANNEL1 0
#define PWM_CHANNEL2 1

#define pin1 18 
#define pin2 19 


//------------------------------------------------------------------------

int main(int argc, char* argv[] ) {

  // Test to control initialisation of <pigpio.h>  library
  if (gpioInitialise() < 0)
    {
      fprintf(stderr, "pigpiod initialisation failed\n");
    }

    
  printf("Raspberry Pi pigpiod  test program\n");
  
  // Test to control user has defined at least on frequency as argument 
  if(argc < 2) {
  printf("Please give the desired frequency as an argument\n");
  exit(1);
   }


  // Initialisation of frequencies
  double frequency1,frequency2;
  frequency1 = strtod(argv[1], NULL);
  frequency2 = 0;
  if(argc == 3) frequency2 = strtod(argv[2], NULL);
  
  
  double frequency[2] = {frequency1, frequency2};
  printf("%f %f\n", frequency[0], frequency[1]);
  

  // Iteration on frequencies
  for(int i=0; i<argc-1; i++){
    
    printf("\n");
    
    int pin;
    int channel;
 
//---------------------------------------------------------------------    
    if (i == 0){
      channel= PWM_CHANNEL1;
      pin = pin1;  // PWM0
    }

    if (i == 1){
      channel = PWM_CHANNEL2;
      pin = pin2 ;  // PWM1
    }

    gpioSetMode(pin, PI_OUTPUT);
    
//---------------------------------------------------------------------      
    printf("Required frequency: %f Hz\n", frequency[i]);
    printf("Loop %i, Channel %i, pin %i\n", i, channel, pin);
    
    
//---------------------------------------------------------------------
// Using of Harware for frequency >= 1 Hz
    
    if (frequency[i] >= 1){

      int dutyC =500000; //period/2;


      gpioHardwarePWM(pin, frequency[i], dutyC);

      int RealRange= gpioGetPWMrealRange(pin);
      printf("the real Range is: %i Hz\n",RealRange);
      
      int RealFrequency= gpioGetPWMfrequency(pin);
      printf("the real frequency is: %i Hz\n",RealFrequency);      
      
    }

//------------------------------------------------------------    
//Using of software for frequency < 1 Hz    
    else {

      //  int wiringPiSetup(void);
      
      // pinMode(pin, PWM_OUTPUT);
      double period[2] = {500 / frequency[0], 500/frequency[1]} ;
      
      
      while(1) {
	gpioPWM(pin, 0);
	delay((int)period[i]);
	
	gpioPWM(pin, 255);
	delay((int)period[i]);
      }
    }
    
  }
//-----------------------------------------------------------      
// Hardware part will keep working if we don't stop it handly

  char stop; 
  printf("----------------------------------\n");
  printf("For exit the code press: x\n");
  scanf("%c",&stop);

  if(stop == 'x'){
    gpioHardwarePWM(18,0,0);
    gpioHardwarePWM(19,0,0);
    exit(0);
    
  }
  
}
