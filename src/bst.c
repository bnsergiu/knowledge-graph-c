#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bst.h"

/* =========================================================
 * bst.c
 * BST indexat după numele entității; nodurile conțin pointeri
 * în tabloul de noduri al grafului (fără duplicarea datelor).
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții auxiliare interne (statice, neexportate)
 * ---------------------------------------------------------- */

/* Eliberează recursiv nodii BSTNode (nu GraphNode-urile spre care
 * pointează — acelea aparțin Grafului). */
static void bst_free_recursive(BSTNode *node)
    __attribute__((unused));
static void bst_free_recursive(BSTNode *node)
{
    /* TODO: traversare post-ordine pentru eliberarea fiecărui BSTNode */
    if (!node) return;
    bst_free_recursive(node->left);
    bst_free_recursive(node->right);
    free(node);
}

/* Inserare recursivă; returnează rădăcina (posibil nouă) a subarbore. */
static BSTNode *bst_insert_recursive(BSTNode *node, GraphNode *gn)
    __attribute__((unused));
static BSTNode *bst_insert_recursive(BSTNode *node, GraphNode *gn)
{
   if (!node) {
        BSTNode *new_node = malloc(sizeof(BSTNode));
        if (!new_node) return NULL;
        new_node->graph_node = gn;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }
    
    int cmp = strcmp(gn->entity.name, node->graph_node->entity.name);
    if (cmp < 0) {
        node->left = bst_insert_recursive(node->left, gn);
    } else if (cmp > 0) {
        node->right = bst_insert_recursive(node->right, gn);
    }
    
    return node;
}

/* Căutare recursivă; returnează GraphNode* sau NULL. */
static GraphNode *bst_search_recursive(const BSTNode *node, const char *name)
    __attribute__((unused));
static GraphNode *bst_search_recursive(const BSTNode *node, const char *name)
{
    /* TODO: compară name, recursie stânga/dreapta */
    if (!node) return NULL;
    int cmp = strcmp(name, node->graph_node->entity.name);
    if (cmp == 0) return node->graph_node;
    if (cmp < 0) return bst_search_recursive(node->left, name);
    return bst_search_recursive(node->right, name);
}

/* Traversare inordine pentru afișare. */
static void bst_inorder_recursive(const BSTNode *node)
    __attribute__((unused));
static void bst_inorder_recursive(const BSTNode *node)
{
    /* TODO: stânga -> vizitare -> dreapta */
    if (!node) return;
    bst_inorder_recursive(node->left);
    entity_print(&node->graph_node->entity);
    bst_inorder_recursive(node->right);
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

/* Alocă și inițializează un BST gol. Returnează NULL la eșec. */
BST *bst_create(void)
{
    /* TODO: alocă structura BST și inițializează root-ul cu NULL */
    BST *tree = malloc(sizeof(BST));
    if (!tree) return NULL;
    tree->root = NULL;
    return tree;
}

/* Eliberează recursiv toți nodii BST, apoi structura BST. */
void bst_free(BST *tree)
{
    /* TODO: traversare post-ordine pentru eliberarea memoriei */
    if (!tree) return;
    bst_free_recursive(tree->root);
    free(tree);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int bst_insert(BST *tree, GraphNode *graph_node)
{
    /* TODO: deleghează la helper-ul recursiv, actualizează tree->root */
    if (!tree || !graph_node) return -1;
    BSTNode *new_root = bst_insert_recursive(tree->root, graph_node);
    if (!new_root && tree->root == NULL) return -1;
    tree->root = new_root;
    return 0;
}

/* ----------------------------------------------------------
 * Interogare
 * ---------------------------------------------------------- */

GraphNode *bst_search(const BST *tree, const char *name)
{
    /* TODO: deleghează la helper-ul recursiv */
    if (!tree || !name) return NULL;
    return bst_search_recursive(tree->root, name);
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void bst_inorder_print(const BST *tree)
{
    if (!tree) return;
    bst_inorder_recursive(tree->root);
    (void)tree;
}
