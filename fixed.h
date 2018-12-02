#ifndef FXD_H_INCLUDED
#define FXD_H_INCLUDED


#define MCP_FIXED_TUNE_STEP 1 //change resistance with that much at a time when tuning fixed positions. 
#define MCP_FIXED1_DEFAULT 10  //Default value to set in the pot when in Fixed1 mode.
#define MCP_FIXED2_DEFAULT 20  //Default value to set in the pot when in Fixed2 mode.
#define MCP_FIXED3_DEFAULT 30  //Default value to set in the pot when in Fixed3 mode.


void Fixed_OnOff();
void TuneFixed();
void TuneFixedEnd();
void FixedLoadEEPROM();
void FixedSaveEEPROM_f1();
void FixedSaveEEPROM_f2();
void FixedSaveEEPROM_f3();


extern byte mcpf1,mcpf2,mcpf3,idxFixedPos;
extern byte f1_has_changed,f2_has_changed,f3_has_changed;

#endif //FXD_H_INCLUDED
