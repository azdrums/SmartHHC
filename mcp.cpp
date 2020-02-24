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
#include "define.h"
#if MCP_MODE == MODE_SPI
#include <SPI.h>
#elif MCP_MODE == MODE_I2C
#include <Wire.h>
#endif


byte mcp=MCP_MAX,mcpreg=0;

#ifdef MCP_MATRIX
  int MCP_out[MCP_MAX+1]={MCP_MATRIX};
#endif
void mcpWrite(byte value)
{
  if (mcpreg == value) return;
#if MCP_MODE == MODE_SPI  
  digitalWrite(MCP_CS, LOW);
  delay(1);
  SPI.transfer(MCP_CMD);
  SPI.transfer(value);
  digitalWrite(MCP_CS, HIGH);
#elif MCP_MODE == MODE_I2C
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(value); // 
  Wire.endTransmission();
#endif
  mcpreg=value;
#if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_DETAIL_MSR ) {
   Serial.print("mw:"); Serial.println(mcpreg);
  } else if ( sDebug==SER_DEBUG_PLOTTER ) {
   Serial.println(mcpreg);
  }
#endif //HAVE_SERIAL_DEBUG
}

void doMCP() {

#if MCP_MIN_DIFF  
 byte m=(byte) ( (abs_measure() * rCoef) + 0.5);
 #if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_DETAIL_MSR ) {
   Serial.print("mcp:"); Serial.print(m);
  }
 #endif //HAVE_SERIAL_DEBUG
 #ifdef MCP_MATRIX
  if (MCP_out[mcp]!=mcpreg) mcp=0xff; //we are coming from fixed and need to reset this check
 #else //! MCP_MATRIX
  if (mcp!=mcpreg) mcp=0xff; //we are coming from fixed and need to reset this check
 #endif //MCP_MATRIX
 if (abs(m-mcp) <MCP_MIN_DIFF) return; //normalize
 mcp=m;
#else //! MCP_MIN_DIFF
 mcp=(byte) ( (abs_measure() * rCoef) + 0.5);
#endif //MCP_MIN_DIFF

#if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_DETAIL_MSR ) {
   Serial.print("mc:"); Serial.print(mcp);
  }
#endif //HAVE_SERIAL_DEBUG
 
#ifdef MCP_MATRIX
  mcpWrite(MCP_out[mcp]);
#else //! MCP_MATRIX
  mcpWrite(mcp);
#endif //MCP_MATRIX

}


void mcpInitial() {
#if MCP_MODE == MODE_SPI  
  SPI.begin();
#elif MCP_MODE == MODE_I2C
  pinMode(I2C_PU_PIN, OUTPUT);
  digitalWrite(I2C_PU_PIN, HIGH);
  Wire.begin();
  cbi(PORTC, 4);
  cbi(PORTC, 5);
#endif
  mcpWrite(mcp);
}

