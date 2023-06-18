#include <header.h>


//-1:assignment, 0:for, 1 print, 2: printsep, 3 end for
int statementType(char *firstToken) {
    char *types[] = {"for", "print", "printsep", "}"};
    int i;
    for (i = 0; i < LEN(types); i++) {
        if (strcmp(firstToken, types[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int statement(char *str) {
    printRemainingTokens("statement", 0);

    int stType = tokenCount ? statementType(tokens[0]) : 8, i;
    int dim, dim1, dim2;//for print statement
    char indent[5];

    strcpy(indent, "\t");
    for (i = 0; i < indentation; i++) {
        strcat(indent, "\t");
    }


    char str1[N];
    str1[0] = 0;
    switch (stType) {
        case 8://empty line
            return 1;
        case -1://assignment statement
            if (assignment(str1, 1)) {
                sprintf(str, "%s%s", indent, str1);
                return 1;
            }
            return 0;
        case 0://for
            if (indentation) {//not parse this
                tests("these kind of nested for loops are not allowed", 0);
                return 0;
            }
            if (forStmt(str1, 1)) {
                if (cur == tokenCount) {

                    strcpy(str, str1);
                    return 1;
                } else return 0;
            }
            return 0;
        case 1://print statement
            cur++;
            CUR_BOUND_CONTROL
            if (strcmp(tokens[cur], "(") != 0) {
                tests("parenthesis was expected after print", 0);
                return 0;
            }
            cur++;
            CUR_BOUND_CONTROL
            if (!expr(str1, &dim, &dim1, &dim2, 1)) {
                tests("print(??", 0);
                return 0;
            }
            CUR_BOUND_CONTROL
            if (strcmp(tokens[cur], ")") != 0) {
                tests("print(<expr>??", 0);
                return 0;
            }
            cur++;
            if (cur != tokenCount) {
                tests("print(<expr>)??", 0);
                return 0;
            }
            if (dim) {//expr is matrix
                sprintf(str, "%sprintM(%s);", indent, str1);
            } else {//expr is scalar
                sprintf(str, "%sprintS(%s);", indent, str1);
            }
            return 1;
        case 2:
            if (tokenCount != 3) {
                tests("printsep??", 0);
                return 0;
            }
            if (strcmp(tokens[1], "(") == 0 && strcmp(tokens[2], ")") == 0) {
                sprintf(str, "%sprintsep();", indent);
                return 1;
            }
            return 0;
        case 3:
            if (tokenCount != 1) {
                tests("end-of-for line should consist of exactly '}'", 0);
                return 0;
            }
            sprintf(str, "%s", indentation == 2 ? "\t\t}\n\t}" : "\n\t}");
            indentation = 0;
            return 1;
        default:
            return 0;
    }
}