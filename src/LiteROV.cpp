#include "pico/stdlib.h"
#include "JOYC.h"
#include "SERVO.h"

#define maxPWM 2048
#define MARGIN 25

int main(){
  
  stdio_init_all();
  
  printf("Hello world!\n");
  
  struct outputs test;
  
  //These signals control the Enable (E) pins on the motor drivers
  SERVO mot0(10, 0, maxPWM);
  gpio_pull_down(10);
  SERVO mot1(12, 0, maxPWM);
  SERVO mot2(14, 0, maxPWM);
  
  //These signals control the Direction (M) pins on the motor drivers
  gpio_init(11);
  gpio_set_dir(11, GPIO_OUT);
  gpio_pull_down(11);
  gpio_init(13);
  gpio_set_dir(13, GPIO_OUT);
  
  //Initialising the joystick driver
  JOYC joyc(1, 26, 27);
  
  while(1){
    test = joyc.getCartesian();
    printf("Received: %4d %4d %4d %4d - %2d %2d\n", test.Left0-100, test.Left1-100, test.Right0-100, test.Right1-100, test.LButton, test.RButton);
    
    if(test.Left0 > (100 + MARGIN)){
      mot0.moveServo((test.Left0-100));
      gpio_put(11, 0);
      printf("Active: - test.Left0=%4d\n", test.Left0);
      printf("Sent: %d\n", (test.Left0-100));
    }
    else if(test.Left0 < (100 - MARGIN)){
      mot0.moveServo(-(test.Left0-100));
      gpio_put(11, 1);
      printf("Active: - test.Left0=%4d\n", test.Left0);
      printf("Sent: %d\n", -(test.Left0-100));
    }
    else{
      mot0.moveServo(0);
      gpio_put(11, 0);
      printf("Active: | test.Left0=%4d\n", test.Left0);
      printf("Sent: 0\n");
    }
    
    sleep_ms(10);
  }
  
  return(0);
}
