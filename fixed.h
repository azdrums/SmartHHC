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

#ifndef FXD_H_INCLUDED
#define FXD_H_INCLUDED


#define MCP_FIXED_TUNE_STEP 1 //change resistance with that much at a time when tuning fixed positions. 
#define MCP_FIXED1_DEFAULT 2  //Default value to set in the pot when in Fixed1 mode.
#define MCP_FIXED2_DEFAULT 6  //Default value to set in the pot when in Fixed2 mode.
#define MCP_FIXED3_DEFAULT 12  //Default value to set in the pot when in Fixed3 mode.


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
