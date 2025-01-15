#include <stdlib.h>
#include <stdio.h>
#include "List.h"
#include "bst.h"
#include "readData.h"

static void invertedIndex(struct object *index, FILE *file);
int main(void) {
    List urls = ListNew();
    urls = getCollection(urls);
    struct object *index = bstNew();
    index = invertedList(urls, index);
    FILE *file = fopen("invertedIndex.txt", "w");
    invertedIndex(index, file);
    ListFree(urls);
    bstFree(index);
    fclose(file);
    return 0;
}

// Recursive function recursively prints into file word index
// in alphabetical order
static void invertedIndex(struct object *index, FILE *file) {
    if (index == NULL) {
        return;
    }
    invertedIndex(index->left, file);
    fprintf(file, "%s", index->key);
    ListIterator it = ListItNew(index->l);

    // List iterator to iterate through list
    while (ListItHasNext(it)) {
        char *url = ListItNext(it);
        fprintf(file, " %s", url);
    }
    ListItFree(it);
    fprintf(file, "\n");
    invertedIndex(index->right, file);
}