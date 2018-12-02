#ifndef LEARN_H_INCLUDED
#define LEARN_H_INCLUDED

//learn time values
#define LEARN_M_DELAY 10 //Add that much delay (milliseconds) between measurements in Learn mode
#define LEARN_DURATION 8000 //Total duration of Learn procedure, after which we go back to normal operation mode

//Position related settings
#define P_MIN_DEFAULT 700 //if never ran a Learn procedure, treat that sensor reading as fully closed position
#define P_MAX_DEFAULT 900 //if never ran a Learn procedure, treat that sensor reading as fully open position




extern float rCoef;
extern float pwmCoef;

extern byte mM_has_changed,MCP_Out_has_changed;  // flags altered by the Set command to indicate it can be written to EEPROM

void calc_rCoef();

void Learn();
void LearnStart();
void RMALoadEEPROM();
void RMASaveEEPROM();
void MinMaxLoadEEPROM();
void MinMaxSaveEEPROM();
void calc_rCoef();

#endif //LEARN_H_INCLUDED
