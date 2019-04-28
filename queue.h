//process queue
typedef struct process_queue {
    int capacity;
    int size;
    int front;
    int rear;
    PROCESS **elements;
} process_queue;

//creates the queue given the length
process_queue* create_process_queue(int length) {
    process_queue *q;

    q = malloc(sizeof(process_queue));

    q->elements = malloc(sizeof(PROCESS) * length);
    q->size = 0;
    q->capacity = length;
    q->front = 0;
    q->rear = -1;

    return q;
}

//add to the queue
void enqueue_process(process_queue* q, PROCESS* process) {
    if (q->size == q->capacity) {
        printf("ERROR: queue is full to capacity!\n");
	exit(2);
    }

    q->size++;
    q->rear = q->rear + 1;

    if (q->rear == q->capacity) {
        q->rear = 0;
    }
    q->elements[q->rear] = process;
}

//checks is the queue has somthing next to it given the process
int queue_has_next(process_queue* q) {
    return q->size == 0 ? 0 : 1;
}

PROCESS* peek_queue(process_queue* q) {
    if (!queue_has_next(q)) {
        printf("ERROR: queue is empty, can't get the next element\n");
        exit(2);
    }
    return q->elements[q->front];
}

PROCESS* peek_queue_at_index(process_queue* q, int index) {
    return q->elements[index];
}

void dequeue_process(process_queue* q) {
    if (!queue_has_next(q)) {
        printf("ERROR: queue is empty, can't dequeue anything\n");
        exit(2);
    }
   q->size--;
   q->front++;

    if (q->front == q->capacity) {
        q->front = 0;
    }
}

//iterates through the queue
int iterate_queue_index(process_queue* q, int index) {
    return q->front + index;
}

//remove the process from queue
void dequeue_process_at_index(process_queue* q, int index) {
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
void print_process_queue(process_queue* q) {
    int counter;

    //temp process
    PROCESS* process;

    printf("\tInput queue: [");
    for (counter = 0; counter < q->size; counter += 1) {
        process = peek_queue_at_index(q, iterate_queue_index(q, counter));

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

