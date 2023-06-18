#include <header.h>

int isInteger(char *token) {
    int isnumber = 1;
    char *q;

    for (q = token; *q != 0; q++) {
        isnumber = isnumber && isdigit(*q);
    }

    return (isnumber);
}

int scalar(char *str, int recDepth) {
    printRemainingTokens("scalar",recDepth);
    CUR_BOUND_CONTROL //cannot search scalar at end of line
    int curSave=cur;
    if (!isInteger(tokens[cur])) {
        tests("scalar returning 0",recDepth);
        return 0;
    }
    strcpy(str, tokens[cur]);
    cur++;
    if (cur == tokenCount)
        return 1;
    if (strcmp(tokens[cur], ".") == 0) {
        strcat(str, tokens[cur]);
        if(cur+1==tokenCount){tests("end of line reached",recDepth);return 0;}
        if (!isInteger(tokens[cur + 1])) {
            tests("scalar returning 0 after '.'",recDepth);
            cur=curSave;
            return 0;
        }
        cur++;
        strcat(str, tokens[cur]);
        strcat(str, "f");
        cur++;
        return 1;//scalar with decimal point
    }
    //scalar without decimal point
    return 1;
}

int nums(char *str, int *numCount, int recDepth) {
    printRemainingTokens("nums",recDepth);

    char str1[N], str2[N];
    str1[0] = str2[0] = 0;
    if (cur < tokenCount) {
        if (scalar(str1, recDepth + 1)) {
            (*numCount)++;
            strcat(str1, " ");
            if (nums(str2, numCount, recDepth + 1)) {
                strcat(str1, str2);
                strcpy(str, str1);
                return 1;
            }
        }
    }
    //nums can be null. which is to be assigned to an array of size 0
    strcpy(str, str1);
    return 1;
}

int numList(char *str, int *numCount, int recDepth) {
    printRemainingTokens("numList",recDepth);

    char str1[N];
    str1[0] = 0;
    CUR_BOUND_CONTROL // cannot search numList at the end of line
    if (strcmp(tokens[cur], "{") == 0) {
        cur++;
        CUR_BOUND_CONTROL
        if (!nums(str1, numCount, recDepth + 1)) {//no probability for nums to return false
            tests("{ should be followed by number", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], "}") != 0) {
            tests("num list should end with }", recDepth);
            return 0;
        }
        cur++;
        sprintf(str, "\" %s\"", str1);
        return 1;
    }
    return 0;
}

//return true if id exist. str as _<matCodeIdName> dim, dim1, dim2 as recorded before.
int id(char *str, int *dim, int *dim1, int *dim2, int recDepth) {
    printRemainingTokens("id",recDepth);
    CUR_BOUND_CONTROL//cannot search id at end of line
    int index = getIndex(tokens[cur]);

    if (index + 1) {//it means id is valid. if id hasn't been declared, index is -1
        sprintf(str, "_%s", tokens[cur]);
        cur++;
        *dim = identifierAttributes[index]->dim;
        *dim1 = identifierAttributes[index]->dim1;
        *dim2 = identifierAttributes[index]->dim2;
        return 1;
    }
    test(tokens[cur], "id does not exist", recDepth);

    return 0;
}

//dim is 0 if id_optionallyIndexed is scalar or is indexed matrix or vector.
//dim is 1, 2 if id_optionallyIndexed is vector, matrix id resp.
//dim1 and dim2 are dimensions of id if id_optionallyIndexed is id other than scalar.
int id_optionallyIndexed(char *str, int *dim, int *dim1, int *dim2, int recDepth) {
    printRemainingTokens("id_optionallyIndexed",recDepth);

    CUR_BOUND_CONTROL //cannot search id_optionallyIndexed at end of line

    (*dim) = (*dim1) = (*dim2) = 0;
    //id_optionallyIndexed is id or id[expr] or id[expr,expr]

    char str1[N];//output of id
    char str2[N];//output of expr1 if exists
    char str3[N];//output of expr2 if exists
    str1[0] = str2[0] = str3[0] = 0;
    int idDim, idDim1, idDim2,/* */e1d = 0, e1d1, e1d2,/* */e2d = 0, e2d1, e2d2;
    if (!id(str1, &idDim, &idDim1, &idDim2, recDepth + 1)) {
        tests("id_optionallyIndexed should start with an id", recDepth);
        return 0;
    }
    if (cur < tokenCount) {
        if (strcmp(tokens[cur], "[") == 0) {
            test(str1, "indexing", recDepth);
            //an expr must follow.
            cur++;
            CUR_BOUND_CONTROL
            if (!expr(str2, &e1d, &e1d1, &e1d2, recDepth + 1)) {
                tests("[ symbol should be followed by an expr", recDepth);
                return 0;
            }
            //should be followed by ']' or ','
            if (strcmp(tokens[cur], "]") == 0) {//vector indexation
                cur++;//due to ']'
                *dim = 0;
                *dim1 = 0;
                *dim2 = 0;
                if (e1d) {
                    tests("indices should be scalar", recDepth);
                    return 0;
                }
                if (idDim != 1) {
                    tests("identifier must be type of vector", recDepth);
                    return 0;
                }
                sprintf(str, "%s->value[(int)(%s)-1][0] ", str1, str2);
                return 1;
            }
            if (strcmp(tokens[cur], ",") != 0) {
                tests("not valid indexation: [<expr>??", recDepth);
                return 0;
            }
            cur++;//due to ','
            CUR_BOUND_CONTROL
            if (!expr(str3, &e2d, &e2d1, &e2d2, recDepth + 1)) {
                tests("not valid indexation: [<expr>,??", recDepth);
                return 0;
            }
            if (strcmp(tokens[cur], "]") != 0) {
                tests("not valid indexation: [<expr>,<expr>??", recDepth);
                return 0;
            }
            cur++;//due to ']'
            //matrix indexation
            *dim = 0;
            *dim1 = 0;
            *dim2 = 0;
            if (e1d || e2d) {//they both should be scalar i.e. dimension 0
                tests("indices should be scalar", recDepth);
                return 0;
            }
            if (idDim != 2) {
                tests("identifier must be type of matrix", recDepth);
                return 0;
            }

            sprintf(str, "%s->value[(int)(%s) - 1][(int)(%s) - 1] ", str1, str2, str3);
            return 1;
        }
    }
    //id_optionallyIndexed consist of only an id, no indexation
    test(str1, "alone id", recDepth);
    *dim = idDim;
    *dim1 = idDim1;
    *dim2 = idDim2;
    strcpy(str, str1);
    return 1;
}

int assignment(char *str, int recDepth) {
    printRemainingTokens("assignment",recDepth);
    CUR_BOUND_CONTROL
    char str1[N], str2[N];
    str1[0] = str2[0] = 0;
    int expDim, expDim1, expDim2;//id_optionallyIndexed of the assignment will determine these values.
    int dim, dim1, dim2, numCount = 0;//rhs will determine
    if (!id_optionallyIndexed(str1, &expDim, &expDim1, &expDim2, recDepth + 1)) {
        return 0;
    }
    CUR_BOUND_CONTROL
    if (strcmp(tokens[cur], "=") != 0) {
        return 0;
    }
    cur++;
    CUR_BOUND_CONTROL
    if (numList(str2, &numCount, recDepth + 1)) {
        test(str2, "numListDonus", recDepth);
        switch (expDim) {//dimension check
            case 0:
                tests("rhs of assignment does not evaluates to a scalar", recDepth);
                return 0;
            case 1:
                if (numCount != expDim1) {//dimension mismatch with vector
                    tests("vector dimension doesn't match", recDepth);
                    return 0;
                }//otherwise : str:= _str1.value = str2
                break;
            case 2:
                if (numCount != (expDim1 * expDim2)) {//dim mismatch with matrix
                    test(str1, "matrix dimension doesn't match ", recDepth);
                    testd(numCount, "nc", recDepth);
                    testd(expDim1, "ed1", recDepth);
                    testd(expDim2, "ed2", recDepth);

                    return 0;
                }//otherwise : str:= _str1.value = str2
                break;
            default:
                break;
        }
        sprintf(str, "assignML(%s,%s);", str1, str2);
        if (cur == tokenCount) {//last token of the line must have read.
            return 1;
        } else {
            tests("assignment line includes non recognizable tokens", recDepth);
            return 0;
        }
    }

    if (expr(str2, &dim, &dim1, &dim2, recDepth + 1)) {
        //check exceptional cases in terms of dimensions.
        switch (expDim) {
            case 0://id_optionallyIndexed is a scalar
                if (dim != 0) {
                    tests("scalar expected", recDepth);
                    return 0;
                }//otherwise : str:= str1 = str2
                break;
            case 1://id_optionallyIndexed is a vector
                if (dim == 0 || dim1 != expDim1 || 1 != expDim2) {
                    tests("rhs dimension must match with id_optionallyIndexed", recDepth);
                    return 0;
                }//otherwise : str:= str1.value = str2
                break;
            case 2://id_optionallyIndexed is a matrix
                if (dim == 0 || dim1 != expDim1 || dim2 != expDim2) {
                    tests("rhs dimension must match with id_optionallyIndexed", recDepth);
                    return 0;
                }//otherwise : str:= str1.value = str2
                break;
            default:
                break;
        }

        sprintf(str, dim ? "assignMM(%s, %s);" : "%s = %s;", str1, str2);

        if (cur == tokenCount) {//last token of the line must have been read.
            return 1;
        } else {
            tests("assignment line includes non recognizable tokens", recDepth);
            return 0;
        }
    }

    return 0;
}
