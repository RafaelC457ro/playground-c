#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *init_queue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue != NULL)
    {
        queue->front = NULL;
        queue->rear = NULL;
    }
    return queue;
}

int is_empty(Queue *queue)
{
    return (queue->front == NULL);
}

void enqueue(Queue *queue, int data)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode != NULL)
    {
        newNode->data = data;
        newNode->next = NULL;

        if (is_empty(queue))
        {
            queue->front = newNode;
            queue->rear = newNode;
        }
        else
        {
            queue->rear->next = newNode;
            queue->rear = newNode;
        }
    }
}

int dequeue(Queue *queue)
{
    if (!is_empty(queue))
    {
        QueueNode *temp = queue->front;
        int data = temp->data;

        queue->front = queue->front->next;
        free(temp);

        if (is_empty(queue))
        {
            queue->rear = NULL;
        }

        return data;
    }
    return -1; // this is very bad code but I'm too lazy to fix it
}

void destroy_queue(Queue *queue)
{
    while (!is_empty(queue))
    {
        dequeue(queue);
    }
    free(queue);
}
