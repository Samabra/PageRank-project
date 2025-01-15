// Code from Lab 3 COMP2521 Summer Term 2024
// Defines pageRank List ADT
#ifndef PAGELIST_H
#define PAGELIST_H

#include <stdbool.h>

typedef struct pageList *PageList;

struct pageList {
    struct cell *head;
    struct cell *tail;
    int size;
};
struct cell {
    char *s;
    double pageRank;
    struct cell *next;
};

// Creates a new empty list
// Complexity: O(1)
PageList PageListNew(void);

// Frees all memory allocated for the given list
// Complexity: O(n)
void PageListFree(PageList l);

// Adds a string to the end of the list. Makes a copy of the string
// before adding it to the list.
// Complexity: O(1)
void PageListAppend(PageList l, char *s, double pageRank);


// Sorts the list in desceding order of pageRank
// Average complexity: O(n log n)
void PageListSort(PageList l);

// Prints the list, one string per line
// If the strings themselves contain newlines, too bad
void PageListPrint(PageList l);

#endif
