#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "../include/heap.h"

/* =========================================================
 * heap.c
 * Min-heap de perechi (node_id, dist) pentru Dijkstra.
 * Folosește un tablou dinamic cu indexarea standard
 * părinte/copil:
 *   parinte(i)       = (i - 1) / 2
 *   copil_stang(i)   = 2 * i + 1
 *   copil_drept(i)   = 2 * i + 2
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper intern de swap
 * ---------------------------------------------------------- */

static void swap(HeapNode *a, HeapNode *b) __attribute__((unused));
static void swap(HeapNode *a, HeapNode *b)
{
    /* TODO: interschimbă cele două elemente HeapNode */
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

/* ----------------------------------------------------------
 * Funcții de sift
 * ---------------------------------------------------------- */

static void sift_up(MinHeap *h, int i) __attribute__((unused));
static void sift_up(MinHeap *h, int i)
{
    /* TODO: mută elementul de la indexul i în sus până când
     *       proprietatea heap este respectată */
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[i].dist < h->data[parent].dist) {
            swap(&h->data[i], &h->data[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

static void sift_down(MinHeap *h, int i) __attribute__((unused));
static void sift_down(MinHeap *h, int i)
{
    /* TODO: mută elementul de la indexul i în jos până când
     *       proprietatea heap este respectată */
    int size = h->size;
    while (2 * i + 1 < size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (h->data[left].dist < h->data[smallest].dist) {
            smallest = left;
        }
        if (right < size && h->data[right].dist < h->data[smallest].dist) {
            smallest = right;
        }

        if (smallest != i) {
            swap(&h->data[i], &h->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

MinHeap *heap_create(int initial_capacity)
{
    /* TODO: alocă MinHeap și tabloul său de date */
    MinHeap *h = malloc(sizeof(MinHeap));
    if (!h) return NULL;
    h->size = 0;
    h->capacity = initial_capacity > 0 ? initial_capacity : 16;
    h->data = malloc(h->capacity * sizeof(HeapNode));
    if (!h->data) {
        free(h);
        return NULL;
    }
    return h;
}

void heap_free(MinHeap *h)
{
    /* TODO: eliberează tabloul de date, apoi structura */
    if (!h) return;
    free(h->data);
    free(h);
}

/* ----------------------------------------------------------
 * Operații de bază
 * ---------------------------------------------------------- */

int heap_push(MinHeap *h, int node_id, float dist)
{
    if (h->size == h->capacity) {
        h->capacity *= 2;
        HeapNode *new_data = realloc(h->data, h->capacity * sizeof(HeapNode));
        if (!new_data) return -1;
        h->data = new_data;
    }
    h->data[h->size].node_id = node_id;
    h->data[h->size].dist = dist;
    sift_up(h, h->size);
    h->size++;
    return 0;
}

HeapNode heap_pop(MinHeap *h)
{
    HeapNode result = {-1, FLT_MAX};
    if (h->size == 0) return result;
    
    result = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        sift_down(h, 0);
    }
    return result;
}

int heap_is_empty(const MinHeap *h)
{
    /* TODO: returnează 1 când size == 0 */
    return h->size == 0 ? 1 : 0;
}

int heap_decrease_key(MinHeap *h, int node_id, float new_dist)
{
    /* TODO: găsește node_id, actualizează dist dacă e mai mic,
     *       execută sift up */
    for (int i = 0; i < h->size; i++) {
        if (h->data[i].node_id == node_id) {
            if (new_dist < h->data[i].dist) {
                h->data[i].dist = new_dist;
                sift_up(h, i);
            }
            return 0; // A fost găsit
        }
    }
    return 1;
}
