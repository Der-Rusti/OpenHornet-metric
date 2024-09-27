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
 * @file OHRightAUX.ino
 * @author -=Rusti=-
 * @date 09.13.2024
 * @version 0.0.1
 * @copyright Copyright 2016-2024 OpenHornet. Licensed under the Apache License, Version 2.0.
 * @brief ALE controlling HOOK LEVER, WING FOLD, AV COOL and HYD PRESSURE in right AUX PANEL
 *
 * @details 
 *  * **Reference Designator:** 5A2A1 - HOOK LEVER AND INDICATOR, 5A2A2 - WING FOLD HANDLE, 5A2A3 - AV COOL SWITCH, 5A2A5 - HYD PRESS GAUGE
 *  * **Intended Board:** ABSIS ALE
 *  * **RS485 Bus Address:** 2
 * 
 * ### Wiring diagram:
 * PIN | Function
 * --- | ---
 * A1  | HYD PRESS GAUGE NEEDLE 1 COIL 1
 * 4   | HYD PRESS GAUGE NEEDLE 1 COIL 2
 * A0  | HYD PRESS GAUGE NEEDLE 1 COIL 3
 * 15  | HYD PRESS GAUGE NEEDLE 1 COIL 4
 * A3  | HYD PRESS GAUGE NEEDLE 2 COIL 1
 * 2   | HYD PRESS GAUGE NEEDLE 2 COIL 2
 * A2  | HYD PRESS GAUGE NEEDLE 2 COIL 3
 * 3   | HYD PRESS GAUGE NEEDLE 2 COIL 4
 * 6   | WING FOLD UNLOCK
 * 14  | WING FOLD FOLD
 * 7   | WING FOLD SPREAD
 * 16  | AV COOL SWITCH
 * 8   | HOOK SWITCH
 * 10  | HOOK LED 1
 * 9   | HOOK LED 2

 * @brief The following #define tells DCS-BIOS that this is a RS-485 slave device.
 * It also sets the address of this slave device. The slave address should be
 * between 1 and 126 and must be unique among all devices on the same bus.
 *
 * @bug Currently does not work with the Pro Micro (32U4), Fails to compile. 

   #define DCSBIOS_RS485_SLAVE 2 ///DCSBios RS485 Bus Address, once bug resolved move line below comment.
*/

/**
 * Check if we're on a Mega328 or Mega2560 and define the correct
 * serial interface
 * 
 */
#if defined(__AVR_ATmega328P__) ||  defined(__AVR_ATmega2560__)
  #define DCSBIOS_IRQ_SERIAL
#else
  #define DCSBIOS_DEFAULT_SERIAL
#endif

/**
 * The Arduino pin that is connected to the
 * RE and DE pins on the RS-485 transceiver.
*/
#define TXENABLE_PIN 5  ///< Sets TXENABLE_PIN to Arduino Pin 5
#define UART1_SELECT    ///< Selects UART1 on Arduino for serial communication

#ifdef __AVR__
 #include <avr/power.h> 
#endif

/**
 * DCS Bios library include
 */
#include "DcsBios.h"

/**
 * Stepper Library include
 */
#include <Stepper.h>

#define STEPS 649 //steps per revolution. 

//Define pins as per OH interconnect document
#define COILHyd1_1  A1
#define COILHyd1_2  4
#define COILHyd1_3  A0
#define COILHyd1_4  15

#define COILHyd2_1  A3
#define COILHyd2_2  2
#define COILHyd2_3  A2
#define COILHyd2_4  3

#define WF_UNLOCK   6
#define WF_FOLD     14
#define WF_SPREAD   7
#define AVC_SW      16
#define HOOK_SW     8
#define HOOK_LED1   10
#define HOOK_LED2   9


//Stepper setup

int Hyd1Val = 0;
int Hyd2Val = 0;
int Hyd1Pos = 0;
int Hyd2Pos = 0;


Stepper stepperHyd1 (STEPS, COILHyd1_1, COILHyd1_2, COILHyd1_3, COILHyd1_4); //Hyd 1 Gauge
Stepper stepperHyd2 (STEPS, COILHyd2_1, COILHyd2_2, COILHyd2_3, COILHyd2_4); //Hyd 2 Gauge


void onHydIndLeftChange(unsigned int newValueHyd1) {
		Hyd1Val = map (newValueHyd1, 0, 65535, 10, 600); //Obtain first two numbers from DCS. Adjust the last two numbers to align pointers.
}
DcsBios::IntegerBuffer hydIndLeftBuffer(0x751e, 0xffff, 0, onHydIndLeftChange);


void onHydIndRightChange(unsigned int newValueHyd2) {
	Hyd2Val = map (newValueHyd2, 0, 65535, 20, 600);
}
DcsBios::IntegerBuffer hydIndRightBuffer(0x7520, 0xffff, 0, onHydIndRightChange);

//LED and switches
bool HookLightState = LOW;  ///< Initializing for cold/ground start

void onArrestingHookLtChange(unsigned int newValueHookLight) {
		if (newValueHookLight ==1) {
      digitalWrite(HOOK_LED1, HIGH);
      digitalWrite(HOOK_LED2, HIGH);
    } else {
      digitalWrite(HOOK_LED1, LOW);
      digitalWrite(HOOK_LED2, LOW);
    }
}

DcsBios::IntegerBuffer arrestingHookLtBuffer(0x74a0, 0x0400, 10, onArrestingHookLtChange);

//switches
DcsBios::Switch2Pos hookLever("HOOK_LEVER", HOOK_SW, true);
DcsBios::Switch2Pos wingFoldPull("WING_FOLD_PULL", WF_UNLOCK, true);
DcsBios::Switch2Pos avCoolSw("AV_COOL_SW", AVC_SW);

const byte wingFoldRotatePins[3] = {WF_SPREAD, DcsBios::PIN_NC, WF_FOLD};
DcsBios::SwitchMultiPos wingFoldRotate("WING_FOLD_ROTATE", wingFoldRotatePins, 3);

void setup() {
  // put your setup code here, to run once:
DcsBios::setup();

stepperHyd1.setSpeed(60);
stepperHyd2.setSpeed(60);
// Zero out Gauges This should result in both gauges pointing straight up.
stepperHyd1.step(-649); //reset full on position
delay(500);
stepperHyd1.step(649);  //reset full off position
delay(500);
stepperHyd2.step(-649); //reset full on position
delay(500); 
stepperHyd2.step(649);  //reset full off position

}

void loop() {

DcsBios::loop();

//Move Stepper for Hyd1
  if (abs(Hyd1Val - Hyd1Pos) > 2) {      //if difference is greater than 2 steps.
    if ((Hyd1Val - Hyd1Pos) > 0) {
      stepperHyd1.step(-1);      // move one step to the left.
      Hyd1Pos++;
    }
    if ((Hyd1Val - Hyd1Pos) < 0) {
      stepperHyd1.step(1);       // move one step to the right.
      Hyd1Pos--;
    }
  }

//Move Stepper for Hyd2
  if (abs(Hyd2Val - Hyd2Pos) > 2) {      //if difference is greater than 2 steps.
    if ((Hyd2Val - Hyd2Pos) > 0) {
      stepperHyd2.step(-1);      // move one step to the left.
      Hyd2Pos++;
    }
    if ((Hyd2Val - Hyd2Pos) < 0) {
      stepperHyd2.step(1);       // move one step to the right.
      Hyd2Pos--;
    }
  }


}
