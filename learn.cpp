/*
  SmartHHC - Electronic Drums HiHat Controller Firmware
  Copyright (C) 2018-2020 Lyudmil Tachev

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <Arduino.h>
#include <EEPROM.h>
#include "define.h"

union Int ADC0;//used for easier writing of int to EEPROM, which is addressed in bytes.
unsigned long tmLP=0;
float rCoef=0.2;
float pwmCoef=2;

byte mM_has_changed=0, MCP_Out_has_changed=0;  // flags altered by the Set command to indicate it can be written to EEPROM

void Learn(){
  unsigned int a=0;
   
    if (millis()-tmLP < LEARN_DURATION) {
      a=measure();
      if ( a > p_max) p_max=a;
      if ( a < p_min) p_min=a;
      if (LEARN_M_DELAY) delay(LEARN_M_DELAY);
    } else { 
#if HAVE_SERIAL_DEBUG
  if ( sDebug == SER_DEBUG_EVENTS) {
        Serial.print("LN-0;");
  }      
#endif
     
#if LEARN_CORR_MIN      
      p_min+=LEARN_CORR_MIN;
#endif
#if LEARN_CORR_MAX
      p_max-=LEARN_CORR_MAX;
#endif      
      calc_rCoef();
      mM_has_changed=1;
      MinMaxSaveEEPROM();
#if HAVE_SERIAL_DEBUG
      Serial.print("m:"); Serial.print(p_min);Serial.print(";M:"); Serial.print(p_max);  
      Serial.print(";mc:"); Serial.println(rCoef);
#endif           
  }
}

void MinMaxLoadEEPROM() {
  if (! eeprom_verify(LADDR_CTRL,LEARN_INIT_CH)) return;
  
  ADC0.b[0]=EEPROM.read(LADDR_PMIN);
  ADC0.b[1]=EEPROM.read(LADDR_PMIN+1);
  p_min=ADC0.I;
  ADC0.b[0]=EEPROM.read(LADDR_PMAX);
  ADC0.b[1]=EEPROM.read(LADDR_PMAX+1);
  p_max=ADC0.I;
    
  calc_rCoef();
#if HAVE_SERIAL_DEBUG
   Serial.print("m:"); Serial.print(p_min);Serial.print(";M:"); Serial.print(p_max);  
   Serial.print(";mc:"); Serial.println(rCoef);
#endif        
}

void MinMaxSaveEEPROM() {
  if (! mM_has_changed ) return; // nothing to do
  //write to EEPROM
  ADC0.I=p_min;
   EEPROM.write(LADDR_PMIN,ADC0.b[0]);
   EEPROM.write(LADDR_PMIN+1,ADC0.b[1]);
  ADC0.I=p_max;
   EEPROM.write(LADDR_PMAX,ADC0.b[0]);
   EEPROM.write(LADDR_PMAX+1,ADC0.b[1]);
   EEPROM.write (LADDR_CTRL,LEARN_INIT_CH);//confirm write success by writting control value
  opMode=OP_NORMAL; 
  mM_has_changed=0; // clear the flag
}

void RMALoadEEPROM() {
   if (! eeprom_verify(LADDR_RMA_CTRL,LEARN_RMA_CH)) return;
   for (byte i=0;i<=MCP_MAX; i++) {
     MCP_out[i]=EEPROM.read(LADDR_RMA+i);
   }
#if HAVE_SERIAL_DEBUG
    printArray("rma",MCP_out,MCP_MAX+1);
#endif        

}

void RMASaveEEPROM() {
  if (! MCP_Out_has_changed ) return; // nothing to do
  for (byte i=0;i<=MCP_MAX; i++) {
     EEPROM.write(LADDR_RMA+i,MCP_out[i]);
  }
  EEPROM.write(LADDR_RMA_CTRL,LEARN_RMA_CH);//confirm write success by writting control value
  MCP_Out_has_changed=0; // clear the flag
}

void LearnStart() {
      tmLP=millis();
      p_min=0xffff;
      p_max=0;
      opMode=OP_LEARN;
      idxLed=0;
}

void calc_rCoef() {
//calculate coefs
  rCoef=p_max-p_min; rCoef=MCP_MAX/rCoef;
  rCoef=(int)((rCoef*1000)+0.005); rCoef=(rCoef/1000);//round to 3 after 0
}
