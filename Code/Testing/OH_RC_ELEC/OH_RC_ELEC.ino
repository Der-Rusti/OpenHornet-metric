/**************************************************************************************
 *        ____                   _    _                       _
 *       / __ \                 | |  | |                     | |
 *      | |  | |_ __   ___ _ __ | |__| | ___  _ __ _ __   ___| |_
 *      | |  | | '_ \ / _ \ '_ \|  __  |/ _ \| '__| '_ \ / _ \ __|
 *      | |__| | |_) |  __/ | | | |  | | (_) | |  | | | |  __/ |_
 *       \____/| .__/ \___|_| |_|_|  |_|\___/|_|  |_| |_|\___|\__|
 *             | |
 *             |_|
 *
 *
 *   Project OpenHornet
 *
 *   The OpenHornet Project is a F/A-18C OFP 13C Lot 20 1:1 Replica Simulator,
 *   consisting of a physical structure and electrical/software interfaces to a PC
 *   to be driven by Digital Combat Simulator (DCS).
 *
 *   ---------------------------------------------------------------------------------
 *
 *   This Project is released under the Creative Commons
 *   Atribution - Non Comercal - Share Alike License.
 *
 *   CC BY-NC-SA 3.0
 *
 *   You are free to:
 *   - Share — copy and redistribute the material in any medium or format
 *   - Adapt — remix, transform, and build upon the material
 *   The licensor cannot revoke these freedoms as long as you follow the license terms.
 *
 *   Under the following terms:
 *   - Attribution — You must give appropriate credit, provide a link to the license,
 *     and indicate if changes were made. You may do so in any reasonable manner,
 *     but not in any way that suggests the licensor endorses you or your use.
 *   - NonCommercial — You may not use the material for commercial purposes.
 *   - ShareAlike — If you remix, transform, or build upon the material,
 *     you must distribute your contributions under the same license as the original.
 *
 *   No additional restrictions — You may not apply legal terms or technological
 *   measures that legally restrict others from doing anything the license permits.
 *
 *   More Information about the license can be found under:
 *   https://creativecommons.org/licenses/by-nc-sa/3.0/
 *
 *   ---------------------------------------------------------------------------------
 *
 *   The OpenHornet Software is based on DCS-BIOS
 *   You can find more information here: http://dcs-bios.a10c.de
 *
 *   DCS-BIOS is released under the following terms:
 *   https://github.com/dcs-bios/dcs-bios/blob/develop/DCS-BIOS-License.txt
 * 
 *   Use only the following DCS-BIOS Arduino Library
 *   https://github.com/DCSFlightpanels/dcs-bios-arduino-library
 *
 *   ---------------------------------------------------------------------------------
 *
 *   This Project uses Doxygen as a documentation generator.
 *   Please use Doxigen capable comments.
 *
 **************************************************************************************/

/**
 * @file 
 * @author G. Rick, adapted by Higgins on 08.08.2024
 * @date 06/05/2023
 * @brief Sketch for ELEC panel including stepper.
 *  
 *  **Intended Board:**
 * 
 * 
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

#define STEPS 158 //steps per revolution. 

//Define pins as per OH interconnect document
#define COILBatE1 15
#define COILBatE2 6
#define COILBatE3 14
#define COILBatE4 7

#define COILBatU1 16
#define COILBatU2 8
#define COILBatU3 10
#define COILBatU4 9

int BatE = 0;
int BatU = 0;
int BatEVal = 0;
int BatUVal = 0;
int BatEPos = 0;
int BatUPos = 0;

Stepper stepperBatE (STEPS, COILBatE4, COILBatE3, COILBatE2, COILBatE1); //BAT E Gauge
Stepper stepperBatU (STEPS, COILBatU1, COILBatU2, COILBatU3, COILBatU4); //BAT U Gauge

//***
void onVoltEChange(unsigned int newValueBatE) {
    BatE = map (newValueBatE, 0, 65535, 27, 275); //Obtain first two numbers from DCS. ajust the last two numbers to align pointers.
}
DcsBios::IntegerBuffer voltEBuffer(0x753e, 0xffff, 0, onVoltEChange);

//***
void onVoltUChange(unsigned int newValueBatU) {
    BatU = map (newValueBatU, 0, 65535, 35, 277);
}
DcsBios::IntegerBuffer voltUBuffer(0x753c, 0xffff, 0, onVoltUChange);

//DCS Bios ELEC Switch commands
DcsBios::Switch3Pos batterySw("BATTERY_SW", 2, A2);
DcsBios::Switch2Pos lGenSw("L_GEN_SW", A3);
DcsBios::Switch2Pos rGenSw("R_GEN_SW", 3);

void setup() {
  // put your setup code here, to run once:
DcsBios::setup();

stepperBatE.setSpeed(60);
stepperBatU.setSpeed(60);
// Zero out Gauges This should result in both gauges pointing straight up.
stepperBatE.step(-649); //reset Full On position
stepperBatE.step(649);  //reset full off position

stepperBatU.step(-649); //reset Full On position
stepperBatU.step(649);  //reset full off position

BatEPos - 0;
BatE = map(0, 0, 65535, 0, 649);

BatUPos - 0;
BatU = map(0, 0, 65535, 0, 649);


}

void loop() {

DcsBios::loop();

//Move Stepper for BatE
  BatEVal = BatE;
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
BatUVal = BatU;
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
