#ifndef _QUEUE_H
#define _QUEUE_H

// Define the structure for the queue node
typedef struct QueueNode
{
    int data;
    struct QueueNode *next;
} QueueNode;

// Define the queue structure
typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

// Function prototypes
Queue *init_queue();
int is_empty(Queue *queue);
void enqueue(Queue *queue, int data);
int dequeue(Queue *queue);
void destroy_queue(Queue *queue);

#endif /* _QUEUE_H */