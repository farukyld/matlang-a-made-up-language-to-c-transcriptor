#ifndef HEADER1
#define HEADER1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <funcNames.h>

#define LEFT_ASSOC
#define ALLOW_DEBUGGING_OUTPUT_TO_CONSOLE
#define MAX_TOKENS 100
#define N  1000
#define LINE_LENGTH 200
#define NUM_LINES 100
#define LEN(ar) (sizeof(ar)/(sizeof(ar[0])))
#define MAX_ID_COUNT 101
//it is a safety check to provide attempts to read out of range after possibly updating cur.
#define CUR_BOUND_CONTROL if(cur==tokenCount){tests("end of line reached",0);return 0;}

//NOT USED
//typedef struct Expr {
//    int dim, dim1, dim2;
//} Expr;
//
//typedef struct Term {
//    int dim, dim1, dim2;
//} Term;
//
//typedef struct MoreTerms {
//    int sign, dim, dim1, dim2, isNull;
//} MoreTerms;
//
//typedef struct Factor {
//    int dim, dim1, dim2;
//} Factor;
//
//typedef struct MoreFactors {
//    int dim, dim1, dim2, isNull;
//} MoreFactors;

typedef struct Id {
    int dim, dim1, dim2;
} Id;

extern char *tokens[];
extern char **allLines;
extern int lineCount;
extern int cur;
extern int tokenCount;
extern int matlangKeywordCount;
extern int spCharCount;
extern char spChars[];
extern char *matlangKeyWords[];
extern char **identifierNames;
extern Id **identifierAttributes;
extern int identifierCount;
extern int indentation;//0 or 1 or 2.
extern char* copy1;

//NOT USED
//when parsing functions return postfix expressions, they should inform the caller about the operation.
//some operators are easy to write (*,-,+, which are gonna be used to inform about scalar operations)
//these variables are storing other operator symbols.
//(in order for me not to have to memorize symbols, just call by their name)
//
//extern char *matMulOp;
//extern char *matAddOp;
//extern char *matSubOp;
//extern char *scMatOp;
//extern char *matScOp;
//extern char *matTpOp;
//extern char *matIndexOp;
//extern char *vecIndexOp;
//extern char *chooseOp;
//extern char *sqrtOp;

#endif