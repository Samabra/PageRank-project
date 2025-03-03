
// Code from Lab 3 COMP2521 Summer Term 2024
// Defines List ADT
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct list *List;

struct node {
    char *s;
    struct node *next;
};

// Creates a new empty list
// Complexity: O(1)
List ListNew(void);

// Frees all memory allocated for the given list
// Complexity: O(n)
void ListFree(List l);

// Adds a string to the end of the list. Makes a copy of the string
// before adding it to the list.
// Complexity: O(1)
void ListAppend(List l, char *s);

// Returns the number of items in the list
// Complexity: O(1)
int  ListSize(List l);

// Sorts the list in ASCII order
// Average complexity: O(n log n)
void ListSort(List l);

// Prints the list, one string per line
// If the strings themselves contain newlines, too bad
void ListPrint(List l);

bool ListFind(List l, char *url);

typedef struct listIterator *ListIterator;

// Creates an iterator for the given list
// Complexity: O(1)
ListIterator ListItNew(List l);

// Gets the next item in the list. The item should not be modified.
// Complexity: O(1)
char *ListItNext(ListIterator it);

// Checks if the list has a next item
// Complexity: O(1)
bool ListItHasNext(ListIterator it);

// Frees the given iterator
// Complexity: O(1)
void ListItFree(ListIterator it);

#endif