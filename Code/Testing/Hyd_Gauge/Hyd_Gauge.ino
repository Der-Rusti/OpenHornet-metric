/**************************************************************************************
 *        ____                   _    _                       _
 *       / __ \                 | |  | |                     | |
 *      | |  | |_ __   ___ _ __ | |__| | ___  _ __ _ __   ___| |_
 *      | |  | | '_ \ / _ \ '_ \|  __  |/ _ \| '__| '_ \ / _ \ __|
 *      | |__| | |_) |  __/ | | | |  | | (_) | |  | | | |  __/ |_
 *       \____/| .__/ \___|_| |_|_|  |_|\___/|_|  |_| |_|\___|\__|
 *             | |
 *             |_|
 *   ----------------------------------------------------------------------------------
 *   Copyright 2016-2024 OpenHornet
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *   ----------------------------------------------------------------------------------
 *   Note: All other portions of OpenHornet not within the 'OpenHornet-Software' 
 *   GitHub repository is released under the Creative Commons Attribution -
 *   Non-Commercial - Share Alike License. (CC BY-NC-SA 4.0)
 *   ----------------------------------------------------------------------------------
 *   This Project uses Doxygen as a documentation generator.
 *   Please use Doxygen capable comments.
 **************************************************************************************/

/**
 * @file OHSketchTemplate.ino
 * @author Gordon Rick (Krikeee)
 * @date 05.06.2024
 * @version u.0.0.1 (untested)
 * @copyright Copyright 2016-2024 OpenHornet. Licensed under the Apache License, Version 2.0.
 * @warning This sketch is based on a wiring diagram, and was not yet tested on hardware. (Remove this line once tested on hardware and in system.)
 * @brief Controls the various items on the RC AUX Panel
 *
 * @details This is the OpenHornet Sketch Template. It should be used as a starting point for every new sketch.
 * Please copy the whole OHSketchTemplate folder to start, and ensure the correct line in the Makefile is uncommented.
 * 
 *  * **Reference Designator:** 5A2A1 thru 5A2A7
 *  * **Intended Board:** ABSIS ALE
 *  * **RS485 Bus Address:** NA
 * 
  * ### Wiring diagram:
 * PIN | Function
 * --- | ---
 * A3  | COIL1
 * 2   | COIL2
 * A2  | COIL3
 * 3   | COIL4
 * A1  | COIL5
 * 4   | COIL6
 * A0  | COIL7
 * 15  | COIL8
 * 6   | WF_UNLOCK
 * 14  | WF_FOLD
 * 7   | WF_SPREAD
 * 16  | AVC_SW
 * 8   | HOOK_SW
 * 10  | HOOK_IND_1
 * 9   | HOOK_IND_2
 * 
 * @brief The following #define tells DCS-BIOS that this is a RS-485 slave device.
 * It also sets the address of this slave device. The slave address should be
 * between 1 and 126 and must be unique among all devices on the same bus.
 *
 * @bug Currently does not work with the Pro Micro (32U4), Fails to compile. 

   #define DCSBIOS_RS485_SLAVE 1 ///DCSBios RS485 Bus Address, once bug resolved move line below comment.
*/

/**
 * Check if we're on a Mega328 or Mega2560 and define the correct
 * serial interface
 * 
 */
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
#define DCSBIOS_IRQ_SERIAL ///< This enables interrupt-driven serial communication for DCS-BIOS. (Only used with the ATmega328P or ATmega2560 microcontrollers.)
#else
#define DCSBIOS_DEFAULT_SERIAL ///< This enables the default serial communication for DCS-BIOS. (Used with all other microcontrollers than the ATmega328P or ATmega2560.)  
#endif

#ifdef __AVR__
#include <avr/power.h>
#endif

/**
 * The Arduino pin that is connected to the
 * RE and DE pins on the RS-485 transceiver.
*/
#define TXENABLE_PIN 5 ///< Sets TXENABLE_PIN to Arduino Pin 5
#define UART1_SELECT ///< Selects UART1 on Arduino for serial communication

#include "Stepper.h"
#include "DcsBios.h"


// Define pins for DCS-BIOS per interconnect diagram.
#define COILHYD1 A3      ///<  stepper coil1
#define COILHYD2 2       ///<  stepper coil2
#define COILHYD3 A2      ///<  stepper coil3
#define COILHYD4 3       ///<  stepper coil4
#define COILHYD5 A1      ///<  stepper coil5
#define COILHYD6 4       ///<  stepper coil6
#define COILHYD7 A0      ///<  stepper coil7
#define COILHYD8 15      ///<  stepper coil8
#define WF_UNLOCK 6   ///<  Wing Fold Unlock
#define WF_FOLD 7     ///<  Wing Fold
#define WF_SPREAD 14  ///<  Wing Spread
#define AVC_SW 16     ///<  AVCool Switch
#define HOOK_SW 8     ///<  Hook Switch
#define HOOK_IND_1 10 ///< Hook Indicator Light 1
#define HOOK_IND_2 9  ///< Hook Indicator Light 2
#define STEPS 649     ///< Stepper Steps per revolution

int HYD1 = 0;
int HYD2 = 0;
int HYD1VAL = 0;
int HYD2VAL = 0;
int HYD1POS = 0;
int HYD2POS = 0;

Stepper stepperHyd1 (STEPS, COILHYD1, COILHYD2, COILHYD3, COILHYD4); //Hyd 1 gauge
Stepper stepperHyd2 (STEPS, COILHYD5, COILHYD6, COILHYD7, COILHYD8); //Hyd 2 gauge

//Declare variables for custom non-DCS logic <update comment as needed>
//bool wowLeft = true;           ///< Update variables as needed and update this Doxygen comment, or remove line/section if not needed.
//bool wowRight = true;          ///< Update variables as needed and update this Doxygen comment, or remove line/section if not needed.


// Connect switches to DCS-BIOS 
DcsBios::Switch2Pos wingFoldPull("WING_FOLD_PULL", WF_UNLOCK);
DcsBios::Switch3Pos wingFoldRotate("WING_FOLD_ROTATE", WF_FOLD, WF_SPREAD);
DcsBios::Switch2Pos avCoolSw("AV_COOL_SW", AVC_SW);
DcsBios::LED arrestingHookLt(0x74a0, 0x0400, HOOK_IND_1);
DcsBios::Switch2Pos hookLever("HOOK_LEVER", HOOK_SW, true);

// Hydrolic Pressure Gauge Reads
void onHydIndLeftChange(unsigned int newValueHyd2) {
  HYD2 = map (newValueHyd2, 5, 65535, 0, 649);
}
  DcsBios::IntegerBuffer hydIndLeftBuffer(0x751e, 0xffff, 0, onHydIndLeftChange);

void onHydIndRightChange(unsigned int newValueHyd1) {
  HYD1 = map (newValueHyd1, 5, 65535, 0, 649);
  }
  DcsBios::IntegerBuffer hydIndRightBuffer(0x7520, 0xffff, 0, onHydIndRightChange);



/**
* Arduino Setup Function
*
* Arduino standard Setup Function. Code who should be executed
* only once at the programm start, belongs in this function.
*/
void setup() {

  // Run DCS Bios setup function
  DcsBios::setup();

stepperHyd1.setSpeed(60);
stepperHyd2.setSpeed(60);
// Zero out Gauges This should result in both gauges pointing to home position. Usually 0 on the gauge.
stepperHyd1.step(-649); //reset Full On position
stepperHyd1.step(649);  //reset full off position

stepperHyd2.step(-649); //reset Full On position
stepperHyd2.step(649);  //reset full off position

HYD1POS - 0;
HYD1 = map(0, 0, 65535, 0, 649);

HYD2POS - 0;
HYD2 = map(0, 0, 65535, 0, 649);


}


/**
* Arduino Loop Function
*
* Arduino standard Loop Function. Code who should be executed
* over and over in a loop, belongs in this function.
*/
void loop() {

  //Run DCS Bios loop function
  DcsBios::loop();

//Move Stepper for HYD1
  HYD1VAL = HYD1;
  if (abs(HYD1VAL - HYD1POS) > 2) {      //if diference is greater than 2 steps.
    if ((HYD1VAL - HYD1POS) > 0) {
      stepperHyd1.step(-1);      // move one step to the left.
      HYD1POS++;
    }
    if ((HYD1VAL - HYD1POS) < 0) {
      stepperHyd1.step(1);       // move one step to the right.
      HYD1POS--;
    }
  }

//Move Stepper for HYD2
HYD2VAL = HYD2;
  if (abs(HYD2VAL - HYD2POS) > 2) {      //if diference is greater than 2 steps.
    if ((HYD2VAL - HYD2POS) > 0) {
      stepperHyd2.step(-1);      // move one step to the left.
      HYD2POS++;
    }
    if ((HYD2VAL - HYD2POS) < 0) {
      stepperHyd2.step(1);       // move one step to the right.
      HYD2POS--;
    }
  }

}

/**
* A brief description on a single line, ended by a period or blank line.
* 
* A longer comment, which may stretch over several lines and may include other things like:
* Lists:
* - list points
* + nested list points
* - more list points
* 
* # Headers Level 1
* ## Headers Level 2
* ### Headers Level 3
* 
* > Block quotes
* 
* **Emphasis**
* _Emphasis_
* 
* `code()`
* 
* even Tables are supported:
* First Header  | Second Header
* ------------- | -------------
* Content Cell  | Content Cell 
* Content Cell  | Content Cell 
* 
* Links:
* [More about markdown support](http://www.doxygen.nl/manual/markdown.html)
* 
* @param myParam1 Description of 1st parameter.
* @param myParam2 Description of 2nd parameter.
* @returns Description of returned value.
*/
int sampleFunction(int myParam1, int myParam2) {
  int myReturn;
  return myReturn;
}
