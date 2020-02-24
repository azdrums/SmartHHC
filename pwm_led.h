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

#ifndef PWM_LED_H_INCLUDED
#define PWM_LED_H_INCLUDED

//For LED, as we drive it with PWM
#define PWM_MAX 255  
#define On PWM_MAX
#define Off 0

void Led(byte L);
void doLed();
void calcPWMcoef();

extern byte idxLed;
extern float pwmCoef;

#define LedInitialBlink Led(On); delay(100);Led(Off);delay(100);Led(On);delay(100);Led(Off);

#endif // PWMLED_H_INCLUDED

