#ifndef Project3_process_h
#define Project3_process_h

typedef struct {
    int pid;
    int arrivalTime;
    int lifeTime;
    int memReqs;

    int timeAddedToMemory;
    int isActive;
    int timeFinished;
} PROCESS;
#endif

