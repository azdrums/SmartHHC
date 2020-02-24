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

#ifndef MCP_H_INCLUDED
#define MCP_H_INCLUDED


#define MODE_SPI  1
#define MODE_I2C  2

#define MCP_MODE MODE_I2C //MODE_SPI

#if MCP_MODE==MODE_SPI
//Digital pot MCP41050 related settings
#define MCP_CMD 0b00010001 //DO NOT CHANGE - this is a control word used for normal POT operation - consult datasheed if interested how it was contructed !!!
#elif MCP_MODE==MODE_I2C
//Microchip MCP4017 I2C 5K, 10K, 50K and 100K digital potentiometers, default i2c address (only address really)
#define MCP_ADDR 0b0101111 //0x2f

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#endif

#define MCP_MAX 127 //max value that can be written to the pot, also the size of the MCP_Out map array and basically the resolution of the system




extern byte mcpreg;//holds last value written to MCP register. Used by PWM calculation for LED in normal mode and by measurement routines as history value holder
extern byte mcp;//holds the MCP_Out index of the last value written to MCP register
 
#ifdef MCP_MATRIX
extern int MCP_out[MCP_MAX+1];
#endif

void mcpWrite(byte value);
void doMCP();
void mcpInitial();
 
#endif //MCP_H_INCLUDED
 
