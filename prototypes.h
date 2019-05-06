#include "process.h"
#include "queue.h"
#include "memory.h"

//makes a list of processes given the filepath
PROCESS* assign_process_list(const char* file_path);

//returns a string or tab based on whats been announced (if any) 
char* get_announcement_prefix(int current_time);

//gets number of processes (first num is provided by the file)
int get_number_of_processes_from_file(FILE* filePtr);

//the main function
int main();

//asks for number input from user and checks if its valid provided by function pointer
int process_numeric_input_from_user(const char* output, int (*func)(int));

//assigns available memory to the waiting procs that can fit in
void assign_available_memory_to_waiting_procs(int current_time);

//clears any extra chars from stdin
void clear_stdin(char* buf);

//removes the completed procs from memory
void terminate_completed_procs(int current_time);

//adds newly arrived procs to the input queue
void enqueue_newly_arrived_procs(int current_time);

//gets the user input (mem size, number of pages, path to input file)
void get_user_input(int* mem, int* page, char* file_path);

//returns a 1 if the int is a 1, 2, or 3 and a 0 if not
int is_one_two_or_three(int t);

//the main program loop
void main_loop();

//returns a 1 if the int is a multiple of 100 and a 0 if not
int multiple_of_one_hundred(int t);

//prints the average turnaround time
void print_turnaround_times();

//prompts the user for a valid filename
void prompt_for_filename(char* res);
