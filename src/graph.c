#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/graph.h"

/* =========================================================
 * graph.c
 * Implementarea grafului direcționat ponderat cu liste de
 * adiacență.
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru relații
 * ---------------------------------------------------------- */

const char *relation_type_to_str(RelationType type)
{
    /* TODO: returnează șir cu litere mici pentru fiecare enum de relație */
    switch(type){
        case WORKS_AT: return "works_at";
        case FRIEND_OF: return "friend_of";
        case LOCATED_IN: return "located_in";
        case PARTICIPATES_IN: return "participates_in";
        default: return "unknown";
    }
}

RelationType str_to_relation_type(const char *str)
{
    /* TODO: parsează șirul relației la valoarea enum */
    if(strcmp(str, "works_at") == 0) return WORKS_AT;
    if(strcmp(str, "friend_of") == 0) return FRIEND_OF;
    if(strcmp(str, "located_in") == 0) return LOCATED_IN;
    if(strcmp(str, "participates_in") == 0) return PARTICIPATES_IN;
    return (RelationType)-1;
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Graph *graph_create(int initial_capacity)
{
    /* TODO: alocă structura Graph și tabloul de noduri */
    Graph *g = malloc(sizeof(Graph));
    if(!g)
        return NULL;
    g->size = 0;
    g->capacity = initial_capacity > 0 ? initial_capacity : 16;
    g->nodes = calloc(g->capacity, sizeof(GraphNode));
    if(!g->nodes){
        free(g);
        return NULL;
    }
    return g;
}

void graph_free(Graph *g)
{
    /* TODO: eliberează șirul de nume al fiecărui nod și lista de muchii,
     *       apoi tabloul și structura */
    if(!g)
        return;
    for(int i = 0; i < g->size; i++){
        free(g->nodes[i].entity.name);
        EdgeNode *curr = g->nodes[i].edges;
        while(curr){
            EdgeNode *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(g->nodes);
    free(g);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int graph_add_node(Graph *g, const char *name, EntityType type)
{
    /* TODO: extinde tabloul dacă e necesar, inițializează noul GraphNode,
     *       returnează id-ul */
    if (!g || !name) return -1;
    if (g->size == g->capacity) {
        g->capacity *= 2;
        GraphNode *new_nodes = realloc(g->nodes, g->capacity * sizeof(GraphNode));
        if (!new_nodes) return -1;
        g->nodes = new_nodes;
    }
    int id = g->size++;
    g->nodes[id].entity.name = strdup(name);
    g->nodes[id].entity.type = type;
    g->nodes[id].entity.id = id;
    g->nodes[id].edges = NULL;
    return id;
}

int graph_add_edge(Graph *g, int src_id, int dest_id,
                   RelationType type, float cost)
{
    /* TODO: alocă EdgeNode, adaugă la finalul listei de muchii a sursei
     *       pentru a păstra ordinea de inserare */
    if (!g || src_id < 0 || src_id >= g->size || dest_id < 0 || dest_id >= g->size) {
        return -1;
    }
    
    EdgeNode *new_edge = malloc(sizeof(EdgeNode));
    if (!new_edge) return -1;
    new_edge->dest_id = dest_id;
    new_edge->type = type;
    new_edge->cost = cost;
    new_edge->next = NULL;

    if (g->nodes[src_id].edges == NULL) {
        g->nodes[src_id].edges = new_edge;
    } else {
        EdgeNode *curr = g->nodes[src_id].edges;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_edge;
    }
    return 0;
}

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru interogare
 * ---------------------------------------------------------- */

int graph_find_id(const Graph *g, const char *name)
{
    /* TODO: scanare liniară returnând id-ul potrivit sau -1 */
    if (!g || !name) return -1;
    for (int i = 0; i < g->size; i++) {
        if (strcmp(g->nodes[i].entity.name, name) == 0) {
            return i;
        }
    }
    return -1;
}

GraphNode *graph_get_node(const Graph *g, int id)
{
    /* TODO: verifică limitele și returnează pointerul */
    if (!g || id < 0 || id >= g->size) return NULL;
    return &g->nodes[id];
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void graph_print(const Graph *g)
{
    /* TODO: afișează fiecare nod și lista sa de muchii */
    if (!g) return;
    for (int i = 0; i < g->size; i++) {
        printf("%d %s:", g->nodes[i].entity.id, g->nodes[i].entity.name);
        EdgeNode *curr = g->nodes[i].edges;
        while (curr) {
            printf(" [%d %s %.2f]", curr->dest_id, relation_type_to_str(curr->type), curr->cost);
            curr = curr->next;
        }
        printf("\n");
    }
}
