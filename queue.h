//process queue
typedef struct processQue {
    int cap;
    int size;
    int front;
    int rear;
    PROCESS **elements;
} processQue;

//creates the queue given the length
processQue* createProcessQue(int length) {
    processQue *q;

    q = malloc(sizeof(processQue));

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
    q->elements[q->rear] = process;
}

//checks is the queue has somthing next to it given the process
int queNext(processQue* q) {
    return q->size == 0 ? 0 : 1;
}

PROCESS* peek_queue(processQue* q) {
    if (!queNext(q)) {
        printf("ERROR: queue is empty, can't get the next element\n");
        exit(2);
    }
    return q->elements[q->front];
}

PROCESS* peek_queue_at_index(processQue* q, int index) {
    return q->elements[index];
}

void dequeProcess(processQue* q) {
    if (!queNext(q)) {
        printf("ERROR: queue is empty, can't dequeue anything\n");
        exit(2);
    }
   q->size--;
   q->front++;

    if (q->front == q-> cap) {
        q->front = 0;
    }
}

//iterates through the queue
int iterQueIndex(processQue* q, int index) {
    return q->front + index;
}

//remove the process from queue
void dequeProcessAtIndex(processQue* q, int index) {
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
void printProcessQue(processQue* q) {
    int counter;

    //temp process
    PROCESS* process;

    printf("\tInput queue: [");
    for (counter = 0; counter < q->size; counter += 1) {
        process = peek_queue_at_index(q, iterQueIndex(q, counter));

	if(counter != (q->size - 1)) { 
        printf("%d ", process->pid);
	}
	else{
	printf("%d", process->pid);
	printf("]\n");
	return;
	}
    }
    printf("]\n");
}

