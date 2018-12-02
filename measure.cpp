#include <Arduino.h>
#include "define.h"

unsigned int p_min=P_MIN_DEFAULT,p_max=P_MAX_DEFAULT,lastmsr=0;

#if AV_M_ITERATIONS
unsigned long avm=0; //average measure sum holder
int avma[AV_M_ITERATIONS]={}; //average measure array holder
byte avmi=0;//average measure index
#endif // AV_M_ITERATIONS
#if HAVE_SERIAL 
void printArray(String msg, int* myArray,byte c);
#endif //HAVE_SERIAL


unsigned int measure () {
#if AV_M_ITERATIONS  
    avma[avmi]=analogRead(S_PIN);
    avmi++;
    if (avmi>=AV_M_ITERATIONS) avmi=0;
    avm=0;
    for (byte i=0;i<AV_M_ITERATIONS; i++) avm+=avma[i];
    unsigned int am=(unsigned int) ((avm/AV_M_ITERATIONS) + 0.5);
 #if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_DETAIL_MSR) { 
      Serial.print(";av:");Serial.print(am);Serial.print("t:");Serial.print(avm);printArray("arr:", avma, AV_M_ITERATIONS);
  }
 #endif //HAVE_SERIAL_DEBUG     
    lastmsr=am;
    return am;
#else //AV_M_ITERATIONS
  unsigned int  m=analogRead(S_PIN);  
 #if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_DETAIL_MSR) { 
  Serial.print("msr:"); Serial.println(m);
 #endif //HAVE_SERIAL_DEBUG
  lastmsr=m;
  return m;
#endif //AV_M_ITERATIONS
}

//absolute range measure, returns position ofset (0 - RANGE)
unsigned int abs_measure() {
  unsigned int avm=measure();
  if (avm<p_min) avm=p_min;
  if (avm>p_max) avm=p_max;
#if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_DETAIL_MSR) {  
   Serial.print(";m:"); Serial.print(avm);
  }
#endif //HAVE_SERIAL_DEBUG
  avm-=p_min;
#if HAVE_SERIAL_DEBUG
  if (sDebug==SER_DEBUG_DETAIL_MSR) { 
   Serial.print(";abs:"); Serial.println(avm);
  }
#endif //HAVE_SERIAL_DEBUG
  return avm;  
}
#if HAVE_SERIAL
void printArray(String msg, int* myArray,byte c) {
if (msg != NULL) {
 Serial.print(msg);
 Serial.print(":");
} 
for (byte i=0;i<c;i++) {
   Serial.print(myArray[i]);
   if (i<c-1) Serial.print("|");
   else Serial.println(""); //Serial.println(";");
 }
}
#endif //HAVE_SERIAL
#if AV_M_ITERATIONS
void prepareAV_M() {
 for (avmi=0; avmi<AV_M_ITERATIONS; avmi++) { 
     avma[avmi]=analogRead(S_PIN);
   } 
 avmi=0;
}
#endif //AV_M_ITERATIONS


