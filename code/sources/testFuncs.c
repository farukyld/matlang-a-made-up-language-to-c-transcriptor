#include <header.h>

#ifdef ALLOW_DEBUGGING_OUTPUT_TO_CONSOLE
void printIthIdentifierInfo(int i,int indent){
    char msg[N];
    sprintf(msg,"id: %5s dim: %3d dim1: %3d dim2: %3d  ",identifierNames[i],identifierAttributes[i]->dim,identifierAttributes[i]->dim1,identifierAttributes[i]->dim2);
    tests(msg,indent);
}

char* numOfIndent(int i){
    char* indent= malloc(50);
    indent[0]=0;
    for(;i-->0;){
        strcat(indent,"|\t");
    }
    return indent;
}

void test(char *msg, char *sender,int indent) {
    printf("%s%s: %s\n",numOfIndent(indent), sender,msg);
    fflush(stdout);
}

void printRemainingTokens(char* sender,int indent){
    char rem[N];
    int i;
    rem[0]=0;
    for (i = cur; i < tokenCount; i++) {
        strcat(rem," ");
        strcat(rem,tokens[i]);

    }
    char* indents=numOfIndent(indent);
    printf("%s%s:::%s\n",indents,sender,rem);
    free(indents);
    fflush(stdout);
}

void tests(char *msg,int indent) {
    char* indents=numOfIndent(indent);
    printf("%s%s\n",indents,msg);
    free(indents);
    fflush(stdout);
}

void testd(int a, char *sender,int indent) {
    char* indents=numOfIndent(indent);
    printf("%s%s: %d\n",indents, sender, a);
    free(indents);
    fflush(stdout);
}

void testp(void* a,int indent) {
    char* indents=numOfIndent(indent);
    printf("%s%p\n", indents, a);
    free(indents);
    fflush(stdout);
}
#else
void printIthIdentifierInfo(int i,int indent){
}

char* numOfIndent(int i){
    return NULL;
}

void test(char *msg, char *sender,int indent) {
}

void printRemainingTokens(char* sender,int indent){
}

void tests(char *msg,int indent) {
}

void testd(int a, char *sender,int indent) {
}

void testp(void* a,int indent) {
}
#endif
