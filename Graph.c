// The following code is adapted from Lab 3 of COMP2521 Summer Term 2024
// Implementation of Graph ADT using adjacency linked lists


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Map.h"

#define DEFAULT_CAPACITY 1 
struct adjLinks {
    int v;
    struct adjLinks *next;
};

struct graph {
    int nUrl;
    int nE;
    char **urls;
    Map urlToId;
    int capacity;
    struct adjLinks **adj;
};
static void printAdjList(struct adjLinks *l);
static void increaseCapacity(Graph g);
static int adjLinksSize(struct adjLinks *l);
static int urlToId(Graph g, char *url);
static struct adjLinks *newAdjLinks(int v);
static struct adjLinks *adjLinksInsert(struct adjLinks *l, int v);
static char *myStrdup(char *s);
static void freeAdjLinks(struct adjLinks *l);
static bool inAdjLink(struct adjLinks *l, int v);

// Implements a new graph

Graph GraphNew(void) {
    Graph graph = malloc(sizeof(*graph));
    if (graph == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    // Number of urls
    graph->nUrl = 0;
    
    graph->capacity = DEFAULT_CAPACITY;
    graph->urls = calloc(graph->capacity, sizeof(char *));
    if (graph->urls == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    graph->urlToId = MapNew();
    graph->adj = calloc(graph->capacity, sizeof(struct adjLinks *));
    if (graph->adj == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return graph;
}

// Free memory taken by graph
void GraphFree(Graph g) {
    for (int i = 0; i < g->capacity; i++) {
        freeAdjLinks(g->adj[i]);
    }
    free(g->adj);
    MapFree(g->urlToId);
    for (int i = 0; i < g->nUrl; i++) {
        free(g->urls[i]);
    }
    free(g->urls);
    
    free(g);
}
// Add url to graph
// Url will be contained in char **url
// ID will be mapped to url

bool GraphAddUrl(Graph g, char *s) {
     if (g->nUrl == g->capacity) {
        increaseCapacity(g);
    }
    if (!MapContains(g->urlToId, s)) {
        int id = g->nUrl++;
        g->urls[id] = myStrdup(s);
    
        MapSet(g->urlToId, s, id);
        return true;
    } else {
        return false;
    }
}


static void increaseCapacity(Graph g) {
    int newCapacity = g->capacity * 2;
    
    g->urls = realloc(g->urls, newCapacity * sizeof(char *));
    if (g->urls== NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = g->capacity; i < newCapacity; i++) {
        g->urls[i] = NULL;
    }
    
    g->adj = realloc(g->adj, newCapacity * sizeof(struct adjLinks));
    if (g->adj == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = g->capacity; i < newCapacity; i++) {
        g->adj[i] = NULL;
    }
    
    g->capacity = newCapacity;
}


int GraphNumUrl(Graph g) {
    return g->nUrl;
}

// Inserts links.
// Links are directional
bool GraphInsertLink(Graph g, char *s1, char *s2) {
    int id1 = urlToId(g, s1);
    int id2 = urlToId(g, s2);
    if (id1 == id2) {
        return false;
    }
    if (id1 == -1 || id2 == -1) {
        return false;
    }

    if (!inAdjLink(g->adj[id1], id2)) {
        g->adj[id1] = adjLinksInsert(g->adj[id1], id2);
        return true;
    } else {
        return false;
    }
}

bool GraphIsAdjacent(Graph g, char *s1, char *s2) {
    int id1 = MapGet(g->urlToId, s1);
    int id2 = MapGet(g->urlToId, s2);
    return inAdjLink(g->adj[id1], id2);
}

int GraphOutlinkSize(Graph g, char *s) {
    int id = MapGet(g->urlToId, s);
    return adjLinksSize(g->adj[id]);
}

void printOutAdjLinks(Graph g, char *s) {
    int id = MapGet(g->urlToId, s);
    printAdjList(g->adj[id]);
}
static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

static void freeAdjLinks(struct adjLinks *l) {
    struct adjLinks *n = l;
    while (n != NULL) {
        struct adjLinks *temp = n;
        n = n->next;
        free(temp);
    }
}


static bool inAdjLink(struct adjLinks *l, int v) {
    for (struct adjLinks *n = l; n != NULL; n = n->next) {
        if (n->v == v) {
            return true;
        }
    }
    return false;
}

static void printAdjList(struct adjLinks *l) {
    for (struct adjLinks *n = l; n != NULL; n = n->next) {
        printf(" %d", n->v);
    }
    printf("\n");
}
// Converts a name to an ID. Raises an error if the name doesn't exist.
static int urlToId(Graph g, char *url) {
    if (!MapContains(g->urlToId, url)) {
        return -1;
    }
    return MapGet(g->urlToId, url);
}

// Inserts a link in the adjacency list

static struct adjLinks *adjLinksInsert(struct adjLinks *l, int v) {
    if (l == NULL) {
        struct adjLinks *new = newAdjLinks(v);
        new->next = l;
        return new;
    } else if (v == l->v) {
        return l;
    } else {
        l->next = adjLinksInsert(l->next, v);
        return l;
    }
}

// Creates a new node for the adjacency list
static struct adjLinks *newAdjLinks(int v) {
    struct adjLinks *n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    n->v = v;
    n->next = NULL;
    return n;
}

// Recursive call finds size of an adjacency list
static int adjLinksSize(struct adjLinks *l) {
    if (l == NULL) {
        return 0;
    }
    return 1 + adjLinksSize(l->next);
}


