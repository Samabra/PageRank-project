// Adapted from Lab 3 and Lab 4 of COMP2521 Summer Term 2024
// Implementation of Graph ADT using adjacency lists
#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct graph *Graph;


// Forms new graph
Graph   GraphNew();

// Free entire graph
void    GraphFree(Graph g);

// Add url to graph
bool    GraphAddUrl(Graph g, char *s);

// Returns number of urls
int     GraphNumUrl(Graph g);

// Inserts link into adjacency list of a url
// s2 will be placed in s1 url adjacency list
bool    GraphInsertLink(Graph g, char *s1, char *s2);

// Finds the size of the adjacency list for a url
int     GraphOutlinkSize(Graph g, char *s);

// Useful in debugging, prints out the adjacency list of a url
void    printOutAdjLinks(Graph g, char *s);
// Checks if two urls in graph are linked
// Checks if s2 is in adjacency list of s1.

bool    GraphIsAdjacent(Graph g, char *s1, char *s2);

#endif
