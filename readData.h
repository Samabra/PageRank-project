
#ifndef READ_DATA_H
#define READ_DATA_H

#include "List.h"
#include "Graph.h"
#include "bst.h"


// Get list of all urls in collection.txt
List getCollection(List url);


// Get a graph of urls, with all outgoing links
Graph GetGraph(List url, Graph web);

// Returns inverted list, with bst and linked list implementation
struct object *invertedList(List url, struct object *t);

#endif