#ifndef debug_h
#define debug_h


void trace(char* msg);
void traceln(char* msg);
void tracehex(uint16_t num, uint8_t len);
void tracedec(uint16_t num, uint8_t len);

#define tracenum(num) tracehex(num, 4)
#define tracenet(num) tracehex(num, 2)

void stepInit(void);
void step(void);

#endif