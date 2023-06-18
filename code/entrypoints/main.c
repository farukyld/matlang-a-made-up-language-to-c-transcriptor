#include <header.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"//make ide ignore atoi warning


float newtonSqrt(float a,int allowInf){
    double res=a*.5;
    unsigned long long i=0,n=50;

    if(!allowInf)
    for(i=0;i<n;i++){
        res=(res-a/res)*.5;
    }
    else{
        while ((float)(res*res)!=a){
            res=(res-a/res)*.5;
            i++;
        }
    }
    int size=(sizeof(unsigned long long));
    printf("%llu %d\n",i,size);
    return (float)(res>0?res:-res);
}

int main(int argc, char **argv) {
//    fflush(stdout);
//    float a= newtonSqrt(3.402823466e38,1);
//    printf("%f", a);
    int i, j, k;

#pragma region read all lines of file and store them in allLines array if possible
    if (argc != 2) {
        printf("Give filename as command line argument\n");
        return 1;
    }


    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }
    readAllLines(fp);

#pragma endregion

#pragma  region storing identifiers. I think I can spaghetti-code this part.

    identifierNames = (char **) malloc(MAX_ID_COUNT * sizeof(char *));
    identifierAttributes = (Id **) malloc(MAX_ID_COUNT * sizeof(Id *));


    for (i = 0; i < lineCount; i++) {
        char *currLine = strdup(allLines[i]);

        tokenizeLine(spacedString(currLine));

        if (tokenCount != 0) {//no problem if empty line
            int declarationType = isVarType(tokens[0]);

            if (declarationType) {//declaration line
                if (tokenCount < 2) {//there is at least 2 tokens in a declaration line
                    tests("not valid declaration", 0);
                    printf("Error (Line %d)",i+1);
                    return 1;
                }
                if (isValidIdentifierName(tokens[1])) {
                    if (identifierCount == MAX_ID_COUNT) {
                        tests("max id count is reached. ", 0);
                        printf("max id count is reached. ");
                        return 1;
                    }
                    if (getIndex(tokens[1]) != -1) {
                        test(tokens[1], "redeclaration at line x for id", 0);
                        printf("Error (Line %d)",i+1);
                        return 1;
                    }
                    Id *newAttr;
                    switch (declarationType) {//save it if possible
                        case 1://matrix (return value of isVarType handles it)
                            if (!isValidMatrixDeclaration()) {
                                tests("not valid matrix declaration", 0);
                                printf("Error (Line %d)",i+1);
                                return 1;
                            }
                            identifierNames[identifierCount] = strdup(tokens[1]);
                            newAttr = identifierAttributes[identifierCount] = (Id *) malloc(sizeof(Id));
                            newAttr->dim = 2;
                            newAttr->dim1 = atoi(tokens[3]);
                            newAttr->dim2 = atoi(tokens[5]);
                            identifierCount++;
                            break;
                        case 2://vector
                            if (!isValidVectorDeclaration()) {
                                tests("not valid vector declaration", 0);
                                printf("Error (Line %d)",i+1);
                                return 1;
                            }
                            identifierNames[identifierCount] = strdup(tokens[1]);
                            newAttr = identifierAttributes[identifierCount] = (Id *) malloc(sizeof(Id));
                            newAttr->dim = 1;
                            newAttr->dim1 = atoi(tokens[3]);
                            newAttr->dim2 = 1;
                            identifierCount++;
                            break;
                        case 3://scalar
                            if (tokenCount != 2) {
                                tests("not valid scalar declaration", 0);
                                printf("Error (Line %d)",i+1);
                                return 1;
                            }
                            identifierNames[identifierCount] = strdup(tokens[1]);
                            newAttr = identifierAttributes[identifierCount] = (Id *) malloc(sizeof(Id));
                            newAttr->dim = 0;
                            newAttr->dim1 = 0;
                            newAttr->dim2 = 0;
                            identifierCount++;
                            break;
                        default:
                            break;
                    }
                } else {
                    tests("not valid id name in declaration Error (Line x)", 0);
                    printf("Error (Line %d)",i+1);
                    return 1;
                }
            } else {
                break;
            }
        }
        free(currLine);
    }

    identifierNames = (char **) realloc(identifierNames, identifierCount * sizeof(char *));
    identifierAttributes = (Id **) realloc(identifierAttributes, identifierCount * sizeof(Id *));

#pragma endregion

#pragma region  extract matfile location
    char matFileLocation[N/2];
    matFileLocation[0] = 0;
    tokenizeLine(spacedString(argv[1]));

    for (j = 0; j < tokenCount; j++) {
        if (strcmp(tokens[j + 2], "mat") == 0) {
            break;
        }
    }
    for (k = 0; k < j; k++) {
        strcat(matFileLocation, tokens[k]);
    }
    char cFileLocation[N];
    sprintf(cFileLocation, "%s%s", matFileLocation, "file.c");
//    test(argv[1],"mat loc");
//    test(cFileLocation,"c loc");

#pragma endregion

#pragma region create file and copy required lines into it, define variables
    FILE *targetFile = fopen(cFileLocation, "w");
    if (targetFile == NULL) {
        printf("cannot create file at %s", cFileLocation);
        return 1;
    }
    fprintf(targetFile, "%s\n", copy1);
//    fprintf(targetFile, "%s\n", copy2);
    fflush(targetFile);
    for (j = 0; j < identifierCount; j++) {
        char declarationLine[N];
        if (identifierAttributes[j]->dim) {
            sprintf(declarationLine, "Matrix *_%s=(Matrix *) malloc(sizeof(Matrix));", identifierNames[j]);
            fprintf(targetFile, "\t%s\n", declarationLine);
            sprintf(declarationLine, "_%s->m = %d;", identifierNames[j], identifierAttributes[j]->dim1);
            fprintf(targetFile, "\t%s\n", declarationLine);
            sprintf(declarationLine, "_%s->n = %d;", identifierNames[j], identifierAttributes[j]->dim2);
            fprintf(targetFile, "\t%s\n", declarationLine);
            sprintf(declarationLine, "matrixInit(_%s);", identifierNames[j]);
            fprintf(targetFile, "\t%s\n", declarationLine);
        } else {
            sprintf(declarationLine, "float _%s = 0;", identifierNames[j]);
            fprintf(targetFile, "\t%s\n", declarationLine);
        }
    }

#pragma endregion

#pragma region evaluate remining statements
    for (; i < lineCount; i++) {
        char *currLine = strdup(allLines[i]);

        tokenizeLine(spacedString(currLine));

        char str1[N];
        str1[0] = 0;
        if (statement(str1)) {
            test(str1, "successfull statement\n", 0);
            fprintf(targetFile, "%s\n", str1);//successive calls appends the string
        }
        else{
            printf("Error (Line %d)\n",i+1);
            return 1;
        }
        free(currLine);
    }
#pragma endregion

    if(indentation){
        //not completed for loop
        printf("Error (Line %d)",lineCount);
        return 1;
    }
    fprintf(targetFile,"}\n");

    fclose(targetFile);
    return 0;
}

#pragma clang diagnostic pop

