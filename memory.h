
typedef struct FRAME {
    int asi;
    char loc[40];
    int procAsi;
    int pageNum;
} FRAME;

typedef struct frame_list {
    FRAME* frames;
    int numFrames;
    int pageSize;
} frame_list;

frame_list* createFrameList(int numFrames, int pageSize) {
    int i;

    frame_list *f;

    f = malloc(sizeof(frame_list));

    f->frames = (FRAME*)malloc(sizeof(FRAME) * numFrames);
    f->pageSize = pageSize;
    f->numFrames = numFrames;

    for (i = 0; i < f->numFrames; i += 1) {
        f->frames[i].asi = 0;
    }

    return f;
}

int processIntoMem(frame_list* list, PROCESS* process) {
    int i, num_free_frames = 0;

    for (i = 0; i < list->numFrames; i += 1) {
        if (!list->frames[i].asi) {
            num_free_frames += 1;
        }
    }

    //checks if the number of free frames times the page size is greater than the mem_req for the   	given process
    return (num_free_frames * list->pageSize) >= process->mem_reqs;
}

void fitIntoMem(frame_list* list, PROCESS* process) {
    //assuming you already checked that you are able to fit the proc into mem 
    int i, remaining_mem, current_page = 1;

    remaining_mem = process->mem_reqs;

    for (i = 0; i < list->numFrames; i += 1) {
        // if this frame is not asi
        if (!list->frames[i].asi) {
            // assign it
            list->frames[i].asi = 1;
            // set the page number
            list->frames[i].pageNum = current_page;
            // set the proc num
            list->frames[i].procAsi = process->pid;

            current_page++;
            remaining_mem -= list->pageSize;
        }

        if (remaining_mem <= 0) {
            break;
        }
    }
}

//this function is to print the frame list
void printFrameList(frame_list* list) {
    int i, in_free_block = 0, start;

    printf("\tMemory map: ");
    //print the page numbers by list
    for (i = 0; i < list->numFrames; i += 1) {
	
        if (!in_free_block && !list->frames[i].asi) {
            in_free_block = 1;
            start = i;
        } 
	else if (in_free_block && list->frames[i].asi) {
            in_free_block = 0;
           printf("\t\t%d-%d: Free frame(s)\n",
                   start * list->pageSize,
                   (i * list->pageSize) - 1);
        }

	//added an if statement so the first print is on same line as the title
	if (list->frames[i].asi && i == 0) {
            printf("%d-%d: Process %d, Page %d\n",
                   i * list->pageSize,
                   ((i + 1) * list->pageSize) - 1,
                  list->frames[i].procAsi,
                  list->frames[i].pageNum);
        }
	//else it prints on a new line and it tabs the pages
        else if (list->frames[i].asi) {
            printf("\t\t%d-%d: Process %d, Page %d\n",
                   i * list->pageSize,
                   ((i + 1) * list->pageSize) - 1,
                  list->frames[i].procAsi,
                  list->frames[i].pageNum);
        }
    }//end of for loop
   //this if statement is to print the free frames but starting at the same line as title
   if (in_free_block && i == 0) {
        printf("%d-%d: Free frame(s)\n",
               start * list->pageSize,
               ((i) * list->pageSize) - 1);
    }
   //this prints the frame numbers for each list/pageSize
   else if (in_free_block) {
        printf("\t\t%d-%d: Free frame(s)\n",
               start * list->pageSize,
               ((i) * list->pageSize) - 1);
    }
}

//prints info for the enxt processes that arrive
int frameListIsEmpty(frame_list* list) {
    int i;
    for (i = 0; i < list->numFrames; i += 1) {
        if (list->frames[i].asi) {
            return 0;
        }
    }
    return 1;
}

//frees memory and prevents deadlocks
void freeMemForPid(frame_list* list, int pid) {
    int i;
    FRAME* frame;

    for (i = 0; i < list->numFrames; i += 1) {
       frame = &list->frames[i];

        if (frame->procAsi == pid) {
            frame->procAsi = 0;
            frame->pageNum = 0;
            frame->asi = 0;
        }
    }
}

