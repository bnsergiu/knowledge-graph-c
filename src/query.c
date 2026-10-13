#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "../include/query.h"
#include "../include/heap.h"

/* =========================================================
 * query.c
 * Parsarea și dispatcharea interogărilor de graf:
 *   EXISTS, EDGE, NEIGHBORS, PATH (BFS), DIJKSTRA
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper de parsare
 * ---------------------------------------------------------- */

QueryType parse_query_type(const char *line)
{
    /* TODO: potrivește cuvântul cheie de la începutul liniei */
    if (strncmp(line, "EXISTS", 6) == 0) return Q_EXISTS;
    if (strncmp(line, "EDGE", 4) == 0) return Q_EDGE;
    if (strncmp(line, "NEIGHBORS", 9) == 0) return Q_NEIGHBORS;
    if (strncmp(line, "PATH", 4) == 0) return Q_PATH;
    if (strncmp(line, "DIJKSTRA", 8) == 0) return Q_DIJKSTRA;
    return Q_UNKNOWN;
}

/* ----------------------------------------------------------
 * EXISTS
 * ---------------------------------------------------------- */

void process_exists(const BST *tree, const char *name)
{
    /* TODO: caută în BST, afișează DA/NU */
    if (!tree || !name) return;
    GraphNode *node = bst_search(tree, name);
    printf("EXISTS %s: %s\n", name, node ? "DA" : "NU");
}

/* ----------------------------------------------------------
 * EDGE
 * ---------------------------------------------------------- */

void process_edge(const Graph *g, const BST *tree,
                  const char *src_name, const char *dest_name)
{
    /* TODO: găsește ambele noduri via BST, scanează lista de muchii
     *       a sursei pentru destinație */
    if (!g || !tree || !src_name || !dest_name) return;
    
    GraphNode *src = bst_search(tree, src_name);
    GraphNode *dest = bst_search(tree, dest_name);

    if (!src || !dest) {
        printf("EDGE %s %s: NU\n", src_name, dest_name);
        return;
    }

    EdgeNode *curr = src->edges;
    while (curr) {
        if (curr->dest_id == dest->entity.id) {
            printf("EDGE %s %s: DA\n", src_name, dest_name);
            return;
        }
        curr = curr->next;
    }
    printf("EDGE %s %s: NU\n", src_name, dest_name);
}

/* ----------------------------------------------------------
 * NEIGHBORS
 * ---------------------------------------------------------- */

void process_neighbors(const Graph *g, const BST *tree,
                       const char *name)
{
    /* TODO: găsește nodul, iterează lista de muchii, afișează
     *       numele vecinilor; dacă nu există muchii de ieșire
     *       afișează "NEIGHBORS <name>: NULL" */
    if (!g || !tree || !name) return;
    GraphNode *node = bst_search(tree, name);
    if (!node || !node->edges) {
        printf("NEIGHBORS %s: NULL\n", name);
        return;
    }
    printf("NEIGHBORS %s:", name);
    EdgeNode *curr = node->edges;
    while (curr) {
        GraphNode *neighbor = graph_get_node(g, curr->dest_id);
        if (neighbor) {
            printf(" %s", neighbor->entity.name);
        }
        curr = curr->next;
    }
    printf("\n");
}

/* ----------------------------------------------------------
 * PATH (BFS)
 * ---------------------------------------------------------- */
static void print_graph_path(const char *cmd, const char *src, const char *dest,
                             int dest_id, int *parent, const Graph *g, float dist) {
    int *path = malloc(g->size * sizeof(int));
    int len = 0, curr = dest_id;
    while (curr != -1) {
        path[len++] = curr;
        curr = parent[curr];
    }
    
    if (dist < 0) {
        printf("%s %s %s:", cmd, src, dest);
    } else {
        printf("%s %s %s: COST = %.2f; DRUM =", cmd, src, dest, dist);
    }
    
    for (int i = len - 1; i >= 0; i--) {
        if (i == len - 1) {
            printf(" %s", g->nodes[path[i]].entity.name);
        } else {
            printf(" -> %s", g->nodes[path[i]].entity.name);
        }
    }
    printf("\n");
    free(path);
}

void process_path_bfs(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: BFS de la src la dest, reconstruiește și afișează calea;
     *       dacă nu există cale afișează "PATH <src> <dest>: NU" */
    GraphNode *src = bst_search(tree, src_name), *dest = bst_search(tree, dest_name);
    if (!src || !dest) { printf("PATH %s %s: NU\n", src_name, dest_name); return; }

    int n = g->size, *vis = calloc(n, sizeof(int)), *parent = malloc(n * sizeof(int));
    int *queue = malloc(n * sizeof(int)), head = 0, tail = 0, found = 0;
    for (int i = 0; i < n; i++) parent[i] = -1;

    queue[tail++] = src->entity.id;
    vis[src->entity.id] = 1;

    while (head < tail) {
        int u = queue[head++];
        if (u == dest->entity.id) { found = 1; break; }
        for (EdgeNode *e = g->nodes[u].edges; e; e = e->next) {
            if (!vis[e->dest_id]) {
                vis[e->dest_id] = 1;
                parent[e->dest_id] = u;
                queue[tail++] = e->dest_id;
            }
        }
    }

    if (found) print_graph_path("PATH", src_name, dest_name, dest->entity.id, parent, g, -1.0f);
    else printf("PATH %s %s: NU\n", src_name, dest_name);

    free(vis); free(parent); free(queue);
}

/* ----------------------------------------------------------
 * DIJKSTRA
 * ---------------------------------------------------------- */

void process_dijkstra(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: Dijkstra cu min-heap, afișează costul și calea;
     *       dacă nu există cale afișează "DIJKSTRA <src> <dest>: NU" */
    GraphNode *src = bst_search(tree, src_name), *dest = bst_search(tree, dest_name);
    if (!src || !dest) { printf("DIJKSTRA %s %s: NU\n", src_name, dest_name); return; }

    int n = g->size, *parent = malloc(n * sizeof(int));
    float *dist = malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) { dist[i] = FLT_MAX; parent[i] = -1; }

    MinHeap *h = heap_create(n);
    dist[src->entity.id] = 0;
    heap_push(h, src->entity.id, 0.0f);

    while (!heap_is_empty(h)) {
        HeapNode u = heap_pop(h);
        if (u.dist > dist[u.node_id]) continue;
        if (u.node_id == dest->entity.id) break;

        for (EdgeNode *e = g->nodes[u.node_id].edges; e; e = e->next) {
            float new_d = dist[u.node_id] + e->cost;
            if (new_d < dist[e->dest_id]) {
                dist[e->dest_id] = new_d; parent[e->dest_id] = u.node_id;
                if (heap_decrease_key(h, e->dest_id, new_d) == 1) heap_push(h, e->dest_id, new_d);
            }
        }
    }

    if (dist[dest->entity.id] != FLT_MAX) {
        print_graph_path("DIJKSTRA", src_name, dest_name, dest->entity.id, parent, g, dist[dest->entity.id]);
    } else {
        printf("DIJKSTRA %s %s: NU\n", src_name, dest_name);
    }

    heap_free(h); free(dist); free(parent);
}

/* ----------------------------------------------------------
 * Dispatcher batch
 * ---------------------------------------------------------- */

void process_all_queries(Queue *q, const Graph *g, const BST *tree)
{
    /* TODO: extrage fiecare linie, parsează tipul, dispatchează
     *       la handlerul corespunzător */
    if (!q || !g || !tree) return;
    while (!queue_is_empty(q)) {
        char *line = queue_dequeue(q);
        if (!line) continue;
        char *line_copy = strdup(line);
        QueryType type = parse_query_type(line_copy);
        char *cmd = strtok(line_copy, " \r\n");
        if (!cmd) {
            free(line_copy);
            free(line);
            continue;
        }
        if (type == Q_EXISTS) {
            char *name = strtok(NULL, " \r\n");
            if (name) process_exists(tree, name);
        } else if (type == Q_EDGE) {
            char *src = strtok(NULL, " \r\n");
            char *dest = strtok(NULL, " \r\n");
            if (src && dest) process_edge(g, tree, src, dest);
        } else if (type == Q_NEIGHBORS) {
            char *name = strtok(NULL, " \r\n");
            if (name) process_neighbors(g, tree, name);
        } else if (type == Q_PATH) {
            char *src = strtok(NULL, " \r\n");
            char *dest = strtok(NULL, " \r\n");
            if (src && dest) process_path_bfs(g, tree, src, dest);
        } else if (type == Q_DIJKSTRA) {
            char *src = strtok(NULL, " \r\n");
            char *dest = strtok(NULL, " \r\n");
            if (src && dest) process_dijkstra(g, tree, src, dest);
        }

        free(line_copy);
        free(line);
    }
}
