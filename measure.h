

unsigned int measure ();
unsigned int abs_measure();
#if AV_M_ITERATIONS
  void prepareAV_M();
#endif
#if HAVE_SERIAL 
 void printArray(String msg, int* myArray,byte c);
#endif //HAVE_SERIAL


extern unsigned int p_min,p_max,lastmsr;
