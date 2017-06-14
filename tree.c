#include "CrossReferenceFileBuilder.h"

/******************INITIALIZEROOT******************
Accepts a root tree node, first word (identifier), 
and the line number. Allocates the new root, and
initializes the queue and leaf nodes.
*/
TREENODE *initializeRoot(TREENODE *root, char *identifier, int line_num)
{	
	root->identifier = malloc(strlen(identifier) + 1);
	strcpy(root->identifier, identifier);
	root->left = NULL;
	root->right = NULL;
	root->firstline_q = NULL;
	root->lastline_q = NULL;
	root->count = 1;
	TREENODE **rootPtr = NULL;
	rootPtr = &root;
	enqueue(&(*rootPtr)->firstline_q, &(*rootPtr)->lastline_q, line_num);
	return root;
}

/******************INSERT******************
Accepts a treenode, word and line #. If the 
word isn't in the tree, it is inserted based on
ASCII order. If it is in the tree, the new line # is
inserted to the queue and count increases.
*/
int insert(TREENODE **root, char *identifier, int line_num)
{
	if (!(*root))
	{
		if (!(*root = (TREENODE *)malloc(sizeof(TREENODE))))
			printf("Mem error allocating tree node.\n");
		(*root)->identifier = malloc(strlen(identifier));
		strcpy((*root)->identifier, identifier);
		(*root)->firstline_q = NULL;
		(*root)->lastline_q = NULL;
		enqueue(&(*root)->firstline_q, &(*root)->lastline_q, line_num);
		(*root)->count = 1;
		(*root)->left = (*root)->right = NULL;
		return 1; // data inserted
	}
	if (strcmp((*root)->identifier, identifier) < 0 )
		return insert(&(*root)->right, identifier, line_num);
	else if (strcmp((*root)->identifier, identifier) > 0)
		return insert(&(*root)->left, identifier, line_num);
	else
	{
		if (line_num != (*root)->lastline_q->line_num)
		{
			enqueue(&(*root)->firstline_q, &(*root)->lastline_q, line_num);
			(*root)->count++;
		}
		return 1;
	}
}

/******************WRITETREE******************
Accepts a treenode and file pointer, and writes 
identifiers to the file in L - ROOT - R order, 
followed by each item of the queue.
*/
void writeTree(TREENODE *root, FILE *outFile)
{
	int i;
	if (root)
	{
		QUEUENODE *front = NULL;
		writeTree(root->left, outFile);
		// writing
		fprintf(outFile, "%-25s", root->identifier);
		front = root->firstline_q;
		for (i = 1; i <= root->count; i++)
		{
			if (i % 10 == 0)
				fprintf(outFile, "\n%25s", " ");
			fprintf(outFile, "%5d", front->line_num);
			front = front->next;
		}
		fprintf(outFile, "\n");
		writeTree(root->right, outFile);
	}
	return;
}

/******************DELETETREE******************
Accepts a treenode and first checks if there is a left
leaf, if so it calls delete function for left. Then right
is checked to delete and called. Then the root itself 
deletes its queue, before deleting itself.
*/
void deleteTree(TREENODE *root)
{
	QUEUENODE *front;
	int i = 0;
	if (root)
	{
		if ((root)->left)
			deleteTree((root)->left);
		if ((root)->right)
			deleteTree((root)->right);
		for (i = 0; i < (root)->count; i++)
		{
			front = dequeue(&(root)->firstline_q, &(root)->lastline_q);
			if (front)
				free(front);
		}
		free(root);
		// must also free identifier and firstWord, causes errors
	}
}

// not used in final iteration
/*void printTree(TREENODE *root)
{
int i;
if (root)
{
QUEUENODE *front = NULL;
printTree(root->left);
printf("%-25s", root->identifier);
// printf("")
front = root->firstline_q;
for (i = 1; i <= root->count; i++)
{
if (i % 10 == 0)
printf("\n%25s", " ");
printf("%5d", front->line_num);
front = front->next;
}
printf("\n");
printTree(root->right);
}
return;
}*/

/*void printTreeIndented(TREENODE *root, int i, char *identifier)
{
int j = i;
if (root)
{
printTreeIndented(root->right, i + 1, "R: ");
while (j)
{
printf("\t");
j--;
}
printf("%d. %s %3s\n", i, identifier, root->identifier); //
printTreeIndented(root->left, i + 1, "L: ");
}
return;
}*/