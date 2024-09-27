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
 * 
 */
const uint8_t LIP_BL_CH1        =    13;
const uint8_t LIP_BL_CH1_COUNT  =    250;
const uint8_t LIP_BL_CH2        =    12;
const uint8_t LIP_BL_CH2_COUNT  =    250;
const uint8_t UIP_BL_CH1        =    11;
const uint8_t UIP_BL_CH1_COUNT  =    250;
const uint8_t UIP_BL_CH2        =    10;
const uint8_t UIP_BL_CH2_COUNT  =    200;
const uint8_t LC_BL_CH1         =     9;
const uint8_t LC_BL_CH1_COUNT   =     250;
const uint8_t LC_BL_CH2         =     8;
const uint8_t LC_BL_CH2_COUNT   =     250;
const uint8_t RC_BL_CH1         =     7;
const uint8_t RC_BL_CH1_COUNT   =     250;
const uint8_t RC_BL_CH2         =     6;
const uint16_t RC_BL_CH2_COUNT  =     380;
const uint8_t BL_CH9            =     5;
const uint8_t BL_CH10           =     4;



//setup of the BL channels
Adafruit_NeoPixel LC_1 = Adafruit_NeoPixel(LC_BL_CH1_COUNT, LC_BL_CH1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LC_2 = Adafruit_NeoPixel(LC_BL_CH2_COUNT, LC_BL_CH2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel RC_1 = Adafruit_NeoPixel(RC_BL_CH1_COUNT, RC_BL_CH1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel RC_2 = Adafruit_NeoPixel(RC_BL_CH2_COUNT, RC_BL_CH2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LIP_1 = Adafruit_NeoPixel(LIP_BL_CH1_COUNT, LIP_BL_CH1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LIP_2 = Adafruit_NeoPixel(LIP_BL_CH2_COUNT, LIP_BL_CH2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel UIP_1 = Adafruit_NeoPixel(UIP_BL_CH1_COUNT, UIP_BL_CH1, NEO_GRB + NEO_KHZ800);


//Change colour values here
uint32_t nvis_green_A = LC_1.Color(51, 102, 0);
uint32_t nvis_green_B = LC_1.Color(85, 138, 0);
uint32_t nvis_yellow = LC_1.Color(172, 144, 0);
uint32_t nvis_red = LC_1.Color(158, 4, 4);
uint32_t nvis_white = LC_1.Color(176, 176, 176);
uint32_t dark = LC_1.Color(0,0,0);


/**
* Arduino Setup Function
*
* Arduino standard Setup Function. Code who should be executed
* only once at the programm start, belongs in this function.
*/
void setup() {
/*
  Run DCS Bios setup function
  DcsBios::setup();
  Serial.begin(9600);
  pinMode(ENC_CH_Z, INPUT_PULLUP);

  currentButtonState = digitalRead(ENC_CH_Z);
*/

// INITIALIZE NeoPixel strip object (REQUIRED)
LIP_1.begin();
LIP_2.begin();
LC_1.begin();
LC_2.begin();
RC_1.begin();
RC_2.begin();
UIP_1.begin();


// Show the neopixel 
LIP_1.show(); 
LIP_2.show(); 
LC_1.show(); 
LC_2.show(); 
RC_1.show(); 
RC_2.show(); 
UIP_1.show();

//Show all LEDs with a green colour
//UIP
    for (int i = 0; i < 250; i++)  {                
    UIP_1.setPixelColor(i, green);      
    }
//LIP
    for (int i = 0; i < 100; i++)  {               
    LIP_1.setPixelColor(i, green);
    }    
    
    for (int i = 0; i < 111; i++)  {               
    LIP_2.setPixelColor(i, green);
    }

    for (int i = 111; i < 117; i++)  {             //Standby Instruments backlight (GRB LEDs)
    LIP_2.setPixelColor(i, GRBgreen);
    }

//Left Console
    for (int i = 104; i < 250; i++)  {
    LC_1.setPixelColor(i, green);
    }

    for (int i = 0; i < 250; i++)  {
    LC_2.setPixelColor(i, green);
    }


//Right Aux
    for (int i = 0; i < 24; i++)  {                                       
    RC_1.setPixelColor(i, green);        
    }
  //RADAR ALTIMETER backlight
    for (int i = 24; i < 26; i++)  {         //@warning: GRB LEDs                             
    RC_1.setPixelColor(i, GRBgreen);        
    }
  //HYD PRESS + AV Cool gauge backlight
    for (int i = 26; i < 105; i++)  {                                      
    RC_1.setPixelColor(i, green);        
    }

//Right Console
    for (int i = 105; i < 200; i++)  {                                       
    RC_1.setPixelColor(i, green);        
    }

<<<<<<< Updated upstream
    for (int i = 0; i < 380; i++)  {
    RC_2.setPixelColor(i, green);
    }
=======
for (int i = 29; i < 33; i++)  {    //FIRE LEFT button
UIP_1.setPixelColor(i, nvred);
}

for (int i = 33; i < 37; i++)  {    //MASTER CAUTION button
UIP_1.setPixelColor(i, yellow);
}

for (int i = 37; i < 39; i++)  {    //GO indicator
UIP_1.setPixelColor(i, green);
}

for (int i = 39; i < 41; i++)  {    //NO-GO indicator
UIP_1.setPixelColor(i, yellow);
}

for (int i = 41; i < 43; i++)  {    //R_BLEED indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 43; i < 45; i++)  {    //L_BLEED indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 45; i < 47; i++)  {    //SPD_BRK indicator
UIP_1.setPixelColor(i, green);
}

for (int i = 47; i < 49; i++)  {    //STBY indicator
UIP_1.setPixelColor(i, yellow);
}

for (int i = 49; i < 51; i++)  {    //REC indicator
UIP_1.setPixelColor(i, yellow);
}

for (int i = 51; i < 53; i++)  {    //LBAR red upper indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 53; i < 55; i++)  {    //LBAR green lower indicator
UIP_1.setPixelColor(i, green);
}

for (int i = 55; i < 57; i++)  {    //XMIT indicator
UIP_1.setPixelColor(i, green);
}

for (int i = 57; i < 59; i++)  {    //ASPJ indicator
UIP_1.setPixelColor(i, yellow);
}

//**********************************************************************************************************************************
//HUD PANEL v2 LEDs (56)

for (int i = 59; i < 115; i++)  {    //MASTER ARM panel backlight
UIP_1.setPixelColor(i, ColorWithBrightness(0, 255, 0, 0.5));
}

//**********************************************************************************************************************************
//RIGHT EWI LEDs

for (int i = 115; i < 119; i++)  {    //FIRE RIGHT indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 119; i < 123; i++)  {    //APU FIRE indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 123; i < 125; i++)  {    //DISP indicator
UIP_1.setPixelColor(i, green);
}

for (int i = 125; i < 127; i++)  {    //RCDR indicator
UIP_1.setPixelColor(i, green);
}

for (int i = 127; i < 137; i++)  {    //undesignated indicators
UIP_1.setPixelColor(i, green);
}

for (int i = 137; i < 139; i++)  {    //SAM indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 139; i < 141; i++)  {    //AAA indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 141; i < 143; i++)  {    //AI indicator
UIP_1.setPixelColor(i, nvred);
}

for (int i = 143; i < 145; i++)  {    //CW indicator
UIP_1.setPixelColor(i, nvred);
}

//**********************************************************************************************************************************
//SPIN PANEL v2.1 LEDs (63)  SPIN indicator at 174 and 181

for (int i = 145; i < 174; i++)  {    //SPIN PANEL backlight 1/3
UIP_1.setPixelColor(i, ColorWithBrightness(0, 255, 0, 0.5));
}

for (int i = 175; i < 181; i++)  {    //SPIN PANEL backlight 2/3
UIP_1.setPixelColor(i, ColorWithBrightness(0, 255, 0, 0.5));
}

for (int i = 182; i < 208; i++)  {    //SPIN PANEL backlight 3/3
UIP_1.setPixelColor(i, ColorWithBrightness(0, 255, 0, 0.5));
}

//SPIN indicator
UIP_1.setPixelColor(174, yellow);
UIP_1.setPixelColor(181, yellow);

*/
/*
for (int i = 0; i < 74; i++)  {                                               //ECM JETT PANEL backlight
LIP_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 74; i < 78; i++)  {                                               //ECM button
LIP_2.setPixelColor(i, LIP_2.Color(100, 100, 0));
}

for (int i = 78; i < 85; i++)  {                                               //RWR BL 1/2
LIP_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

LIP_2.setPixelColor(85, LIP_2.Color(0, 100, 0));                              //RWR BIT backlight
LIP_2.setPixelColor(88, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(86, LIP_2.Color(150, 0, 0));                              //RWR BIT FAIL toggle
LIP_2.setPixelColor(87, LIP_2.Color(150, 0, 0));

LIP_2.setPixelColor(89, LIP_2.Color(0, 100, 0));                              //RWR OFFSET backlight
LIP_2.setPixelColor(92, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(90, LIP_2.Color(0, 100, 0));                              //RWR OFFSET ENABLE toggle
LIP_2.setPixelColor(91, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(93, LIP_2.Color(0, 100, 0));                              //RWR SPECIAL backlight
LIP_2.setPixelColor(96, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(94, LIP_2.Color(0, 100, 0));                              //RWR SPECIAL ENABLE toggle
LIP_2.setPixelColor(95, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(97, LIP_2.Color(0, 100, 0));                              //RWR DISPLAY backlight
LIP_2.setPixelColor(100, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(98, LIP_2.Color(0, 100, 0));                              //RWR DISPLAY LIMIT ENABLE toggle
LIP_2.setPixelColor(99, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(101, LIP_2.Color(0, 100, 0));                              //RWR ALR-67 POWER backlight
LIP_2.setPixelColor(104, LIP_2.Color(0, 100, 0));

LIP_2.setPixelColor(102, LIP_2.Color(0, 100, 0));                              //RWR ALR-67 POWER ON toggle
LIP_2.setPixelColor(103, LIP_2.Color(0, 100, 0));

for (int i = 105; i < 111; i++)  {                                             //RWR BL 2/2
LIP_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 111; i < 117; i++)  {                                             //Standby Instruments backlight
LIP_2.setPixelColor(i, LIP_2.Color(100, 0, 0));
}
>>>>>>> Stashed changes



for (int i = 0; i < 23; i++)  {                                             //Ldg gear
LC_1.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 23; i < 104; i++)  {                                             //Select Jett
LC_1.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 104; i < 114; i++)  {                                             //Fire test
LC_1.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 114; i < 149; i++)  {                                             //Sim Pwr
LC_1.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 149; i < 158; i++)  {                                             //Gen Tie
LC_1.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 158; i < 202; i++)  {                                             //Ext Lt
LC_1.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 202; i < 234; i++)  {                                             //Fuel
LC_1.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 0; i < 20; i++)  {                                             //APU/CRANK
LC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 20; i < 43; i++)  {                                             //FCS
LC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 43; i < 152; i++)  {                                             //COMM
LC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 152; i < 165; i++)  {                                            //ANT SEL (here v2: 13; rev4: 17)
LC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 165; i < 181; i++)  {                                            //OBOGS
LC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 181; i < 205; i++)  {                                            //MC/HYD ISOL
LC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 0; i < 63; i++)  {                                            //ECS
RC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 63; i < 85; i++)  {                                            //DEFOG
RC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 85; i < 150; i++)  {                                            //INTR LT
RC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 150; i < 208; i++)  {                                            //SNSR
RC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 208; i < 269; i++)  {                                            //SIM CTRL
RC_2.setPixelColor(i, LIP_2.Color(0, 100, 0));
}

for (int i = 269; i < 348; i++)  {                                            //SIM CTRL
RC_2.setPixelColor(i, LIP_2.Color(0, 200, 0));
}
*/

LIP_1.show(); 
LIP_2.show(); 
LC_1.show(); 
LC_2.show(); 
RC_1.show();
RC_2.show(); 
UIP_1.show();
}

/**
* Arduino Loop Function
*
* Arduino standard Loop Function. Code who should be executed
* over and over in a loop, belongs in this function.
*/
void loop() {

}