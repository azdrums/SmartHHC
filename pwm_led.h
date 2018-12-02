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

