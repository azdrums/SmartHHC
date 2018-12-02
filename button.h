#ifndef BTN_H_INCLUDED
#define BTN_H_INCLUDED


//button related definitions
#define BTN_DB_W 30 //debounce wait interval - after initial change detection, wait that much and check again - this is needed so we don't detect the initial contact state short bounces as separate events.   
#define BTN_LONG_W 2000 //if btn remains depressed for longer then this, raise a 'long press' event.
#define BTN_INTERVAL 500 //max interval between press/release cycles to thread the cycle as next in a sequence
#define BTN_EVENTS_CNT 3 //how many events we handle based on sequential btn press/release cycles

#define BTN_STATE_ON 0 //event code
#define BTN_STATE_OFF 1 //event code
#define BTN_STATE_LONG_PRESS 2 //event code



void btnScan();
void btnHandle();
void btnInitial();

extern byte btnCnt;

#endif //BTN_H_INCLUDED
