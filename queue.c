struct cell {
    struct cell *next; // next node in a linked list
    int x;
    int y;
    int z;
    int value;
}

struct queue { // struct for a queue
    struct cell *head; 
    struct cell *tail;
};

struct cell *create_element(int x, int y, int z, int value) {
    struct cell *new;
    new->x = x;
    new->y = y;
    new->z = z;
    new->value = value;
    return new;
}

/*
   Initializes an empty queue by taking the address
   of an existing queue and setting the head and tail
   pointers to NULL
 */
void initqueue(struct queue *q) {
    q->head = NULL;
    q->tail = NULL;
}

/*
   Enqueues the value 'time' into the queue given. It inserts
   the value at the end of the linked list (the tail)
*/
int enqueue(struct queue *q, struct cell *c) {
    
    struct cell *new; // pointer to new element
    new = (struct cell *)malloc(sizeof(struct cell));
    new->x = c->x; // set the value of the new element
    new->y = c->y;
    new->z = c->z;
    new->value = c->value;
    
    // if the queue is not empty, set the current last element 
    // to the new element
    if (q->tail != NULL) { 
        q->tail->next = new;
    }

    q->tail = new; // the new element becomes the tail
    
    // if the queue is empty, set the head to the new element
    if (q->head == NULL) {
        q->head = q->tail;
    }

    q->tail->next = NULL;
    
    return 0;
}

/*
   Dequeues a value from the front of the linked list (the head)
   and sets the head to the next node in the linked list
*/
struct cell *dequeue(struct queue *q) {
    
    struct cell *ret; // return value

    struct cell *to_remove; // the element we will remove
    to_remove = q->head; // we will remove the first element in the queue
    
    // if the queue is empty, exit
    if (q->head == NULL) {
        exit(EXIT_FAILURE);
    }

    ret->x = to_remove->x; // save the return value
    ret->y = to_remove->y;
    ret->z = to_remove->z;
    ret->value = to_remove->value;

    // if there is only one element in the queue, mark the
    // queue as empty
    if (q->head == q->tail) {
        q->head = NULL;
        q->tail = NULL;
    } 
    else {
        q->head = to_remove->next; // the next element becomes the first
    }

    free(to_remove); // free memory
    return ret; // return the value
}

int isempty(struct queue *q) {
    return (q->head == NULL);
}