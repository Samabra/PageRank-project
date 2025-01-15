#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_URL_COUNT 100

// Struct to be implemented in struct array
// Keeps count of urls across search terms
// Keeps pageRank details in case of ties
typedef struct {
    char *url;
    int count;
    double pageRank;
} UrlInfo;

static void processLine(char *line, char *argv[], int argc, int *totalUrls, UrlInfo *urlArray);
static void processLineTwo(char *token, char *searchTerm, int *totalUrls, UrlInfo *urlArray);
static void processLineThree(char *token, int urlIndex, int *totalUrls, UrlInfo *urlArray);
static char *myStrdup(char *s);
static int compareUrls(const void *a, const void *b);
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <search_term_1> [<search_term_2> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen("invertedIndex.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Can't open file invertedIndex.txt\n");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    UrlInfo urlArray[MAX_URL_COUNT] = {0};
    int totalUrls = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        // Function to process each line of invertedList.txt
        processLine(line, argv, argc, &totalUrls, urlArray);
    }
    fclose(file);
    FILE *pageRankFile = fopen("pagerankList.txt", "r");
    if (pageRankFile == NULL) {
        fprintf(stderr, "Cant open file pagerankList.txt\n");
        exit(EXIT_FAILURE);
    }
    char pageLine[MAX_LINE_LENGTH];
    char *url = NULL;
    double pageRank;
    char *token;
    while (fgets(pageLine, sizeof(pageLine), pageRankFile) != NULL) {
        token = strtok(pageLine, ", ");
        if (token != NULL) {
            url = myStrdup(token);

            // Used to iterate past the outdegree
            token = strtok(NULL, ", ");

            // Collects pageRank information from pagerankList.txt
            token = strtok(NULL, ", ");
            pageRank = atof(token);
        }
        for (int i = 0; i < totalUrls; i++) {
            if (strcmp(url, urlArray[i].url) == 0) {
                // Updates pageRank for the url in the urlArray
                urlArray[i].pageRank = pageRank;
                break;
            }
        }
        free(url);
    }

    // Quicksort, calls function compareUrls
    // Ensures urls are kept in descending order of frequency amongst search terms
    // If tied, then in descending order of pageRank
    qsort(urlArray, totalUrls, sizeof(UrlInfo), compareUrls);
    for (int i = 0; i < totalUrls; i++) {
        printf("%s\n", urlArray[i].url);
    }
    for (int i = 0; i < totalUrls; i++) {
        free(urlArray[i].url);
    }
    fclose(pageRankFile);
    return 0;
}

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Initiates the search for the terms in command line arguments
static void processLine(char *line, char *argv[], int argc, int *totalUrls, UrlInfo *urlArray) {
    for (int i = 1; i < argc; i++) {
        // When search term found, start tokenising the line
        if (strstr(line, argv[i]) != NULL) {
            char *searchTerm = argv[i];
            // Get rid of any whitespace at the end of lines, so
            // the whitespace does not become tokenised
            int length = strlen(line);
            if (length > 0 && line[length - 1] == '\n') {
                line[length - 1] = '\0';
            }
            char *token = strtok(line, " ");
            // Call to second helper function, to avoid adding duplicate urls
            // into array called urlArray
            processLineTwo(token, searchTerm, totalUrls, urlArray);
        }
    }
}

static void processLineTwo(char *token, char *searchTerm, int *totalUrls, UrlInfo *urlArray) {

    while (token != NULL) {
        if (strcmp(token, searchTerm) != 0) {
            int urlIndex = -1;
            // Call to another helper function, which ensures that 
            // urls already in array have their frequency incremented
            // New urls are appended to array
            processLineThree(token, urlIndex, totalUrls, urlArray);
        }
        token = strtok(NULL, " ");
    }
}

static void processLineThree(char *token, int urlIndex, int *totalUrls, UrlInfo *urlArray) {
    for (int j = 0; j < *totalUrls; j++) {
        // If url exists, just increment the frequency
        // of that url.
        if (strcmp(token, urlArray[j].url) == 0) {
            urlIndex = j;
            break;
        }
    }
    // Else add the url to the array
    // Update number of total amount of urls
    if (urlIndex == -1) {
        if (*totalUrls < MAX_URL_COUNT) {
            urlIndex = *totalUrls;
            urlArray[*totalUrls].url = myStrdup(token);
            (*totalUrls)++;
        }
    }

    urlArray[urlIndex].count++;
}

// Called from qsort
// Ensures that urls are kept in descending order
// of count
// If tied, then kept in descending order of pageRank
static int compareUrls(const void *a, const void *b) {
    const UrlInfo *urlA = (const UrlInfo *)a;
    const UrlInfo *urlB = (const UrlInfo *)b;

    if (urlA->count > urlB->count) {
        return -1;
    }
    else if (urlA->count < urlB->count) {
        return 1;
    }
    else {
        return (urlA->pageRank > urlB->pageRank) ? -1 : 1;
    }
}