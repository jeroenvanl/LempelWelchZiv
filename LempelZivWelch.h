/*COMSM1201 Student Jeroen van Lith*/
/* Week 11+: Turtle Graphics */
/* E X E R C I S E  1 . 4   C O M P R E S S E D   T U R T L E S */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


#define BUFF 64
#define NUMALLOWEDARGS 2
#define LOOPARGS 8
#define READNEXT fscanf(p->fp,"%s",p->cw)
#define INDENT 3
#define MINLOOPSIZE 2
#define MINLOOPS 2
#define strsame(A,B) (strcmp(A, B)==0) /* From the COMSM1201 hand-out. Thanks! */
#define ERROR(PHRASE) {fprintf(stderr,\
"Fatal Error %s occured in %s,line %d\n"\
,PHRASE, __FILE__, __LINE__); exit(2);} /* Also from the COMSM1201 hand-out. Thanks! */


typedef enum _bool{
   FALSE,
   TRUE
}Bool;

typedef struct loop{
	int start;
	int length;
	int rep;
}Loop;


typedef struct prog{
	char cw[BUFF]; /* current word */
	char** prog;
	int c;
	char** optiprog;
	int oc;
	Loop loop;
	FILE* fp;
}Prog;


/* 1. Preparation */
void checkCount(int c);
FILE *openFile(char *filename);

/* 2. Turtle functions */
void main_(Prog* p);
void instruc(Prog* p);
void fd(Prog* p);
void lt(Prog* p);
void rt(Prog* p);
void loop(Prog* p);
void set(Prog* p);

/* 3. Reverse Polish Notation (RPN) */
void rpn(Prog* p);

/* 4. Type Checks & Conversions */
Bool strSame(Prog* p,int o,char* test);
Bool isVarNum(char* var);
Bool isVar(char* var);
Bool isNum(char* var);
Bool isOp(char A);

/* 5. Redundancy Check */
void redundancyCheck(Prog* p);

/* 6. Extension: Compressed Turtles */
void readIn(Prog* p);
void detectRep(Prog* p);
void updateProg(Prog* p);
void getWordCount(Prog* p);
void scanRep(Prog* p);
Bool grammarCheck(Prog* p, int start);
void scanFrame(Prog* p, int start, int length);
void storeLoop(Prog* p, int length, int start, int rep);
void reWrite(Prog* p); 
void insert(Prog* p, char* string, int i);
void printOptiProg(Prog* p);
Bool nextLine(Prog* p, int i);
void freeProg(Prog* p);
