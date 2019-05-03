#include "process.h"
#include "queue.h"
#include "memory.h"

//makes a list of processes given the filepath
PROCESS* assignProcessList(const char* file_path);

//returns a string or tab based on whats been announced (if any) 
char* getAnnouncementPrefix(int current_time);

//gets number of processes (first num is provided by the file)
int getNumberOfProcessesFromFile(FILE* filePtr);

//the main function
int main();

//asks for number input from user and checks if its valid provided by function pointer
int processNumericInputFromUser(const char* output, int (*func)(int));

//assigns available memory to the waiting procs that can fit in
void assignAvailableMemoryToWaitingProcs(int current_time);

//clears any extra chars from stdin
void clearStdin(char* buf);

//removes the completed procs from memory
void terminateCompletedProcs(int current_time);

//adds newly arrived procs to the input queue
void enqueueNewlyArrivedProcs(int current_time);

//gets the user input (mem size, number of pages, path to input file)
void getUserInput(int* mem, int* page, char* file_path);

//returns a 1 if the int is a 1, 2, or 3 and a 0 if not
int isOneTwoOrThree(int t);

//the main program loop
void mainLoop();

//returns a 1 if the int is a multiple of 100 and a 0 if not
int multipleOfOneHundred(int t);

//prints the average turnaround time
void printTurnaroundTimes();

//prompts the user for a valid filename
void promptFile(char* res);
