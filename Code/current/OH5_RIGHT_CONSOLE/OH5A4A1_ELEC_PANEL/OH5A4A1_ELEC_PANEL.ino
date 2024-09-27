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
 * @file 5A4A1-ELEC_PANEL.ino
 * @author -=Rusti=-
 * @date 09.23.2024
 * @version 0.0.1
 * @copyright Copyright 2016-2024 OpenHornet. Licensed under the Apache License, Version 2.0.
 * @brief ALE controlling ELEC PANEL
 *
 * @details 
 *  * **Reference Designator:** 5A4A1 - ELEC PANEL
 *  * **Intended Board:** ABSIS ALE
 *  * **RS485 Bus Address:** 3
 * 
 * ### Wiring diagram:
 * PIN | Function
 * --- | ---
 * A3  | L GEN NORM
 * 2   | BATT ON
 * A2  | BATT ORIDE
 * 3   | R GEN NORM
 * 15  | HYD PRESS GAUGE NEEDLE 1 COIL 1
 * 6   | HYD PRESS GAUGE NEEDLE 1 COIL 2
 * 14  | HYD PRESS GAUGE NEEDLE 1 COIL 3
 * 7   | HYD PRESS GAUGE NEEDLE 1 COIL 4
 * 16  | HYD PRESS GAUGE NEEDLE 2 COIL 1
 * 8   | HYD PRESS GAUGE NEEDLE 2 COIL 2
 * 10  | HYD PRESS GAUGE NEEDLE 2 COIL 3
 * 9   | HYD PRESS GAUGE NEEDLE 2 COIL 4

 * @brief The following #define tells DCS-BIOS that this is a RS-485 slave device.
 * It also sets the address of this slave device. The slave address should be
 * between 1 and 126 and must be unique among all devices on the same bus.
 *
 * @bug Currently does not work with the Pro Micro (32U4), Fails to compile. 

   #define DCSBIOS_RS485_SLAVE 3 ///DCSBios RS485 Bus Address, once bug resolved move line below comment.
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
#define LGEN_NORM A3
#define BATT_ON 2
#define BATT_ORIDE A2
#define RGEN_NORM 3

#define COILBatE1 15
#define COILBatE2 6
#define COILBatE3 14
#define COILBatE4 7

#define COILBatU1 16
#define COILBatU2 8
#define COILBatU3 10
#define COILBatU4 9

int BatEVal = 0;
int BatUVal = 0;
int BatEPos = 0;
int BatUPos = 0;

Stepper stepperBatE (STEPS, COILBatE4, COILBatE3, COILBatE2, COILBatE1); //BAT E Gauge
Stepper stepperBatU (STEPS, COILBatU1, COILBatU2, COILBatU3, COILBatU4); //BAT U Gauge

//***
void onVoltEChange(unsigned int newValueBatE) {
    BatEVal = map (newValueBatE, 0, 65535, 140, 380); //Obtain first two numbers from DCS. Adjust the last two numbers to align pointers.
}
DcsBios::IntegerBuffer voltEBuffer(0x753e, 0xffff, 0, onVoltEChange);

//***
void onVoltUChange(unsigned int newValueBatU) {
    BatUVal = map (newValueBatU, 0, 65535, 150, 390);
}
DcsBios::IntegerBuffer voltUBuffer(0x753c, 0xffff, 0, onVoltUChange);

//DCS Bios ELEC Switch commands
DcsBios::Switch2Pos lGenSw("L_GEN_SW", LGEN_NORM);
DcsBios::Switch3Pos batterySw("BATTERY_SW", BATT_ON, BATT_ORIDE);
DcsBios::Switch2Pos rGenSw("R_GEN_SW", RGEN_NORM);

void setup() {
  // put your setup code here, to run once:
DcsBios::setup();

stepperBatE.setSpeed(60);
stepperBatU.setSpeed(60);
// Zero out Gauges This should result in both gauges pointing straight up.
stepperBatE.step(-649); //reset full on position
delay(500);
stepperBatE.step(649);  //reset full off position
delay(500);
stepperBatU.step(-649); //reset full on position
delay(500);
stepperBatU.step(649);  //reset full off position
}

void loop() {

DcsBios::loop();

//Move Stepper for BatE
  if (abs(BatEVal - BatEPos) > 2) {      //if diference is greater than 2 steps.
    if ((BatEVal - BatEPos) > 0) {
      stepperBatE.step(-1);      // move one step to the left.
      BatEPos++;
    }
    if ((BatEVal - BatEPos) < 0) {
      stepperBatE.step(1);       // move one step to the right.
      BatEPos--;
    }
  }

//Move Stepper for BatU
  if (abs(BatUVal - BatUPos) > 2) {      //if diference is greater than 2 steps.
    if ((BatUVal - BatUPos) > 0) {
      stepperBatU.step(-1);      // move one step to the left.
      BatUPos++;
    }
    if ((BatUVal - BatUPos) < 0) {
      stepperBatU.step(1);       // move one step to the right.
      BatUPos--;
    }
  }

}
