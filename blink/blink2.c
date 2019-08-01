#include <wiringPi.h>
#include <softPwm.h>


#include <stdio.h>
#include <stdlib.h>

// Function prototypes:
int range_PWM(int a, int b, int c);


const int PWM_pin = 1;

int main (void){

  if (wiringPiSetup()== -1) { /*verify GPIO output is not empty*/
    printf("WiringPiSetup failed!");
    exit(1);
  }

  pinMode(PWM_pin, PWM_OUTPUT); /*set PWM pin as output*/

  int frequency;
  printf("Give the frequency in Hz:\n");
  scanf("%i", &frequency);      /*Now we fixed the frequency used to blink LED*/
  
  int pwmFrequency = 19.2e6; /*Intern frequency of RPI in Hz*/
  int pwmC = 1920;
  int pwmR = range_PWM(pwmFrequency, pwmC, frequency); 
  
  
  //  int Period = 1/frequency;
  //  int microPeriod = 1000*Period; /*Convert Period in milli second*/
  int microPeriod = 1000/frequency; /*Convert Period in milli second*/
  int T = (int) microPeriod/2;
  //int temp;

  printf("f: %i, pwmR: %i, mP: %i\n", frequency, pwmR, microPeriod);
  
  while (1){  /* condition always available*/
    
    
    //for (temp=0 ; temp<=T; ++temp){
      pwmWrite(PWM_pin, 1024);
      delay(T);
      //}
      //for (temp=T; temp>=0; --temp){
      pwmWrite(PWM_pin, 0);
      delay(T);
      //}
    
  }  
}  


/* Function to use the frequency choosen by the user*/

int range_PWM(int a, int b, int c){
  int d = (int) ((double)a / (double)(b*c));
  return d;
}
