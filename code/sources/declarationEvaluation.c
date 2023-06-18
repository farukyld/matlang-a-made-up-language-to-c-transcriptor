#include <header.h>


//check if given token is a valid isValidIdentifierName
int isValidIdentifierName(char *token) {

    if (isMatlangKeyword(token))
        return 0;
    char *c = token;
    if (!(isalpha(*c) || (*c) == '_'))
        return 0;

    for (; *c != 0; c++) {
        if (!(isalnum(*c) || (*c) == '_'))
            return 0;
    }
    return 1;
}

//return -1 if not exist
int getIndex(char* identifier){//currently, linear search. maybe I will make it hashtable
    int i;
    for (i = 0; i < identifierCount; i++) {
        if (strcmp(identifier, identifierNames[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int isMatlangKeyword(char *identifier) {
    int i;
    for (i = 0; i < matlangKeywordCount; i++) {
        if (strcmp(identifier, matlangKeyWords[i]) == 0)
            return 1;
    }
    return 0;
}

//return 0 if str is not a valid type,
// else return index of element that is equal to str in the array {"matrix", "vector", "scalar"}
int isVarType(char *str) {
    char *varTypes[] = {"matrix", "vector", "scalar"};
    int i;
    for ( i = 0; i < 3; i++) {
        if (strcmp(str, varTypes[i]) == 0)return i + 1;
    }
    return 0;
}

int isValidMatrixDeclaration() {
    if (tokenCount != 7)
        return 0;
    return (strcmp(tokens[2], "[") == 0) && (strcmp(tokens[4], ",") == 0) && (strcmp(tokens[6], "]") == 0)
           && isInteger(tokens[3]) && isInteger(tokens[5]);
}

int isValidVectorDeclaration() {
    if (tokenCount != 5)
        return 0;
    return (strcmp(tokens[2], "[") == 0) && (strcmp(tokens[4], "]") == 0) && isInteger(tokens[3]);
}