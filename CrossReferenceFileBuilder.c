/**************************************************************************
This program reads a user's .c file as specified on the command line and 
creates a cross reference file based on its code. This file contains each 
word and which lines it appears on. Words used must start with [A-Za-z_], 
and may contain any combination of [A-Za-z_0-9]. Words inside quotations 
are ignored.
Each word is inserted into a binary search tree, whose nodes consist of a 
word and a queue of line numbers.
***************************************************************************/
#include "CrossReferenceFileBuilder.h"
#include "queue.c"
#include "tree.c"

// argc = 3
// argv[] = "Z:\C\CrossReferenceFileBuilder\Debug\CrossReferenceFileBuilder.exe", 
//			"CrossReferenceFileBuilder.c", "input.c"
int main(int argc, char *argv[])
{	
	greetUser();
	FILE *inFile = openFile(argv[2]); // argv[2] = "input.c"
	int i = 1;


	printf("Allocating and building tree...\n\n");
	TREENODE *root = NULL;
	char word[ID_SIZE] = "";
	int line_num = 1;
	root = (TREENODE *)malloc(sizeof(TREENODE));
	if (!root)
		printf("Error allocating root\n");
	strcpy(word, parseFile(inFile, root));

	FILE *outFile = fopen(OUTFILE, "w");
	if (!outFile)
		printf("Unable to open %s\n", OUTFILE);
	else
	{
		printf("Writing output to: %s\n\n", OUTFILE);
		writeOutFile(outFile, inFile, root);
	}
		
	printf("Deleting tree.\n");
	deleteTree(root);

	if (fclose(inFile) == EOF)
	{
		printf("Error closing %s.\nExiting\n", argv[2]);
	}
	if (fclose(outFile) == EOF)
	{
		printf("Error closing %s.\nExiting\n", OUTFILE);
	}

	#ifdef _MSC_VER
		_CrtDumpMemoryLeaks();
		printf(_CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
		system("pause");
	#endif

	return 0;
}

/******************GREETUSER******************
Explains the structure of this program.
*/
void greetUser()
{
	printf("This program uses several files to work:\n  - CrossReferenceFileBuilder.h ");
	printf("is a header file containing global variables, typedefs and function prototypes.\n");
	printf("  - queue.c contains function definitions for the queue functions,\n");
	printf("  - tree.c contains function definitions for the tree functions, \n");
	printf("  - CrossReferenceFileBuilder.c (main) contains functions for parsing and writing files.\n\n");
}

/******************OPENFILE******************
Accepts a char infilename and tests to see if 
the file exists, if so it's opened for reading 
and the file pointer is returned.
*/
FILE *openFile(char *infilename)
{
	printf("Opening input file: %s\n\n", infilename);
	FILE *inFile = fopen(infilename, "r");
	if (!inFile) // check for infilename
		printf("Unable to open %s!\n", infilename);
	return inFile;
}

/******************PARSEFILE******************
Accepts an input filestream and BST root as input.
Returns the first word to main() for printing the tree.

Reads 1 line at a time from inFile, uses strtok to grab 
a word, and calls checkWord to parse for special characters.
The word is inserted as the root if root_flag is set,
otherwise it calls the insert tree.c function.
*/
char *parseFile(FILE *inFile, TREENODE *root)
{
	char line[LINE_SIZE];
	char lineCopy[LINE_SIZE];
	char *firstWord;
	char *badChars = "~`!@#$^&*()+-=[]{}\\|;:,.<>/?0123456789\040\011\n"; // _ is ok
	char *specialChars = "\042\047%";
	int counter = 0;
	char *temp;
	int line_num = 0;
	int root_flag = 1;
	while (fgets(line, LINE_SIZE, inFile))
	{
		strcpy(lineCopy, line);
		line_num++;
		temp = strtok(line, badChars);
		while (temp)
		{
			temp = checkWord(temp, lineCopy);
			if (*temp != '\177')
			{
				if (root_flag)
				{
					root = initializeRoot(root, temp, line_num);
					firstWord = malloc(strlen(temp) + 1); // heap
					strcpy(firstWord, temp);
					root_flag = 0;
				}
				else
				{
					insert(&root, temp, line_num);
				}
			}
			temp = strtok(NULL, badChars);
		}
	}
	return firstWord;
}

/******************CHECKWORD******************
Accepts a char *(word) and char *(line) from 
parseFile, checks if they contain " ' or %. If so,
sends a dummy variable back to parseFile to stop
the word from being inserted. If " ' or % are not
found, the word is passed back unchanged.
*/
char *checkWord(char *word, char *line)
{
	int counter = 0, i = 0;
	char *linePtr = NULL;
	char *wordPtr = NULL;
	char *wordCopy = NULL;
	char *specialChars = "\042\047%"; // " ' %
	counter = strcspn(word, specialChars);
	if (counter != strlen(word))
		return strcpy(word, "\177");
	return word;
}

/******************WRITEOUTFILE******************
Accepts an inFile, outFile and treenode as input.
Writes specified formatted text to the output file:
   1. A timestamp of when the file was created
   2. Calls writeTree to write the line in L-Root-Right
      order to the file, along with the queue of line #s.
   3. Rewinds the input file and writes each line to 
      the output file, with a line # in front.
*/
void writeOutFile(FILE *outFile, FILE *inFile, TREENODE **root)
{
	char outputline[] = "/************************  Cross Reference Listing  ************************/\n\n";
	fprintf(outFile, outputline);

	time_t currtime;
	currtime = time(NULL);
	char timestamp[LINE_SIZE] = "Cross reference listing made at: ";
	strcat(timestamp, ctime(&currtime));
	strcat(timestamp, "\n\n");
	fprintf(outFile, timestamp);

	writeTree(root, outFile);
	fprintf(outFile, "\n");

	char inputline[LINE_SIZE];
	rewind(inFile);
	int line_num = 1;
	while (fgets(inputline, LINE_SIZE, inFile))
	{
		fprintf(outFile, "%d\t: ", line_num);
		fputs(inputline, outFile);
		line_num++;
	}
	fprintf(outFile, "\n");

}
/****************************** SAMPLE OUTPUT (SCREEN) ******************************/
/*

This program uses several files to work:
- CrossReferenceFileBuilder.h is a header file containing global variables, typedefs and function prototypes.
- queue.c contains function definitions for the queue functions,
- tree.c contains function definitions for the tree functions,
- CrossReferenceFileBuilder.c (main) contains functions for parsing and writing files.

Opening input file: input.c

Allocating and building tree...

Writing output to: output.txt

Deleting tree.
Memory Leak
Press any key to continue . . .

*/

/****************************** SAMPLE OUTPUT FILE 1 ******************************/

/************************  Cross Reference Listing  ************************/
/*
Cross reference listing made at: Wed Mar 22 17:55:06 2017


FILE                        19   20   21   22   32   55   90
MAXCITIES                    5   18
MAXLINE                      6   58   60
NULL                        40   46   71
argc                        16   20   24   32   34
arguments                   36
argv                        16   24
char                        10   16   20   32   58   71
checkArgs                   20   24   32
city                        58   62   63   71   76
command                     36
count                       12   21   22   25   26   57   63   66   67
68   70   76   80   81   85   90   94   96
ctype                        4
define                       5    6
else                        66   78
error                       73
exit                        37   43   49   74
f                           95
fgets                       60
fin                         19   20   21   22   24   25   32   40   55
60
first                       57   65
float                       96
fopen                       40   46
for                         94
fout                        19   20   24   26   32   46   90   95
fprintf                     95
h                            1    2    3    4
highTemp                    57   62   67   80
highTemps                   18   21   25   26   55   63   67   68   70
76   80   81   90   95   96
hightemps                   22
i                           92   94   95   96
if                          34   40   46   63   65   70
include                      1    2    3    4
infile                      20   32   40
input                       42
int                         11   12   16   20   21   22   32   55   57
90   92
line                        36   58   60   62
main                        16
malloc                      71
number                      36
of                          36
on                          36
open                        42   48
outfile                     20   32   46
output                      48
perror                      42   48   73
printf                      36
readIntoWeatherStructs      21   25   55
return                      85
sscanf                      62
statecity                   10   63   70   76   95
stdio                        1
stdlib                       2
strcmp                      63
strcpy                      76
string                       3
strlen                      71
struct                       8   18   21   22   55   90
tempsum                     11   67   80   96
void                        20   22   32   90
weather                      8   18   21   22   55   90
while                       60
writeAveragesToOutput       22   26   90

1	: #include <stdio.h>
2	: #include <stdlib.h>
3	: #include <string.h>
4	: #include <ctype.h>
5	: #define MAXCITIES 100
6	: #define MAXLINE    80
7	:
8	: struct weather
9	: {
10	:     char *statecity;
11	:     int   tempsum;
12	:     int   count;
13	: };
14	:
15	:
16	: int main(int argc, char **argv)
17	: {
18	:     struct weather highTemps[MAXCITIES] = {{"", 0, 0}};
19	:     FILE *fin, *fout;
20	:     void checkArgs(int argc,char *infile,char *outfile,FILE **fin,FILE **fout);
21	:     int readIntoWeatherStructs(struct weather *highTemps, FILE *fin), count;
22	:     void writeAveragesToOutput(struct weather *hightemps, int count, FILE *fin);
23	:
24	:     checkArgs(argc, argv[1], argv[2], &fin, &fout);
25	:     count = readIntoWeatherStructs(highTemps, fin);
26	:     writeAveragesToOutput(highTemps, count, fout);
27	: }
28	:
29	:
30	:
31	:
32	: void checkArgs(int argc, char *infile, char *outfile, FILE **fin, FILE **fout)
33	: {
34	:     if (argc != 3)
35	:     {
36	:         printf("Wrong number of arguments on command line!\n");
37	:         exit(1);
38	:     }
39	:
40	:     if ((*fin = fopen(infile, "r")) == NULL)
41	:     {
42	:         perror("Cannot open input file");
43	:         exit(1);
44	:     }
45	:
46	:     if ((*fout = fopen(outfile, "w")) == NULL)
47	:     {
48	:         perror("Cannot open output file");
49	:         exit(1);
50	:     }
51	: }
52	:
53	:
54	:
55	: int readIntoWeatherStructs(struct weather *highTemps, FILE *fin)
56	: {
57	:     int count = 0, first = 1, highTemp;
58	:     char city[MAXLINE], line[MAXLINE];
59	:
60	:     while (fgets(line, MAXLINE, fin))
61	:     {
62	:          sscanf(line, "%s %d", city, &highTemp);
63	:          if (strcmp(city, highTemps[count].statecity) != 0)
64	:          {
65	:               if (first) first = 0;
66	:               else count++;
67	:               highTemps[count].tempsum = highTemp;
68	:               highTemps[count].count = 1;
69	:
70	:               if ((highTemps[count].statecity =
71	:                    (char *) malloc(strlen(city) + 1)) == NULL)
72	:               {
73	:                   perror("Malloc error!");
74	:                   exit(1);
75	:               }
76	:               strcpy(highTemps[count].statecity, city);
77	:          }
78	:          else
79	:          {
80	:               highTemps[count].tempsum += highTemp;
81	:               highTemps[count].count++;
82	:          }
83	:     }
84	:
85	:     return count;
86	: }
87	:
88	:
89	:
90	: void writeAveragesToOutput(struct weather *highTemps, int count, FILE *fout)
91	: {
92	:      int i;
93	:
94	:      for (i = 0; i <= count; i++)
95	:         fprintf(fout, "%s %-5.1f\n", highTemps[i].statecity,
96	:                  (float) highTemps[i].tempsum/highTemps[i].count);
97	: }

*/

/****************************** SAMPLE OUTPUT FILE 2 ******************************/
/************************  Cross Reference Listing  ************************/
/*
Cross reference listing made at: Wed Mar 22 18:02:03 2017


aChar                        1    5    8   12
char                         1
checkLetter                  1
else                         7   10
if                           5    8
int                          1    3
isLetter                     3    6    9   11   14
printf                      12
return                      14

1	: int checkLetter(char aChar)
2	: {
3	:     int isLetter;
4	:
5	:     if (aChar >= 'a' && aChar <= 'z')
6	:         isLetter = 1;
7	:     else
8	:         if (aChar aChar >= 'A' && aChar <= 'Z')
9	:             isLetter = 1;
10	:         else
11	:             isLetter = 0;
12	: 	printf("%c", aChar);
13	:
14	:     return isLetter;
15	: }

*/