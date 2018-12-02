#include <Arduino.h>
#include "define.h"


byte lastled=0; 
byte pwm=PWM_MAX;
byte idxLed=0;
byte iLed=0;

unsigned long tmLed=0; 



//LED blink intervals in all modes (one in a row: Learn, F1, F2, F3) 
int arrBlink[24]={
800,800,800,800,800,800
,100,0,0,0,0,1000
,100,100,100,0,0,1000
,100,100,100,100,100,1000
};


void Led(byte L) { 
  if(L!=lastled) {analogWrite(LED_PIN,L);
  lastled=L;
 } 
}


void doLed() {
 if (opMode==OP_NORMAL || opMode==OP_FIXED) {
  pwm=(byte) ((mcpreg * pwmCoef) + 0.5);
#if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_LED && opMode==OP_NORMAL) { Serial.print("pwm:"); Serial.println(pwm); }
#endif //HAVE_SERIAL_DEBUG
  Led(pwm);
  return;
 } 

 if (idxLed % 6 == 0) {
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

