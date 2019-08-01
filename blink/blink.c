#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>

const int PWM_pin = 1;  // GPIO18

int main (void){
  int intensity;
  
  if(wiringPiSetup() == -1) {
    printf("wiringPiSetup failed!\n");
    exit (1);
  }
  
  pinMode(PWM_pin, PWM_OUTPUT);
  
  while(1){
    for(intensity = 0; intensity<1024; ++intensity){
      pwmWrite(PWM_pin, intensity);
      delay(1);
    }
    
    delay(1);
    
    for(intensity=1023 ; intensity >=0; --intensity){
      pwmWrite(PWM_pin, intensity);
      delay(1);
    }
    
    delay(1);
  }
}
