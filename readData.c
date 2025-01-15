#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "List.h"
#include "Graph.h"
#include "bst.h"
#include "readData.h"

#define MAX_LINE_LENGTH 1000
#define START "#start"
#define SECTION "Section-1"
#define SECTION_TWO "Section-2"
#define END_MARKER_ONE "#end Section-1"
#define START_MARKER_TWO "start Section-2"
#define END_MARKER_TWO "end Section-2"

static char *myStrdup(char *s);
static void insertLinks(char *line, Graph web, char *urlToAdd);

// Returns a list of urls from collection.txt
List getCollection(List url) {
    FILE *fp = fopen("collection.txt", "r");
    if (fp == NULL) {
		fprintf(stderr, "Can't open collection.txt\n");
		exit(EXIT_FAILURE);
	}
	char line[MAX_LINE_LENGTH];
	while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
		char *token = strtok(line, " \t\n");
		while (token != NULL) {
			char *duplicate = myStrdup(token);
			ListAppend(url, duplicate);
			free(duplicate);
			token = strtok(NULL, " \t\n");
		}
	}
	ListSort(url);
	fclose(fp);
	return url;
}


// Code taken from fb.c in Lab 3 COMP2521 Summer Term 2024
// Duplicates a string
static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Function returns a graph of all urls
Graph GetGraph(List url, Graph web) {
	ListIterator it = ListItNew(url);
	while (ListItHasNext(it)) {
		char *url = ListItNext(it);
		GraphAddUrl(web, url);
	}
	ListItFree(it);
	ListIterator l = ListItNew(url);
	while (ListItHasNext(l)) {
		char *urlToAdd = ListItNext(l);
		// Add the ".txt" after url name
		char *urlFile = malloc(strlen(urlToAdd) + 5);
		strcpy(urlFile, urlToAdd);
		strcat(urlFile, ".txt");
		// Open the file containing the url
		FILE *fp = fopen(urlFile, "r");
		if (fp == NULL) {
			fprintf(stderr, "Can't open %s\n", urlFile);
			exit(EXIT_FAILURE);
		}
		// Read section 1 of the text file associated with url
		char line[MAX_LINE_LENGTH];
		while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
			if (strstr(line, END_MARKER_ONE) != NULL) {
				break;
			}
			if (line[0] == '\n' || line[0] == '\0') {
            	continue;  // Skip empty lines
        	}
			// If within section 1, we read the file. Tokenise 
			// every URL 
			insertLinks(line, web, urlToAdd);
		}
		fclose(fp);
		free(urlFile);
	}
	ListItFree(l);
	return web;
}

// Helper function which inserts adjacency links from url file
// being read to other urls
// Links are directional
static void insertLinks(char *line, Graph web, char *urlToAdd) {
	char *token = strtok(line, " \t\n");
	while (token != NULL) {
		char *duplicate = myStrdup(token);
		if (strcmp(urlToAdd, token) != 0
			&& strcmp(token, START) != 0
			&& strcmp(token, SECTION) != 0) {
			GraphInsertLink(web, urlToAdd, duplicate);
		}
		free(duplicate);
		token = strtok(NULL, " \t\n");
	}
}

// Returns a bst with linked list implementation
struct object *invertedList(List url, struct object *t) {
	ListIterator it = ListItNew(url);
	while (ListItHasNext(it)) {
		// Add the ".txt" after url name
		char *urlToAdd = ListItNext(it);
		char *urlFile = malloc(strlen(urlToAdd) + 5);
		strcpy(urlFile, urlToAdd);
		strcat(urlFile, ".txt");
		// Open the file containing the url
		FILE *fp = fopen(urlFile, "r");
		if (fp == NULL) {
			fprintf(stderr, "Can't open %s\n", urlFile);
			exit(EXIT_FAILURE);
		}

		// Read section 2 of the text file associated with url
		char line[MAX_LINE_LENGTH];
		bool inSection = false;
		while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
			// If "#start Section-2" encountered, commence reading
			if (strstr(line, START_MARKER_TWO) != NULL) {
				inSection = true;
			}
			else if (strstr(line, END_MARKER_TWO) != NULL) {
				inSection = false;
				break;
			}

			// If within section 2, we read the file. Tokenise 
			// every URL 
			// Remove punctuation marks from end, and add it to bst
			// Insert url into specific linked list for bst
			if (inSection) {
				char *token = strtok(line, " \t\n");
				while (token != NULL) {
					if (strcmp(token, START) != 0 &&
						strcmp(token, SECTION_TWO) != 0) {
						char *duplicate = myStrdup(token);
						for (int i = 0; duplicate[i] != '\0'; i++) {
							duplicate[i] = tolower((unsigned char)duplicate[i]);
						}
						int length = strlen(duplicate);
						while (length > 0 && strchr(".,:;?*", duplicate[length - 1]) != NULL) {
							duplicate[--length] = '\0';
						}
						if (strlen(duplicate) > 0) {
							t = bstInsert(t, duplicate, urlToAdd);
						}
						free(duplicate);
					}
					token = strtok(NULL, " \t\n");
				}
			}
		}
		fclose(fp);
		free(urlFile);
	}
	ListItFree(it);
	return t;
}

