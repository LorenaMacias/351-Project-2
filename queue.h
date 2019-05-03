//process queue
typedef struct proc_queue {
    int capacity;
    int size;
    int front;
    int rear;
    PROCESS **elements;
} proc_queue;

//creates the queue given the length
proc_queue* create_proc_queue(int length) {
    proc_queue *q;

    q = malloc(sizeof(proc_queue));

    q->elements = malloc(sizeof(PROCESS) * length);
    q->size = 0;
    q-> cap = length;
    q->front = 0;
    q->rear = -1;

    return q;
}

//add to the queue
void enqProcess(processQue* q, PROCESS* process) {
    if (q->size == q-> cap) {
        printf("ERROR: queue is full to cap!\n");
	exit(2);
    }

    q->size++;
    q->rear = q->rear + 1;

    if (q->rear == q-> cap) {
        q->rear = 0;
    }

    q->elements[q->rear] = proc;
}

//checks is the queue has somthing next to it given the process
int queue_has_next(proc_queue* q) {
    return q->size == 0 ? 0 : 1;
}

PROCESS* peek_queue(proc_queue* q) {
    if (!queue_has_next(q)) {
        printf("ERROR: queue is empty, can't get the next element\n");
        exit(2);
    }

    return q->elements[q->front];
}

PROCESS* peek_queue_at_index(proc_queue* q, int index) {
    return q->elements[index];
}

void dequeue_proc(proc_queue* q) {
    if (!queue_has_next(q)) {
        printf("ERROR: queue is empty, can't dequeue anything\n");
        exit(2);
    }

    if (q->front == q-> cap) {
        q->front = 0;
    }
}

//iterates through the queue
int iterate_queue_index(proc_queue* q, int index) {
    return q->front + index;
}

//remove the process from queue
void dequeue_proc_at_index(proc_queue* q, int index) {
    int i, prev = 0;

    for (i = 0; i < q->size; i += 1) {
        if (i > index) {
            q->elements[prev] = q->elements[i];
        }

        prev = i;
    }

    q->size--;
    q->rear--;
}

//print whole queue
void print_proc_queue(proc_queue* q) {
    int counter;

    //temp process
    PROCESS* proc;

    printf("\tInput queue: [");
    for (counter = 0; counter < q->size; counter += 1) {
        proc = peek_queue_at_index(q, iterate_queue_index(q, counter));

	if(counter != (q->size - 1)) { 
        printf("%d ", proc->pid);
	}
	else{
	printf("%d", proc->pid);
	printf("]\n");
	return;
	}
    }
    printf("]\n");
}

