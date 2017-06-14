# Cross-Reference-File-Builder
This program reads a user's .c file as specified on the command line and creates a cross reference file based on its code. This file contains each word and which lines it appears on. Words used must start with [A-Za-z_], and may contain any combination of [A-Za-z_0-9]. Words inside quotations are ignored.
Each word is inserted into a binary search tree, whose nodes consist of a word and a queue of line numbers.
