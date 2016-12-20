#ifndef QUEUE_H
#define QUEUE_H

#include<stdio.h>
#include<stdlib.h>

#include "list.h"

/* by default, QueueElement is int
 Usage: #define QueueElement <TYPE> */
#ifndef QueueElement
#define QueueElement void*
#endif


#define QUEUE_IS_FULL		0x01
#define QUEUE_IS_EMPTY		0x02

typedef struct{
    int capacity;
    int size;
    QueueElement e;
    struct list_head list;
} Queue;

Queue * initQueue(int max);
QueueElement front(Queue *Q);	
QueueElement tail(Queue* Q);
int dequeue(Queue *Q);
int enqueue(Queue *Q, QueueElement element);

#define QUEUE_CAPACITY(queue)  ((queue)->capacity)
#define QUEUE_SIZE(queue)  ((queue)->size)

#endif	/*QUEUE_H*/
