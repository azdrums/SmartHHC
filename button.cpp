#include <Arduino.h>
#include "define.h"

//button

byte btnDbOn=0;//flag to indicate debounce wait is in progress
byte btnState=0;
byte btnPrevSt=0;
byte btnCnt=0;
unsigned long tmBtn=0;


void btnOnPress();
void btnOnLongPress();
void btnOnRelease();
void btnOnReleaseAfterLP();


void btnScan() 
  { unsigned char b=0;    
      //1. still in debounce wait
      if ( (btnDbOn) && ((millis() - tmBtn) < BTN_DB_W)) return; 
 
      b=digitalRead(BTN_PIN); //read current btn state
      
      //2.in a long press still
      if ((b==BTN_STATE_ON) && (btnState==BTN_STATE_LONG_PRESS)) return; 
      
     //3. no new event - but handle long press, between press intervals or clear debounce for false events
      if ( b  == btnState) {
        if (btnDbOn) { //clear false trigger that didn't persist after debounce wait. 
          btnDbOn=0; 
        }
        else {//hanadle intervals
            if ( (b==BTN_STATE_ON) && ((millis() - tmBtn) >= BTN_LONG_W)) btnState=BTN_STATE_LONG_PRESS; //long press
            else if ( (b==BTN_STATE_OFF) && ((millis() - tmBtn) >= BTN_INTERVAL)) btnCnt=0; //clear sequential press counter - start over 
         }
        return;    
      }
                  
      //4.new btn event
      if ( b  != btnState ) {
        
        if (! btnDbOn)  { //just occured, start a debounce wait
          tmBtn=millis(); //record event time
          btnDbOn=1; //mark we started the wait  
          return;
        } else {//a real event - confirmed by persisting after the debounce wait ..
          btnDbOn=0;
          btnState=b;
          btnCnt++;
        }
      }
}


void btnHandle() 
{
if (btnState==btnPrevSt) return; //nothing to process
if (opMode==OP_LEARN) return;

switch (btnState) {
 case BTN_STATE_LONG_PRESS:
  btnOnLongPress();
  break;
 case BTN_STATE_ON:
  btnOnPress();
  break;   
 case BTN_STATE_OFF:
  if (btnPrevSt==BTN_STATE_LONG_PRESS) {
    btnOnReleaseAfterLP();
    //reset sequent count 
    btnCnt=0; //-=2 if don't want a complete reset
  }
  else {
    if ((btnPrevSt==BTN_STATE_ON) && (btnCnt>(BTN_EVENTS_CNT*2))) btnCnt=2; //reset seq count
    btnOnRelease();
  }
  break;
 }
 btnPrevSt=btnState;//mark event as processed.     
  
}

void btnOnPress() {
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_BTN) {
            Serial.println("b1");
          }
#endif
}

void btnOnLongPress() {
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_BTN) {
              Serial.println("bL1");
          }
#endif
     switch (opMode) {
      case OP_NORMAL:
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_EVENTS) {
            Serial.println("LN-1");
          }  
#endif
        LearnStart();
        break;
     case OP_FIXED:
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_EVENTS) {
            Serial.println("FT-1");
          }
#endif
        opMode=OP_TUNE_FIXED;
        break;
     case OP_TUNE_FIXED:
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_EVENTS) {
            Serial.println("FT-0");
          }
#endif
        TuneFixedEnd();
        opMode=OP_NORMAL;
        break;
     }
}

void btnOnReleaseAfterLP() {
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_BTN) {
            Serial.println("bL0");
          }
#endif
}

void btnOnRelease() {
  if (btnCnt==2)  { 
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_BTN) {
            Serial.println("b0");
          }
#endif
  } else {
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_BTN) {
            Serial.print("b0-");Serial.println(btnCnt/2);
          }
#endif
  }
  if (opMode==OP_NORMAL || opMode==OP_FIXED) Fixed_OnOff();
  else if (opMode=OP_TUNE_FIXED) TuneFixed();
}

void btnInitial() {
  btnState=digitalRead(BTN_PIN);
  btnPrevSt=btnState;
  tmBtn=millis();
}

//button ends

