#define _CRT_SECURE_NO_DEPRECATE
#ifdef _MSC_VER
#include <crtdbg.h>  // check for memory leaks
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ID_SIZE 25
#define LINE_SIZE 255
#define OUTFILE "output.txt"

// typedef for queue structure
#ifndef QUEUENODE_H
#define QUEUENODE_H
typedef struct queueNode QUEUENODE;
struct queueNode
{
	int line_num;
	struct queueNode *next;
};
#endif

// typedef for BST treenode structure
#ifndef TREENODE_H
#define TREENODE_H
typedef struct treeNode TREENODE;
struct treeNode
{
	char *identifier;
	struct treeNode *left;
	struct treeNode *right;
	int count;
	QUEUENODE *firstline_q;
	QUEUENODE *lastline_q;
};
#endif

// main() CrossReferenceFileBuilder.c functions
void greetUser();
FILE *openFile(char *infilename);
char *parseFile(FILE *inFile, TREENODE *root);
char *checkWord(char *word, char *linePtr);
void writeOutFile(FILE *outFile, FILE *inFile, TREENODE **root);

// tree.c functions
TREENODE *initializeRoot(TREENODE *root, char *identifier, int line_num);
void printTree(TREENODE *root);
int insert(TREENODE **root, char *identifier, int linenum);
void printTreeIndented(TREENODE *root, int i, char *identifier);
void writeTree(TREENODE *root, FILE *outFile);
void deleteTree(TREENODE **root);

// queue.c functions
QUEUENODE *dequeue(QUEUENODE **queue, QUEUENODE **rear);
void enqueue(QUEUENODE **queue, QUEUENODE **rear, int line_num);