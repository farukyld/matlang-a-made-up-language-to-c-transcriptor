#ifndef FUNC_NAMES
#define FUNC_NAMES

void test(char *, char *,int);

void tests(char *,int);

void testd(int, char *,int);

void testp(void*,int);

int expr(char *,int*,int*,int*,int);

int term(char *,int* ,int* ,int*,int);

int moreTerms(char *,int* ,int* ,int* ,int*,int*,int);

int factor(char *,int* ,int*, int*,int);

int moreFactors(char *,int* ,int* ,int* ,int*,int);

int isInteger(char *);

char *spacedString(char *);

void tokenizeLine(char *);

void deleteComments(char *);

int isSpecialCharacter(char);

void readAllLines(FILE *);

int isMatlangKeyword(char *);

int isVarType(char *);

int isValidIdentifierName(char *);

int getIndex(char *);

int isValidMatrixDeclaration();

int isValidVectorDeclaration();

int assignment(char*,int);

void printIthIdentifierInfo(int,int);

int statement(char*);

int scalar(char*,int);

int id_optionallyIndexed(char*,int*,int*,int*,int);

int forStmt(char*,int);

void printRemainingTokens(char* ,int );

int id(char *, int *, int *, int *, int );

#endif
