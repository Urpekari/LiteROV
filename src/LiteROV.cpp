//v1.0
//2024-06-20

#include "pico/stdlib.h"
#include "JOYC.h"
#include "SERVO.h"

#define maxPWM 4096
#define MARGIN 25

uint8_t motorSpeed(int16_t speed, SERVO motor, uint8_t dirPin);
uint8_t steer(uint8_t xInput, uint8_t yInput, SERVO motL, SERVO motR, uint8_t dirL, uint8_t dirR);

int main(){
  
  stdio_init_all();
  
  struct outputs control;
  
  //These signals control the Enable (E) pins on the motor drivers
  SERVO mot0(10, 0, maxPWM);
  //We CAN'T use pull downs like this for output pins, for whatever reason they just don't work.
  //Add a physical Pull down resitor (At MOST 330 Ohm) to all the PWM channels
  //gpio_pull_down(10);
  SERVO mot1(12, 0, maxPWM);
  //We CAN'T use pull downs like this for output pins, for whatever reason they just don't work.
  //Add a physical Pull down resitor (At MOST 330 Ohm) to all the PWM channels
  //gpio_pull_down(12);
  SERVO mot2(14, 0, maxPWM);
  //We CAN'T use pull downs like this for output pins, for whatever reason they just don't work.
  //Add a physical Pull down resitor (At MOST 330 Ohm) to all the PWM channels
  //gpio_pull_down(14);
  
  //These signals control the Direction (M) pins on the motor drivers
  gpio_init(11);
  gpio_set_dir(11, GPIO_OUT);
  
  gpio_init(13);
  gpio_set_dir(13, GPIO_OUT);
  
  gpio_init(15);
  gpio_set_dir(15, GPIO_OUT);
  
  //Initialising the joystick driver
  JOYC joyc(1, 26, 27);
  
  while(1){
    control = joyc.getCartesian();
    
    //Normalising the values
    int8_t left0Norm = control.Left0-100;
    int8_t left1Norm = control.Left1-100;
    int8_t right0Norm = control.Right0-100;
    int8_t right1Norm = control.Right1-100;
    printf("Received: %4d %4d %4d %4d - %2d %2d\n", left0Norm, left1Norm, right0Norm, right1Norm, control.LButton, control.RButton);
    
    //I hate what I'm about to do.
    //Left0: 100 (Full Left)  -100 (Full Right)
    //Left1: 100 (Full Up)    -100 (Full Down)
    
    int8_t speedL;
    int8_t speedR;
    
    //Steer LEFT, speedR > speedL
    if(left0Norm < -MARGIN){
      speedL = left0Norm;
      speedR = -speedL;
      printf("LEFT!!\n");
    }
    else if(left0Norm > MARGIN){
      speedL = left0Norm;
      speedR = -speedL;
      printf("RIGHT!!\n");
    }
    else{
      printf("FORWARDS!! (Or backwards idk)\n");
      speedL = left1Norm;
      speedR = left1Norm;
    }
    
    motorSpeed(speedL, mot0, 11);
    motorSpeed(speedR, mot1, 13);
    
    //We can run the Z motor directly off of the Y axis on the right Joystick
    motorSpeed(right1Norm, mot2, 15);
  }
  
  return(0);
}


//This takes a normalised speed (S€[-100, 100]) value
uint8_t motorSpeed(int16_t speed, SERVO motor, uint8_t dirPin){
    
    //printf("Received speed: %d\n", speed);
    
    if(speed > MARGIN*2){
      motor.moveServo(speed);
      gpio_put(dirPin, 0);
      return(speed);
    }
    else if(speed < 0-(MARGIN*2)){
      motor.moveServo(-speed);
      gpio_put(dirPin, 1);
      return(-speed);
    }
    else{
      motor.moveServo(0);
      gpio_put(dirPin, 0);
      return(0);
    }
}
