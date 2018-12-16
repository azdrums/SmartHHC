#include <Arduino.h>
#include "define.h"

void(* resetDo) (void) = 0; //declare reset function @ address 0

#if HAVE_SERIAL

#include "SerialCommand.h"

SerialCommand sCmd;     // The SerialCommand object

boolean hostConnected=false;
long tmHostUpd;
int HostUpdInt=HOST_UPDATE_MIN_INT;
int lastSent_P,lastSent_m,lastSent_M=0;
byte lastSent_r=0xff,lastSent_f1=0,lastSent_f2=0,lastSent_f3=0;
boolean MCP_Out_sent=false;

void UpdateHost() {
  boolean c=false;  
  if (opMode != OP_NORMAL ) return; //not updating in learning modes 
  if (millis()-tmHostUpd < HostUpdInt) return; //not updating too frequently

  
#if PP_MIN_DIFF //used here to normalize raw/abs pedal position because it fluctuates. Is not used also on mcpreg, as it is already applied to it by the normal measure algo
   if (getdelta(lastSent_P,lastmsr)>PP_MIN_DIFF) {
#else
   if (lastSent_P != lastmsr) {
#endif    
    Serial.print("P:"); Serial.println(lastmsr);
    lastSent_P = lastmsr;
    c=true;
   }
   if (lastSent_r != mcp) {
    Serial.print("i:"); Serial.println(mcp);
    lastSent_r = mcp;
    c=true;
   }

   if (lastSent_f1 != mcpf1) {
    Serial.print("f1:"); Serial.println(mcpf1);
    lastSent_f1 = mcpf1;
    c=true;
   }

   if (lastSent_f2 != mcpf2) {
    Serial.print("f2:"); Serial.println(mcpf2);
    lastSent_f2 = mcpf2;
    c=true;
   }

   if (lastSent_f3 != mcpf3) {
    Serial.print("f3:"); Serial.println(mcpf3);
    lastSent_f3 = mcpf3;
    c=true;
   }

   if (lastSent_m != p_min) {
    Serial.print("m:"); Serial.println(p_min);
    lastSent_m = p_min;
    c=true;
   }

   if (lastSent_M != p_max) {
    Serial.print("M:"); Serial.println(p_max);
    lastSent_M = p_max;
    c=true;
   }
   
   if (! MCP_Out_sent) {
     printArray("R",MCP_out,MCP_MAX+1);
     MCP_Out_sent=true;
     c=true;
   }
   
  if (c) tmHostUpd=millis();

}

void hcProcess() {
 
    sCmd.readSerial();
    if (hostConnected) UpdateHost();

}


void hc_about() {
    
    Serial.println();
    Serial.println(ABOUT_STR);
}

void hc_G() {
  char *arg;
  byte a;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  Serial.print(arg);Serial.print(':');
  if (strcmp(arg,"m")==0) { Serial.println(p_min);} //get MIN ADC level
  else if (strcmp(arg,"M")==0) { Serial.println(p_max);}  //get MAX ADC level
  else if (strcmp(arg,"f1")==0) { Serial.println(mcpf1);} // get fixed position 1 MCP value
  else if (strcmp(arg,"f2")==0) { Serial.println(mcpf2);} // get fixed position 2 MCP value
  else if (strcmp(arg,"f3")==0) { Serial.println(mcpf3);} // get fixed position 2 MCP value
  else if (strcmp(arg,"D")==0)  { //get debug level if HAVE_SERIAL_DEBUG is defined
#if HAVE_SERIAL_DEBUG  
    Serial.println(sDebug); 
#else
   Serial.println(-1);  
#endif
  }  
  else if (strcmp(arg,"P")==0) {  Serial.println(lastmsr);}   //RAW ADC read from sensor - actual pedal position, despite min and MAX
  else if (strcmp(arg,"i")==0) {  Serial.println(mcp);}   // the current output array pointer, eg MCP_Out[r] 
  else if (strcmp(arg,"R")==0) {  printArray("",MCP_out,MCP_MAX+1);}
  else if (strcmp(arg,"r")==0) { //get array member value
    arg = sCmd.next();    
    a=atoi(arg);
    Serial.print(a);Serial.print(':');    
    if (a <= MCP_MAX) { Serial.println(MCP_out[a]); }
    else { Serial.print("E:OoR");}
  }
  else {Serial.println("E:PNR"); }
}

void hc_S() {
  char *arg;
  byte a;
  int j;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  //Serial.print("S:");
  Serial.print(arg);Serial.print(':');
 if (strcmp(arg,"m")==0) { 
      arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
      j=atoi(arg);
      if ( (j) && (j != p_min) && ((j < p_max)) ) { 
        p_min=j;
        calc_rCoef();
        mM_has_changed=1;
      }
    Serial.println(p_min);
  } 
  else if (strcmp(arg,"M")==0) { 
      arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
      j=atoi(arg);
      if ( (j) && (j != p_max) && ((j > p_min)) ) { 
        p_max=j;
        calc_rCoef();
        mM_has_changed=1;
      }
    Serial.println(p_max);
  } 
  else if (strcmp(arg,"f1")==0) { 
      arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
      a=atoi(arg); 
      if ( (a <= MCP_MAX) and (a != mcpf1)) { 
          mcpf1=a;
          f1_has_changed=1;
          if ( ((opMode == OP_TUNE_FIXED)||(opMode == OP_FIXED)) && (idxFixedPos==1) ) {
             mcpWrite(mcpf1);
          }
      }
      Serial.println(mcpf1);
  }
    else if (strcmp(arg,"f2")==0) { 
      arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
      a=atoi(arg); 
      if ( (a <= MCP_MAX) and (a != mcpf2)) { 
          mcpf2=a;
          f2_has_changed=1;
          if ( ((opMode == OP_TUNE_FIXED)||(opMode == OP_FIXED)) && (idxFixedPos==2) ) {
             mcpWrite(mcpf2);
          }
      }
      Serial.println(mcpf2);
  }
  else if (strcmp(arg,"f3")==0) { 
      arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
      a=atoi(arg); 
      if ( (a <= MCP_MAX) and (a != mcpf3)) { 
          mcpf3=a;
          f3_has_changed=1;
          if ( ((opMode == OP_TUNE_FIXED)||(opMode == OP_FIXED)) && (idxFixedPos==3) ) {
             mcpWrite(mcpf3);
          }
      }
      Serial.println(mcpf3);
  }
  else if (strcmp(arg,"D")==0) { 
#if HAVE_SERIAL_DEBUG  
    arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
    a=atoi(arg);
    Serial.println(a);
    sDebug=a;
#else
      Serial.println(-1);  
#endif
  } 
  else if (strcmp(arg,"r")==0) { 
    arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
    if (arg == NULL ) {Serial.println("E:NaR"); return;}
    Serial.print(arg);Serial.print(':'); 
     a=atoi(arg);
      if (a <= MCP_MAX) { 
        arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
        byte r=atoi(arg);
        if (r <= MCP_MAX) { 
          MCP_out[a]=r;
          MCP_Out_has_changed=true;
          Serial.println(MCP_out[a]);  
        }
        else { Serial.print("E:OoR");}
      }
      else { Serial.print("E:OoR");}
  } 
  else {Serial.println("E:PNR"); } 
}

void hc_W() {
  char *arg;
  byte a;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  Serial.print("W:");Serial.print(arg);Serial.print(':');
  if (strcmp(arg,"m")==0) { 
      MinMaxSaveEEPROM();
      Serial.println('w');
  } else if (strcmp(arg,"R")==0) { 
      RMASaveEEPROM();
      Serial.println("w");
  } else if (strcmp(arg,"f1")==0) { 
      if (f1_has_changed) {
        FixedSaveEEPROM_f1();
        f1_has_changed=0;
      }
      Serial.println("w");
  } else if (strcmp(arg,"f2")==0) { 
      if (f2_has_changed) {
        FixedSaveEEPROM_f2();
        f2_has_changed=0;
      }
      Serial.println("w");
  } else if (strcmp(arg,"f3")==0) { 
      if (f3_has_changed) {
        FixedSaveEEPROM_f3();
        f3_has_changed=0;
      }
      Serial.println("w");
  }
  else {Serial.println("E:PNR"); } 
  
}

void hc_C() {
  char *arg;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  Serial.print("C:");
  HostUpdInt=atoi(arg);
  if ((HostUpdInt) && (HostUpdInt < HOST_UPDATE_MIN_INT)) HostUpdInt=HOST_UPDATE_MIN_INT; //can't be shorter then this
  hostConnected=(HostUpdInt);
  if (hostConnected) {
          Serial.println(HostUpdInt);
  }
  else {
      lastSent_P=0;
      lastSent_r=0xff;
      lastSent_m=0;
      lastSent_M=0;
      lastSent_f1=0;
      lastSent_f2=0;
      lastSent_f3=0;
      MCP_Out_sent=false;
      Serial.println("0");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void hc_NULL(const char *command) {
  Serial.println("E:CNR");
}

void hc_reset() {
  
Serial.println("resetting... ");
resetDo();
Serial.println("after reset \n");   
}

void hcInit() {
 sCmd.addCommand("RESET",hc_reset);        // DOes a soft-reset by jumping to address 0
 sCmd.addCommand("RST",hc_reset);        // DOes a soft-reset by jumping to address 0
 sCmd.addCommand("HELO", hc_about);        // Echos the string argument back
 sCmd.addCommand("HELLO", hc_about);        // Echos the string argument back
 sCmd.addCommand("?", hc_about);        // Echos the string argument back
 sCmd.addCommand("G", hc_G);        //get values command
 sCmd.addCommand("S", hc_S);        //set values command
 sCmd.addCommand("W", hc_W);        //write to EEPROM command
 sCmd.addCommand("C", hc_C);        //write to EEPROM command
 sCmd.setDefaultHandler(hc_NULL); 
 hc_about();
}
#endif //HAVE_SERIAL
