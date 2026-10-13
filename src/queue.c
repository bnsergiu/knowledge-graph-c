#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/queue.h"

/* =========================================================
 * queue.c
 * Coadă FIFO susținută de o listă simplu înlănțuită.
 * Stochează copii alocate pe heap ale șirurilor.
 * ========================================================= */

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Queue *queue_create(void)
{
    /* TODO: alocă și inițializează cu zero o structură Queue */
    Queue *q = malloc(sizeof(Queue));
    if (!q) return NULL;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void queue_free(Queue *q)
{
    /* TODO: golește coada eliberând fiecare nod și datele sale */
    if (!q) return;
    while (!queue_is_empty(q)) {
        char *data = queue_dequeue(q);
        free(data);
    }
    free(q);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int queue_enqueue(Queue *q, const char *data)
{
    /* TODO: duplică data, alocă QueueNode, adaugă la capătul rear */
    if (!q || !data) return -1;
    
    QueueNode *node = malloc(sizeof(QueueNode));
    if (!node) return -1;
    node->data = strdup(data);
    node->next = NULL;
    
    if (queue_is_empty(q)) {
        q->front = node;
        q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;
    return 0;
}

char *queue_dequeue(Queue *q)
{
    /* TODO: elimină nodul din față, returnează datele sale (apelantul
     *       eliberează) */
    if (!q || queue_is_empty(q)) return NULL;
    
    QueueNode *tmp = q->front;
    char *data = tmp->data;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(tmp);
    q->size--;
    return data;
}

/* ----------------------------------------------------------
 * Inspecție
 * ---------------------------------------------------------- */

int queue_is_empty(const Queue *q)
{
    /* TODO: returnează 1 când size == 0 */
    return (!q || q->size == 0) ? 1 : 0;
}

void queue_print(const Queue *q)
{
    /* TODO: iterează de la front la rear, afișează fiecare șir de date */
    if (!q) return;
    QueueNode *curr = q->front;
    while (curr) {
        printf("%s\n", curr->data);
        curr = curr->next;
    }
}
