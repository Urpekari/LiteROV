//v1.1
//2024-06-18

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#ifndef __JOYC_H_
#define __JOYC_H_

  //Joysticks: Cartesian coordinates (X = Horizontal, Y = Vertical)
  #define LXAddr 0x60
  #define LYAddr 0x61
  #define RXAddr 0x62
  #define RYAddr 0x63
  
  //Joysticks: Polar coordinates (P = Angle, R = Radius)
  #define LPAddr 0x70
  #define LRAddr 0x71
  #define RPAddr 0x72
  #define RRAddr 0x73

  //Auxiliary buttons: 
  #define PBAddr 0x64

  //We're using the same struct to get both types (cartesian and polar) of coordinates through the getter
  struct outputs{
    uint8_t Left0;  //X or Angle
    uint8_t Left1;  //Y or Radius
    uint8_t Right0;
    uint8_t Right1;
    bool LButton;
    bool RButton;
  };

class JOYC{

    private:
        //VARS
        uint8_t addresses[10] = {LXAddr, LYAddr, RXAddr, RYAddr, LPAddr, LRAddr, RPAddr, RRAddr, PBAddr};
        
        
        struct outputs cartesian;
        struct outputs polar;
        //FUNCS
        void update();
        void xyzGradient();
        uint8_t colorRamp(uint8_t potValIn);
        
        

    public:
        //VARS
        void drawRGB(uint8_t red, uint8_t green, uint8_t blue);
        
        //FUNCS
        struct outputs getCartesian();

          //TODO: DOCUMENT
          //FOR THE TIME BEING: TOP GEAR TOP TIP: **AVOID**
          //LITERALLY NO CLUE WHAT THE DATA MEANS. The angle starts at Full left joystick, the radius might be a float.
          struct outputs getPolar();
          
        JOYC(uint8_t i2c, uint8_t SCL, uint8_t SDA);
        ~JOYC();

};

#endif
