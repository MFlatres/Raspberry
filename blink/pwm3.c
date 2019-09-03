#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pigpio.h>


//------------------------------------------------------------------------
// Initialisation of output: two different channels for two different motors

#define PWM_CHANNEL1 0  //PWM0 on RPI
#define pin1 18 

#define PWM_CHANNEL2 1  //PWM1 on RPI
#define pin2 19 


//------------------------------------------------------------------------
// Create a structure PWM which contains output and frequency

struct PWM{
  int pin;
  double frequency;
  int channel;
};


//-------------------------------------------------------------------------
// Call a function that manage software part

void PWMsoftware(struct PWM a);


//------------------------------------------------------------------------

int main(int argc, char* argv[] ) {

  printf("Raspberry Pi pigpiod  test program\n");
  
  // Test to control user has defined at least on frequency as argument 
  if(argc < 2 || argc > 3) {
    printf("Please give one or two desired frequencies as arguments\n");
    exit(1);
  }
  
  
  // Test to control initialisation of <pigpio.h>  library
  if (gpioInitialise() < 0){
      fprintf(stderr, "pigpiod initialisation failed\n");
  }

    
  // Initialisation of frequencies
  double frequency1,frequency2;
  frequency1 = strtod(argv[1], NULL);
  frequency2 = 0;
  if(argc == 3) frequency2 = strtod(argv[2], NULL);
  
  
//-------------------------------------------------------------------------
//Initialisation of the 2 structures and put it in a single array

  struct PWM PWM1 = {pin1, frequency1, PWM_CHANNEL1};
  struct PWM PWM2 = {pin2, frequency2, PWM_CHANNEL2};

  struct PWM PWMwork[2]={PWM1, PWM2};
    

//---------------------------------------------------------------------
// Iteration on frequencies
  for  (int i=0; i<argc-1; i++){
    
//---------------------------------------------------------------------
    gpioSetMode(PWMwork[i].pin, PI_OUTPUT);
    
//---------------------------------------------------------------------      
    printf("Required frequency: %f Hz\n", PWMwork[i].frequency);
    printf("Loop %i, Channel %i, pin %i\n", i,PWMwork[i].channel, PWMwork[i].pin);
    
    
//---------------------------------------------------------------------
// Using of Harware for frequency >= 1 Hz
    
    if (PWMwork[i].frequency >= 1){

      int dutyC =500000; //period/2;


      gpioHardwarePWM(PWMwork[i].pin, PWMwork[i].frequency, dutyC);

      int RealRange= gpioGetPWMrealRange(PWMwork[i].pin);
      printf("the real Range is: %i Hz\n",RealRange);
      
      int RealFrequency= gpioGetPWMfrequency(PWMwork[i].pin);
      printf("the real frequency is: %i Hz\n",RealFrequency);      
      
    }

//------------------------------------------------------------    
//Using of software for frequency < 1 Hz (with the a function)
    
    else {
      PWMsoftware(PWMwork[i]);
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
  


//----------------------------------------------------------------
// Function Software

void PWMsoftware(struct PWM a){
  double period = 500 / a.frequency;
    
  
  while(1) {
    gpioPWM(a.pin, 0);
    gpioDelay((int)period);
    
    gpioPWM(a.pin, 255);
    gpioDelay((int)period);
  }
}
