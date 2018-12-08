#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED


#include "git-version.h"
#include "configuration.h"
#include "pwm_led.h"
#include "button.h"
#include "learn.h"
#include "mcp.h"
#include "measure.h"
#include "fixed.h"
#include "eeprom.h"
//init ser and output debug messages? (1 - only results, 2 - output also sensor detailed info, >2 other events without measured data
#if HAVE_SERIAL
 #define HAVE_SERIAL_DEBUG 0
 #define HOST_UPDATE_MIN_INT 100 //update host on change of pedal position and MCP reg value but not more frequent than that interval
 #define HAVE_BT 1
 #if HAVE_SERIAL_DEBUG //sDebug var controls this. Control with the 'S D n' command from host.
  #define SER_DEBUG_HOST 1
  #define SER_DEBUG_PLOTTER 5
  #define SER_DEBUG_LED 6
  #define SER_DEBUG_EVENTS 7
  #define SER_DEBUG_BTN 8
  #define SER_DEBUG_DETAIL_MSR 10
 #endif
 #include "hc.h"
#endif


#define STARTUP_DELAY 100 //Wait that much (milliseconds) after all initialisation operations before going into the main loop
//#define MAIN_LOOP_DELAY 1 //Wait that much (milliseconds) between each measurement in main loop (in normal operation mode). 


//Main loop, operation codes
#define OP_NORMAL 1
#define OP_LEARN 2
#define OP_FIXED 3
#define OP_TUNE_FIXED 4
extern byte opMode;
extern byte sDebug;


union Int {
  unsigned int I;
  byte      b[2];
};

#define getdelta(x,y) (x<y?y-x: x-y)

void LearnStart();
void TuneFixedEnd();
void Fixed_OnOff();
void TuneFixed();


#endif //DEFINE_H_INCLUDED
