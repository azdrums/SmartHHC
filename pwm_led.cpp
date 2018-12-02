#include <Arduino.h>
#include "define.h"


byte lastled=0; 
byte pwm=PWM_MAX;
byte idxLed=0;
byte iLed=0;

unsigned long tmLed=0; 



//LED blink intervals in all modes (one in a row: Learn, F1, F2, F3, LF1, LF2, LF3) Prepared by setting idxLed - set in btn handlers, as well as F mode handler. 
int arrBlink[42]={
800,800,800,800,800,800 //LN
,200,0,0,0,0,800 //F1
,120,140,240,0,0,800 //F2
,120,140,140,140,240,800 //F3
,800,0,0,0,0,200 //TnF1
,400,100,400,0,0,800 //TnF2
,400,100,400,100,400,800 //TnF3
};


void Led(byte L) { 
  if(L!=lastled) {analogWrite(LED_PIN,L);
  lastled=L;
 } 
}


void doLed() {
 if (opMode==OP_NORMAL) {
  pwm=(byte) ((mcpreg * pwmCoef) + 0.5);
#if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_LED && opMode==OP_NORMAL) { Serial.print("pwm:"); Serial.println(pwm); }
#endif //HAVE_SERIAL_DEBUG
  Led(pwm);
  return;
 } 

 if (idxLed % 6 == 0) { //% is the reminder from operation devide. If it devides by 6 with no reminder => start a new sequence here by making iLed=idxLed
     Led(On);
     tmLed=millis();
     iLed=idxLed;
     idxLed++;//so we don't come here until next blink init. (idxLed  % 6 == 0) will be always true otherwise.
     return;
 }
 if (millis() - tmLed >= arrBlink[iLed]) {
     tmLed=millis();
     if (iLed % 2 == 0) Led(Off); else Led(On); 
     iLed++;
     if (iLed==(idxLed+5)) iLed=idxLed-1;//it's actually (idxLed-1)+6. 
 }
  
}

void calcPWMcoef() {
  pwmCoef=PWM_MAX/MCP_MAX;
  pwmCoef=(int)(pwmCoef*100)+0.5;
  pwmCoef=(pwmCoef/100); 
#if HAVE_SERIAL_DEBUG 
  Serial.print("pwc:"); Serial.println(pwmCoef);
#endif  //HAVE_SERIAL_DEBUG

}
