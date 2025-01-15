// Code from Lab 2 COMP2521 Summer Term 2024
// Implementation of BST with linked lists

//#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "bst.h"
#include "List.h"

////////////////////////////////////////////////////////////////////////

static struct object *newObject(char *key, char *url);
//static void showBstNode(struct node *t);
//static void doBstPrint(struct node *t, FILE *out);
static char *myStrdup(char *s);
static void insertDuplicate(struct object *t, char *url);

////////////////////////////////////////////////////////////////////////

// Creates a new empty bst
struct object *bstNew(void) {
	return NULL;
}

// Frees all the memory associated with the given bst
void bstFree(struct object *t) {
	if (t == NULL) {
		return;
	}

	bstFree(t->left);
	bstFree(t->right);
	ListFree(t->l);
	free(t->key);
	free(t);
}

// Inserts a new value into the given BST and returns the root of the
// updated bst. Does not insert duplicate values.
struct object *bstInsert(struct object *t, char *key, char *url) {
	if (t == NULL) {
		return newObject(key, url);
	} else if (strcmp(key, t->key) < 0) {
		t->left = bstInsert(t->left, key, url);
	} else if (strcmp(key, t->key) > 0) {
		t->right = bstInsert(t->right, key, url);
	} else {
		insertDuplicate(t, url);
	}
	return t;
}

// If word exists in index, but the url is different
// append url to list in BST
static void insertDuplicate(struct object *t, char *url) {
	if (!bstFind(t, url)) {
		ListAppend(t->l, url);
	}
}
// Creates a new node containing the given value
static struct object *newObject(char *key, char *url) {
	struct object *new = malloc(sizeof(*new));
	if (new == NULL) {
		err(EX_OSERR, "couldn't allocate BST node");
	}

	new->key = myStrdup(key);
	new->l = ListNew();
	ListAppend(new->l, url);
	new->left = NULL;
	new->right = NULL;
	return new;
}

// Checks whether a value is in the given bst
bool bstFind(struct object *t, char *key) {
	if (!ListFind(t->l, key)) {
		return false;
	}
	else {
		return true;
	}
}


static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}