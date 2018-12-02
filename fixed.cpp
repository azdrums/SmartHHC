#include <Arduino.h>
#include <EEPROM.h>
#include "define.h"


byte mcpf1=MCP_FIXED1_DEFAULT,mcpf2=MCP_FIXED2_DEFAULT,mcpf3=MCP_FIXED3_DEFAULT;
byte idxFixedPos=1; 
byte f1_has_changed=0,f2_has_changed=0,f3_has_changed=0;


void Fixed_OnOff() {
  
  if (opMode==OP_FIXED){ 
    if (btnCnt==4) {
#if HAVE_SERIAL_DEBUG
  if ( sDebug == SER_DEBUG_EVENTS) {
      Serial.print("->f2,");
      Serial.println(mcpf2);
  }    
#endif
      idxLed=12;
      idxFixedPos=2;
      mcpWrite(mcpf2);
    } else        
    if (btnCnt==6) {
#if HAVE_SERIAL_DEBUG
  if ( sDebug == SER_DEBUG_EVENTS) {
      Serial.print("->f3,");
      Serial.println(mcpf3);
  }
#endif      
      idxLed=18;
      idxFixedPos=3;
      mcpWrite(mcpf3);
    } else       {
#if HAVE_SERIAL_DEBUG
  if ( sDebug == SER_DEBUG_EVENTS) {
      Serial.print("f"); Serial.print(idxFixedPos);Serial.println("->n"); 
  }    
#endif      
      opMode=OP_NORMAL;
      return;
    }  
  } 
  if (opMode==OP_NORMAL){ 
#if HAVE_SERIAL_DEBUG
  if ( sDebug == SER_DEBUG_EVENTS) {
    Serial.print("->f1,");
    Serial.println(mcpf1);
  }  
#endif    
    opMode=OP_FIXED;
    idxLed=6;
    idxFixedPos=1;
    mcpWrite(mcpf1);
    return;
  }
}

void TuneFixed() {
  byte mcpf=mcpreg;
  mcpf+=MCP_FIXED_TUNE_STEP;
  if (mcpf>MCP_MAX) mcpf=0;
  mcpWrite(mcpf);
#if HAVE_SERIAL_DEBUG
  if ( sDebug == SER_DEBUG_EVENTS) {
              Serial.print("/");Serial.println(mcpf);
  }
#endif
  return;
}

void TuneFixedEnd() {
  switch (idxFixedPos) {
    case 1:
        if (mcpf1!=mcpreg) {
          mcpf1=mcpreg;
          FixedSaveEEPROM_f1();
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_EVENTS) {
              Serial.print("f1e,nv:");Serial.println(mcpf1);
          }
       }  else if ( sDebug == SER_DEBUG_EVENTS) { 
              Serial.println("f1e,noc"); 
          }
    
#else
        }        
#endif
      break;
    case 2:
      if (mcpf2!=mcpreg) {
          mcpf2=mcpreg;
          FixedSaveEEPROM_f2();
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_EVENTS) {
              Serial.print("f2e,nv:");Serial.println(mcpf2);
          }
       }  else if ( sDebug == SER_DEBUG_EVENTS) { 
              Serial.println("f2e,noc"); 
          }
    
#else
        }        
#endif
      break;
     case 3:
      if (mcpf3!=mcpreg) {     
          mcpf3=mcpreg;
          FixedSaveEEPROM_f3();
#if HAVE_SERIAL_DEBUG
          if ( sDebug == SER_DEBUG_EVENTS) {
              Serial.print("f3e,nv:");Serial.println(mcpf3);
          }
       }  else if ( sDebug == SER_DEBUG_EVENTS) { 
              Serial.println("f3e,noc"); 
          }
    
#else
        }        
#endif
      break;
  }
}


void FixedSaveEEPROM_f1() {
    EEPROM.write(LADDR_FIXED1,mcpf1);
    EEPROM.write(LADDR_FIXED1+1,LEARNED_FIXED_CH);
}

void FixedSaveEEPROM_f2() {
    EEPROM.write(LADDR_FIXED2,mcpf2);
    EEPROM.write(LADDR_FIXED2+1,LEARNED_FIXED_CH);
}

void FixedSaveEEPROM_f3() {
    EEPROM.write(LADDR_FIXED3,mcpf3);
    EEPROM.write(LADDR_FIXED3+1,LEARNED_FIXED_CH);
}


void FixedLoadEEPROM() {
  //get stored fixed positions if saved at least once. 
  if eeprom_verify(LADDR_FIXED1+1,LEARNED_FIXED_CH) mcpf1=EEPROM.read(LADDR_FIXED1); 
  if eeprom_verify(LADDR_FIXED2+1,LEARNED_FIXED_CH) mcpf2=EEPROM.read(LADDR_FIXED2); 
  if eeprom_verify(LADDR_FIXED3+1,LEARNED_FIXED_CH) mcpf3=EEPROM.read(LADDR_FIXED3);
#if HAVE_SERIAL_DEBUG
    Serial.print("f1:"); Serial.print(mcpf1); Serial.print("f2:"); Serial.print(mcpf2); Serial.print("f3:"); Serial.println(mcpf3);
#endif
}

