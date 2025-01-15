#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "Graph.h"
#include "List.h"
#include "readData.h"
#include "PageList.h"

static void pageRank(double damping, double diffPR, int iterations);
static double calculatePageRank(PageList ranks, char *url, Graph web,
                                double damping, int nUrl);
int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Not enough arguments\n");
        return EXIT_FAILURE;
    }
    double damping = atof(argv[1]);
    double diffPR = atof(argv[2]);

    int iterations = atoi(argv[3]);
    pageRank(damping, diffPR, iterations);
    return 0;

}
// Void function to find pageRank
static void pageRank(double damping, double diffPR, int iterations) {
    List url = ListNew();
    url = getCollection(url);
    Graph web = GraphNew();
    web = GetGraph(url, web);
    PageList ranks = PageListNew();
    int nUrl = GraphNumUrl(web);

    // Array to hold all newer pageRank values
    double pR[ListSize(url)];
    double pageRank = 1.0 / nUrl;
    ListIterator it = ListItNew(url);
    int j = 0;
    while (ListItHasNext(it)) {
        char *url = ListItNext(it);

        // Appending to a list ADT which contains pageRank of each url
        PageListAppend(ranks, url, pageRank);

        // Setting the array to hold initial pageRank values
        pR[j] = pageRank;
        j++;
    }
    ListItFree(it);
    double diff = diffPR;
    int i = 0;
    while (i < iterations && diff >= diffPR) {
        int t = 0;
        for (struct cell *p = ranks->head; p != NULL; p = p->next) {
            // Array will hold pageRank values of each url first
            // pageRank caluclations done with the use of a helper function
            pR[t] = calculatePageRank(ranks, p->s, web, damping, nUrl);
            t++;
        }
        double diffSum = 0.0;
        int k = 0;
        for (struct cell *o = ranks->head; o != NULL; o = o->next) {
            // Find the difference between new value of pageRank after iteration previous value
            diffSum = diffSum + fabs(pR[k] - o->pageRank);
            o->pageRank = pR[k];
            k++;
        }
        diff = diffSum;
        i++;
    }
    FILE *file = fopen("pagerankList.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        return;
    }
    PageListSort(ranks);
    for (struct cell *j = ranks->head; j != NULL; j = j->next) {
        fprintf(file, "%s, %d, %.7lf\n", j->s, GraphOutlinkSize(web, j->s), j->pageRank);
    }
    fclose(file);
    ListFree(url);
    GraphFree(web);
    PageListFree(ranks);
}

static double calculatePageRank(PageList ranks, char *url, Graph web, 
                                double damping, int nUrl) {
    double pR = 0;
    for (struct cell *n = ranks->head; n != NULL; n = n->next) {
        if (GraphIsAdjacent(web, n->s, url)) {
            int outLinks = GraphOutlinkSize(web, n->s);
            pR = pR + ((n->pageRank) / outLinks);
        }
    }
    pR = pR * damping;
    pR = pR + ((1 - damping) / nUrl);
    return pR;
}