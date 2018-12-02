// ROM address to store learned values in EEPROM:
#define LADDR_CTRL 0x01 
#define LEARN_INIT_CH 0x7f //control char so we know we learnt at least once
#define LADDR_PMIN  0x02 //int = 2 bytes offset
#define LADDR_PMAX    0x04 //int = 2 bytes offset
#define LADDR_FIXED1  0x06//first byte - fixed 1 value, second byte - control char to know it was saved at least once. 
#define LADDR_FIXED2  0x08//first byte - fixed 2 value, second byte - control char to know it was saved at least once. 
#define LADDR_FIXED3  0x0A//first byte - fixed 3 value, second byte - control char to know it was saved at least once. 
#define LEARNED_FIXED_CH 0xAf //control char so we know we saved fixed value(s)
#define LADDR_RMA_CTRL  0x0C 
#define LEARN_RMA_CH 0xCf //control char so we know we learnt at least once
#define LADDR_RMA  0x0D //next 127 up to 0x8C ar Resistance Matrix Array (recheck the calc!!!)

#define eeprom_verify(a,b) (EEPROM.read(a)==b)


