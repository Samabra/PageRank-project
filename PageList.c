// Code from Lab 3 COMP2521 Summer Term 2024
// Implementation of PageList ADT


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PageList.h"

typedef struct {
    char *s;
    double pageRank;
} UrlRankPair;

static struct cell *newCell(char *s, double pageRank);
static char *myStrdup(char *s);
static int qsortPageRankUrlCmp(const void *ptr1, const void *ptr2);

// Creates a new empty list
PageList PageListNew(void) {
    PageList l = malloc(sizeof(*l));
    if (l == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

// Frees all memory allocated for the given list
void PageListFree(PageList l) {
    struct cell *curr = l->head;
    while (curr != NULL) {
        struct cell *temp = curr;
        curr = curr->next;
        free(temp->s);
        free(temp);
    }
    free(l);
}

// Adds a string to the end of the list
void PageListAppend(PageList l, char *s, double pageRank) {
    struct cell *n = newCell(s, pageRank);
    if (l->head == NULL) {
        l->head = n;
    } else {
        l->tail->next = n;
    }
    l->tail = n;
    l->size++;
}

static struct cell *newCell(char *s, double pageRank) {
    struct cell *n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    n->s = myStrdup(s);
    n->pageRank = pageRank;
    n->next = NULL;
    return n;
}

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Sorts the list in descending order of pageRank 
// Uses quicksort functionality
void PageListSort(PageList l) {
    UrlRankPair *items = malloc(l->size * sizeof(UrlRankPair));
    if (items == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    for (struct cell *curr = l->head; curr != NULL; curr = curr->next) {
        items[i].s = curr->s;
        items[i].pageRank = curr->pageRank;
        i++;
    }
    qsort(items, l->size, sizeof(UrlRankPair), qsortPageRankUrlCmp);
    i = 0;
    for (struct cell *curr = l->head; curr != NULL; curr = curr->next) {
        curr->s = items[i].s;
        curr->pageRank = items[i].pageRank;
        i++;
    }
    free(items);
}

static int qsortPageRankUrlCmp(const void *ptr1, const void *ptr2) {
    double rank1 = ((UrlRankPair *)ptr1)->pageRank;
    double rank2 = ((UrlRankPair *)ptr2)->pageRank;

    if (rank1 < rank2) {
        return 1;
    }
    if (rank1 > rank2) {
        return -1;
    }
    return 0;
}
