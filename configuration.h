#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED

#if defined GIT_VERSION
  #define ABOUT_STR  "SmartHHC hw1.0 fw-"GIT_VERSION"-git READY"
#else
  #define ABOUT_STR  "SmartHHC hw1.0 fw0.9-dev READY"
#endif

//pin definitions
#define MCP_CS 8 // The digital pot MCP41050 CS( chip select) pin is connected to this Arduino pin:
#define I2C_PU_PIN A2
#define IR_PIN 6//The InfraRed emmiting diode (the blue half) of the TCRT5000 sensor connected to this Arduino  pin:
#define S_PIN A7 //The Photo Transistor (the black half) of the TCRT5000 sensor connected to this Arduino pin:
#define LED_PIN 3 //LED connected to this Arduino pin
#define BTN_PIN 2 //Button conected to this Arduino pin


//output normalization settings (in theory may add lag). Comment out to disable.  
#define AV_M_ITERATIONS 3 //Output will be based on the average of that much previous sensor reads 
#define MCP_MIN_DIFF 2 //Do not set new pot value if difference from previous measurement is not at least that much. 

#define PP_MIN_DIFF 5 //minimum ADC value change to update host of

/*init ser and output debug messages? One of:
SER_DEBUG_PLOTTER - only resistance value (usefull for the "serial plotter" IDE tool)
SER_DEBUG_DETAIL_MSR - output detailed measurement debug , 
SER_DEBUG_EVENTS_ONLY - events without measured data
Keep commented out for real operation
*/
#define HAVE_SERIAL  1

//instead of output the measured/calculated value, use it as an index to an array to map the output resistance. That way different curves can be implemented. A Must with ATV
#define MCP_MATRIX \
0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,8,4,4,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,\
10,10,10,11,11,11,12,12,12,13,13,13,14,14,15,15,16,16,17,17,18,18,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,\
27,28,28,29,29,30,33,37,40,44,47,50,54,57,61,64,67,71,74,78,81,84,88,91,95,98,101,105,108,112,114,115



#endif //CONF_H_INCLUDED
