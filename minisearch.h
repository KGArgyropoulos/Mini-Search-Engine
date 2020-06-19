#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int countLines(char *);
int validIdCheck(char *,int);
int mSL(char *,int);

typedef struct Node{
	char letter;
	char *leaf;
	struct Node *next_level;
	struct Node *same_level;
	struct Node1 *eoString;
}trie;

typedef struct Node1{
	int id;
	int tfs;
	struct Node1 *next;
}posting_list;

void insertion(struct Node **,int,char *);
struct Node *init(void);
struct Node1 *pl_init(void);

void search(char **,trie *,int,int,int *,int,char **,int *);
int findsize(int ,int ,int );
void bubbleSort(int *,double *,int*,int);
void searchResults(int *,double *,char **,int,int *,int *,int,char **,int);
void align(char *,char **,int,int);
void df(struct Node *,int);
int df_word(struct Node *,char *);
int tf(struct Node *,int,char *);
void destroyNodes(struct Node **,int);