// Code adapted from Lab 2 COMP2521 Summer Term 2024 
// Implementation of Binary search tree for 

#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdio.h>
#include "List.h"


struct object {
	char *key;
	List l;
	struct object *left;
	struct object *right;
};

////////////////////////////////////////////////////////////////////////

// Creates a new empty BST
struct object *bstNew(void);

// Frees all the memory associated with the given BST
void bstFree(struct object *t);

// Inserts a new value into the given BST and returns the root of the
// updated BST. Does not insert duplicate values.
struct object *bstInsert(struct object *t, char *key, char *url);

// Finds if a word exists in index, and finds if url associated
// with word exists in the word's url list
bool bstFind(struct object *t, char *key);


////////////////////////////////////////////////////////////////////////


#endif

