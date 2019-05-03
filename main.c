#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "prototypes.h"

// global constants
const int TIME_MAX = 100000;

// shared data
int number_of_procs = 0, last_announcement = -1;
PROCESS* proc_list;
processQue* queue;
frame_list* framelist;

void mainLoop() {
    long current_time = 0;

    while (1) {
	//if a proc arrives then add it to the queue
        enqueueNewlyArrivedProcs(current_time);

	//if a proc completes, then remove it
        terminateCompletedProcs(current_time);

        //assign available memory to a process that need it
        assignAvailableMemoryToWaitingProcs(current_time);

        current_time++;

	//check for deadlocks 
        if (current_time > TIME_MAX) {
            printf("DEADLOCK: max time reached\n");
            break;
        }

        if (queue->size == 0 && frame_list_is_empty(framelist)) {
            break;
        }
    }

    //print out all the info for the procs
    printTurnaroundTimes();
}

int main() {
    int page_size  = 0;
    int mem_size   = 0;

    char* file_path = malloc(100 * sizeof(char));

    //get user input
    getUserInput(&mem_size, &page_size, file_path);

    //read values from the input file into a shared proc list
    proc_list = assignProcessList(file_path);

    //make a shared queue w/ a capacity equal to number of procs
    queue = create_processQue(number_of_procs);

    //make a shared framelist
    framelist = create_frame_list(mem_size / page_size, page_size);

    main_loop();

    return 0;
}

//function to add the processes into the queue
void enqueueNewlyArrivedProcs(int current_time) {
    int i;
    PROCESS* proc;

    for (i = 0; i < number_of_procs; i += 1) {
        proc = &proc_list[i];

	//announce the arrival times
        if (proc->arrivalTime == current_time) {
            printf("%sProcess %d arrives\n",
                   getAnnouncementPrefix(current_time),
                   proc->pid);

            enqueue_proc(queue, proc);

            print_processQue(queue);
        }
    }
}

//function to remove process from the queue and from the memory
void terminateCompletedProcs(int current_time) {
    int i, time_spent_in_memory;
    PROCESS* proc;

    // dequeue any procs that need it
    for (i = 0; i < number_of_procs; i += 1) {
        proc = &proc_list[i];
        time_spent_in_memory = current_time - proc->timeAddedToMemory;

        if (proc->isActive && (time_spent_in_memory >= proc->lifeTime)) {
            printf("%sProcess %d completes\n",
                   getAnnouncementPrefix(current_time),
                   proc->pid);

            proc->isActive = 0;
            proc->timeFinished = current_time;

            free_memory_for_pid(framelist, proc->pid);

            print_frame_list(framelist);
        }
    }
}

//this function allows any waiting process into queue if it can fit into memory 
void assignAvailableMemoryToWaitingProcs(int current_time) {
    int i, index, limit;
    PROCESS* proc;

    //set the limit to the size of the queue
    limit = queue->size;

    // enqueue any procs that can be put into mem
    for (i = 0; i < limit; i += 1) {
        index = iterate_queue_index(queue, i);
        proc = queue->elements[index];
	//if the process can fit into the mem then add it
        if (proc_can_fit_into_memory(framelist, proc)) {
            printf("%sMM moves Process %d to memory\n",
                   getAnnouncementPrefix(current_time),
                   proc->pid);

            fit_proc_into_memory(framelist, proc);

            proc->isActive = 1;
            proc->timeAddedToMemory = current_time;

            dequeue_proc_at_index(queue, i);
            print_processQue(queue);
            print_frame_list(framelist);
        }
    }
}

char* getAnnouncementPrefix(int current_time) {
    char* result;

    result = malloc(20 * sizeof(char));

    if (last_announcement == current_time) {
        sprintf(result, "\t");
    } else {
        sprintf(result, "t = %d: ", current_time);
    }

    last_announcement = current_time;

    return result;
}

void printTurnaroundTimes() {
    int i;
    float total = 0;

    //calculate the turnaround_times
    for (i = 0; i < number_of_procs; i += 1) {
        total += proc_list[i].timeFinished - proc_list[i].arrivalTime;
    }
    //print our the turnaraount times
    printf("Average Turnaround Time: %2.2f\n", total / number_of_procs);
}

int multipleOfOneHundred(int t) {
    return (t % 100) == 0 ? 1 : 0;
}

int isOneTwoOrThree(int t) {
    return (t >= 1 && t <= 3) ? 1 : 0;
}

void clearStdin(char* buf) {
    if (buf[strlen(buf) - 1] != '\n') {
        int ch;
        while (((ch = getchar()) != '\n') && (ch != EOF)) ;
    }
}

int process_numeric_input_from_user(const char* output, int (*func)(int)) {
    char buf[10];
    int success = 0;
    int res = 0;

    while (!success) {
        printf("%s: ", output);

        if (fgets(buf, 10, stdin) == NULL) {
            clearStdin(buf);
            printf("ERROR: You didn't enter any data!\n");

            continue;
        }

        if (sscanf(buf, "%d", &res) <= 0) {
            clearStdin(buf);
            printf("ERROR: You didn't enter a number!\n");

            continue;
        }

        if (!(success = (*func)(res))) {
            clearStdin(buf);
            printf("ERROR: That number is not a valid choice\n");
        }
    }

    return res;
}

//this functions gets the input from the user
void prompt_for_filename(char* res) {
    char buf[100];
    FILE* fp;

    while (1) {
        printf("Input file: ");


//IF STATEMENTS CHECK USER INPUT
        if (fgets(buf, 100, stdin) == NULL) {
            clearStdin(buf);
            printf("ERROR: You didn't enter any data!\n");

            continue;
        }

        if (sscanf(buf, "%s", res) <= 0) {
            clearStdin(buf);
            printf("ERROR: You didn't enter a string!\n");

            continue;
        }

        if (!(fp = fopen(res, "r"))) {
            perror("ERROR: Could not open file!\n");
        } else {
            break;
        }
    }
}

// prompts for memory size and page size
void getUserInput(int* mem, int* page, char* file_path) {
    while (1) {
        *mem = process_numeric_input_from_user(
            "Memory size", multipleOfOneHundred);

        *page = process_numeric_input_from_user(
            "Page size (1: 100, 2: 200, 3: 400)", isOneTwoOrThree);

        switch (*page) {
        case 1: *page = 100; break;
        case 2: *page = 200; break;
        case 3: *page = 400; break;
        }

        if ((*mem) % (*page) == 0) {
            break;
        }

        printf("ERROR: Memory size must be a multiple of the page!");
        printf(" %d is not a multiple of %d, please retry.\n", *mem, *page);
    }

    prompt_for_filename(file_path);
}

// get number of processes from file
int get_number_of_processes_from_file(FILE* filePtr) {
    int num = 0;

    fscanf(filePtr, "%d", &num);

    return num;
}

//stores the values of the processes into the process array
PROCESS* assignProcessList(const char* file_path) {
    int numSpace;
    int tmp;
    int counter = 0;
    int totalSpace = 0;
    FILE* filePtr = fopen(file_path, "r");

    number_of_procs = get_number_of_processes_from_file(filePtr);

    // allocate space for process array
    PROCESS* procList = malloc(number_of_procs * sizeof(PROCESS));

    if (!filePtr) {
        printf("ERROR: Failed to open file %s", file_path);
        exit(1);
    }

    while (!feof(filePtr) && counter < number_of_procs) {
        // store values for processes
        fscanf(filePtr, "%d %d %d %d",
               &(procList[counter].pid),
               &(procList[counter].arrivalTime),
               &(procList[counter].lifeTime),
               &numSpace);

        // get total memory requirements for process
        totalSpace = 0;
        for (int i = 0; i < numSpace; i++) {
            fscanf(filePtr, "%d", &tmp);
            totalSpace += tmp;
        }
	//assign values
        procList[counter].memReqs = totalSpace;
        procList[counter].isActive = 0;
        procList[counter].timeAddedToMemory = -1;
        procList[counter].timeFinished = -1;

        counter++;
    }

    fclose(filePtr);

    return procList;
}