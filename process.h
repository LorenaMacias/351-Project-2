#ifndef Project3_process_h
#define Project3_process_h

typedef struct {
    int pid;
    int arrival_time;
    int life_time;
    int mem_reqs;

    int time_added_to_memory;
    int is_active;
    int time_finished;
} PROCESS;
#endif

