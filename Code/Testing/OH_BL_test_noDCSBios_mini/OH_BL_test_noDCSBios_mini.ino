/**
 * Check if we're on a Mega328 or Mega2560 and define the correct
 * serial interface and if on a Sparkfun Pro Micro use Default Serial
 * 
 */
#if defined(__AVR_ATmega328P__) ||  defined(__AVR_ATmega2560__)
  #define DCSBIOS_IRQ_SERIAL
#else
  #define DCSBIOS_DEFAULT_SERIAL
#endif

#ifdef __AVR__
 #include <avr/power.h> 
#endif

#include <Adafruit_NeoPixel.h>

/**
 * @brief BL channels are correct, the written number of pixels exceeds the actual number, since this is a test only

Actual numbers:

RC_BL_CH1 = LDG CHKLST (24 LEDS) + RALT (2) + HYD PRESS (42) + CAUTION (24) + AV COOL (13) + ELEC () + BATT GAUGE ()
 
 * 
 */

const uint8_t RC_BL_CH1         =     7;
const uint8_t RC_BL_CH1_COUNT   =     250;
const uint8_t RC_BL_CH2         =     6;
const uint8_t RC_BL_CH2_COUNT   =     250;

//setup of the BL channels
Adafruit_NeoPixel RC_1 = Adafruit_NeoPixel(RC_BL_CH1_COUNT, RC_BL_CH1, NEO_GRB + NEO_KHZ800);

//Change colour values here
uint32_t nvis_green_A = RC_1.Color(51, 102, 0);
uint32_t nvis_green_B = RC_1.Color(85, 138, 0);
uint32_t nvis_yellow = RC_1.Color(172, 144, 0);
uint32_t nvis_red = RC_1.Color(158, 4, 4);
uint32_t nvis_white = RC_1.Color(176, 176, 176);
uint32_t dark = RC_1.Color(0,0,0);

/**
* Arduino Setup Function
*/

void setup() {

// INITIALIZE NeoPixel strip object (REQUIRED)
RC_1.begin();

// fill all LEDs with off/dark value 
RC_1.fill();

// Show the neopixel 
RC_1.show(); 

//Show all LEDs with a green colour - standard brightness

RC_1.fill(nvis_green_A);

RC_1.show(); 

}

/**
* Arduino Loop Function
*/

void loop() {

delay(500);
// set brightness to 0%
RC_1.gamma8(0);
RC_1.fill();
RC_1.show();

// set brightness to 25%
RC_1.gamma8(63);
RC_1.fill();
RC_1.show();

// set brightness to 50%
RC_1.gamma8(127);
RC_1.fill();
RC_1.show();

// set brightness to 75%
RC_1.gamma8(190);
RC_1.fill();
RC_1.show();

// set brightness to 100%
RC_1.gamma8(255);
RC_1.fill();
RC_1.show();

}