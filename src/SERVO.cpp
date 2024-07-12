//v1.0
//2024-06-07

/*
  LITEROV NOTE:
  To whoever reads this: I did not want to figure out PWM on the Raspberry Pi Pico again.
  So instead of doing that, I used the Servo Control library I made for my own personal projects.
  I plan on getting rid of this some day... but in the meantime: I modified it to have a larger duty cycle dynamic range.
  If you want to implement the same thing as this in arduino, don't: analogWrite() is what you want. It does exactly what we want.
  The raspberry pi pico has no easy PWM setup that I can find. Maybe the defaults are what I want? Either way, this works!
  
  LITEROV NOTA:
  A quien pueda interesar: No quería volver a adivinar como funciona el PWM en la Raspberry Pi Pico otra vez.
  En lugar de hacer eso, usé la librería de control de servos que hice para proyectos personales.
  Pretendo librarme de esto algun día, pero hasta entonces: He modificado esta para que use mas duty cycle.
  Si quieres implementar algo como esto pero en arduino, no lo hagas: analogWrite() es lo que buscas. Hace exactamente lo que esto intenta hacer.
  No sé si la raspberry Pi Pico tiene un setup default util o no, pero esto funciona.
  
  LITEROV OHARRA:
  Irakurtzen duenari: Ez nuen Raspberry Pi Pico-ren PWM-ak zelan funtzionatzen zuten berriz asmatu nahi.
  Horren ordez, nire proiektu pertsonaletarako erabili nuen servo kontrol liburutegia erabili dut.
  Hau kentzea dut planeatuta noizbati... baina bitartean: Duty Cycle-a hobeto erabiltzeko modifikatu dut.
  Honelako zeozer inplementatu nahian arduinon, ez egin: analogWrite() da nahi duzuna. Honek egin nahi duena egiten du zuzenean.
  Ez dakit Raspberry Pi Pico-ak default erabilgarririk duen ala ez, baina beharbada, hau funtzionatzen du.
  
*/

#include <SERVO.h>
#include "pico/stdlib.h"
#include "stdio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/time.h"
#include "hardware/irq.h"

SERVO::SERVO(uint8_t pin)
{
    this -> pin = pin;
    this -> min = 200;
    this -> max = 1200;

    config();
}

SERVO::SERVO(uint8_t pin, uint16_t min, uint16_t max)
{    
    this -> pin = pin;
    this -> min = min;
    this -> max = max;

    config();
}

void SERVO::config(){
    gpio_init(this -> pin);
    gpio_set_function(pin,  GPIO_FUNC_PWM);

    this-> slice = pwm_gpio_to_slice_num(this -> pin);
    this-> channel = pwm_gpio_to_channel(this -> pin);

    pwm_set_clkdiv(slice, 256.0f);
    pwm_set_wrap(slice, 9804);
    pwm_set_enabled(slice, true);
}

//Literally the map function in arduino
uint16_t SERVO::convertAngle(uint8_t receivedPos, uint16_t min, uint16_t max){

    //receivedPos   : The input value we want to convert
    //in_min        : Lower limit of all possible inputs
    //in_max        : Upper limit of all possible inputs
    //this->min       : Lower limit of the desired outputs
    //this->max       : Upper limit of the desired outputs

    
      //Case-specific modification: DO NOT MERGE INTO REGULAR LIBRARY!
      uint8_t in_min = 0;
      uint8_t in_max = 95;

    return( (receivedPos - in_min) * (max - min) / (in_max - in_min) + min);
}

SERVO::~SERVO(){
}

void SERVO::moveServo(uint8_t pos){
    uint16_t correctedVal = convertAngle(pos, (this->min), (this->max));
    //printf("Servo pos: %d \n", correctedVal);
    pwm_set_chan_level(this->slice, this->channel, correctedVal);
}
