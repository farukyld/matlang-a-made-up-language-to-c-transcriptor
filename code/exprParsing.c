#include "header.h"

int expr(char *str, int *dim, int *dim1, int *dim2, int recDepth) {
    printRemainingTokens("expr",recDepth);

    CUR_BOUND_CONTROL // cannot search expr at the end of line

    char str1[N];//term
    char str2[N];//moreTerms
    str1[0] = str2[0] = 0;
    int moreTermsNull, td, td1, td2,/**/mtd, mtd1, mtd2, mts;
    if (!term(str1, &td, &td1, &td2, recDepth + 1)) {
        tests("no valid term", recDepth);
        return 0;
    }

    if (!moreTerms(str2, &mtd, &mtd1, &mtd2, &moreTermsNull, &mts, recDepth + 1)) {
        tests("no valid moreTerms", recDepth);
        return 0;
    }
    *dim = td;
    *dim1 = td1;
    *dim2 = td2;
    if (moreTermsNull) {
        tests("expr consist of one term", recDepth);
        sprintf(str, "%s", str1);
        return 1;
    }
    if (mtd != td || mtd1 != td1 || mtd2 != td2) {
        tests("expression terms dimension mismatch", recDepth);
        return 0;
    }
    test(str2, "expr.moreterms", recDepth);
    switch (*dim) {
#ifdef LEFT_ASSOC
        case 0:
            sprintf(str, mts ? "%s + %s" : "%s - %s", str1, str2);
            break;
#else
            case 0:
            sprintf(str, mts ? "scAdd(%s, %s)" : "scSub(%s, %s)", str1, str2);
            break;
#endif
        case 1:
        case 2:
            tests("______exprparsing42switchcase2 matsub kaldırılacak.", recDepth);
            sprintf(str, mts ? "matrixAdd(%s, %s)" : "matrixSub(%s, %s)", str1, str2);
            break;
        default:
            break;
    }
    return 1;
}

int term(char *str, int *dim, int *dim1, int *dim2, int recDepth) {
    printRemainingTokens("term",recDepth);
    CUR_BOUND_CONTROL // cannot search term at the end of line
    char str1[N];//factor
    char str2[N];//moreFactors
    int fd, fd1, fd2, mfd, mfd1, mfd2, mfn;
    str1[0] = str2[0] = 0;
    if (!factor(str1, &fd, &fd1, &fd2, recDepth + 1)) {
        tests("no valid factor", recDepth);
        return 0;
    }
    test(str1, "first factor ", recDepth);
    if (!moreFactors(str2, &mfd, &mfd1, &mfd2, &mfn, recDepth + 1)) {
        tests("no valid moreFactor", recDepth);
        return 0;
    }
    test(str2, "following factors ", recDepth);
    if (mfn) {
        *dim = fd;
        *dim1 = fd1;
        *dim2 = fd2;
        sprintf(str, "%s", str1);
        return 1;
    }
    if (fd == 0) {
        if (mfd != 0) {
            //scalar matrix mul
            *dim = mfd;
            *dim1 = mfd1;
            *dim2 = mfd2;
            sprintf(str, "scMatMul(%s, %s)", str1, str2);
        } else {
            *dim = 0;
            *dim1 = 0;
            *dim2 = 0;
            sprintf(str, "%s * %s", str1, str2);
        }
    }
    if (fd != 0) {
        if (mfd == 0) {
            *dim = fd;
            *dim1 = fd1;
            *dim2 = fd2;
            sprintf(str, "matScMul(%s, %s)", str1, str2);
        } else {
            *dim = 2;
            *dim1 = fd1;
            *dim2 = mfd2;
            if (fd2 != mfd1) {
                tests("matrix multiplication dimensions does not match", recDepth);
                return 0;
            }
            sprintf(str, "matMul(%s, %s)", str1, str2);
        }
    }
    return 1;
}

//sign is 0 if negative, 1 if positive
int moreTerms(char *str, int *dim, int *dim1, int *dim2, int *null, int *sign, int recDepth) {
    printRemainingTokens("moreTerms",recDepth);

    char str1[N];//term
    char str2[N];//moreTerms

    int td, td1, td2, mtd, mtd1, mtd2, mtn, mts;
    str1[0] = str2[0] = 0;

    if (cur < tokenCount) {//prevent the function to read out of range of tokens
        *null = 0;
        *sign = (strcmp(tokens[cur], "-") == 0) ? 0 : ((strcmp(tokens[cur], "+") == 0) ? 1 : -1);
        if ((*sign) != -1) {
            cur++;
            CUR_BOUND_CONTROL
            if (!term(str1, &td, &td1, &td2, recDepth + 1)) {
                testd(cur, "not valid term", recDepth);
                return 0;
            }

            if (!moreTerms(str2, &mtd, &mtd1, &mtd2, &mtn, &mts, recDepth + 1)) {
                testd(cur, "not valid moreTerms", recDepth);
                return 0;
            }
#ifdef LEFT_ASSOC
            if (!(*sign)) {//I am converting minus to a unary operator (in moreTerms nonterminal not anywhere else)for matrices
                if (td || mtd) {//since I don't use any function on scalar, just sequence them.
                    // so I am interested in moreTerms that evaluate matrices
                    char copyOfStr1[N / 2];
                    strcpy(copyOfStr1, str1);
                    sprintf(str1, "matScMul(%s,-1)", copyOfStr1);//and going to remove matSub.
                    //actually no need to remove matSub, just change the sign to plus to make sure it is not used
                    *sign = 1;
                }
            }
#endif
        } else {
            *null = 1;
            return 1;
        }
    } else {
        *null = 1;
        return 1;
    }
    *dim = td;
    *dim1 = td1;
    *dim2 = td2;
    if (mtn) {
        sprintf(str, "%s", str1);
        return 1;
    }
    if (td != mtd || td1 != mtd1 || td2 != mtd2) {
        tests("dimension mismatch among terms", recDepth);
        return 0;
    }
    switch (td) {
#ifdef LEFT_ASSOC
        case 0:
            sprintf(str, mts ? "%s + %s" : "%s - %s", str1, str2);
            break;
#else
        case 0:
            sprintf(str, mts ? "scAdd(%s, %s)" : "scSub(%s, %s)", str1, str2);
            break;
#endif
        case 1:
        case 2:
            sprintf(str, mts ? "matrixAdd(%s, %s)" : "matrixSub(%s, %s)", str1, str2);
            break;
        default:
            break;
    }
    return 1;
}

int moreFactors(char *str, int *dim, int *dim1, int *dim2, int *null, int recDepth) {
    printRemainingTokens("moreFactors",recDepth);

    char str1[N];//factor
    char str2[N];//moreFactor

    str1[0] = str2[0] = 0;

    int fd, fd1, fd2, mfd, mfd1, mfd2, mfn;
    if (cur < tokenCount) { // prevent the function to read out of range of tokens
        if ((strcmp(tokens[cur], "*") == 0)) {
            tests("multiplication following", recDepth);
            cur++;
            CUR_BOUND_CONTROL
            if (!factor(str1, &fd, &fd1, &fd2, recDepth + 1)) {
                tests("not valid factor", recDepth);
                return 0;
            }
            test(str1, "factor", recDepth);
            if (!moreFactors(str2, &mfd, &mfd1, &mfd2, &mfn, recDepth + 1)) {
                tests("not valid moreFactors", recDepth);
                return 0;
            }
            *null = 0;
        } else {
            *null = 1;
            return 1;
        }
        *null = 0;
    } else {
        *null = 1;
        return 1;
    }
    if (mfn) {
        *dim = fd;
        *dim1 = fd1;
        *dim2 = fd2;
        sprintf(str, "%s", str1);
        return 1;
    }
    if (fd == 0) {
        if (mfd != 0) {
            //scalar matrix mul
            *dim = mfd;
            *dim1 = mfd1;
            *dim2 = mfd2;
            sprintf(str, "scMatMul(%s, %s)", str1, str2);
        } else {
            *dim = 0;
            *dim1 = 0;
            *dim2 = 0;
            sprintf(str, "%s * %s", str1, str2);
        }
    }
    if (fd != 0) {
        if (mfd == 0) {
            *dim = fd;
            *dim1 = fd1;
            *dim2 = fd2;
            sprintf(str, "matScMul(%s, %s)", str1, str2);
        } else {
            *dim = 2;
            *dim1 = fd1;
            *dim2 = mfd2;
            if (fd2 != mfd1) {
                tests("matrix multiplication dimensions does not match", recDepth);
                return 0;
            }
            sprintf(str, "matMul(%s, %s)", str1, str2);
        }
    }
    return 1;
}

int factor(char *str, int *dim, int *dim1, int *dim2, int recDepth) {
    printRemainingTokens("factor",recDepth);

    CUR_BOUND_CONTROL // cannot search factor at the end of line
    char str1[N];
    char str2[N];
    char str3[N];
    char str4[N];
    str1[0] = str2[0] = str3[0] = str4[0] = 0;
    int
            e1d, e1d1, e1d2,
            e2d, e2d1, e2d2,
            e3d, e3d1, e3d2,
            e4d, e4d1, e4d2,
            idd, idd1, idd2;

    if (scalar(str1, recDepth + 1)) {
        sprintf(str, "%s", str1);
        (*dim) = (*dim1) = (*dim2) = 0;
        tests("returning scalar", recDepth);
        return 1;
    }
    if (strcmp(tokens[cur], "tr") == 0) {
        cur++;
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], "(") != 0) {
            tests("expecting '(' after tr", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(str1, &e1d, &e1d1, &e1d2, recDepth + 1)) {
            tests("expecting expr", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ")") != 0) {
            tests("expecting )", recDepth);
            return 0;
        }
        cur++;
        //succesfull tr call.
        *dim = e1d;
        *dim1 = e1d2;
        *dim2 = e1d1;
        sprintf(str, e1d ? "trM(%s)" : "trS(%s)", str1);
        return 1;
    }
    if (strcmp(tokens[cur], "sqrt") == 0) {
        cur++;
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], "(") != 0) {
            tests("expecting '(' after sqrt", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(str1, &e1d, &e1d1, &e1d2, recDepth + 1)) {
            tests("expecting expr", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ")") != 0) {
            tests("expecting )", recDepth);
            return 0;
        }
        cur++;
        //succesfull sqrt call.
        *dim = 0;
        *dim1 = 0;
        *dim2 = 0;
        if (e1d) {
            tests("expecting scalar value in sqrt fn", recDepth);
            return 0;
        }
        sprintf(str, "sqrtf(%s)", str1);
        return 1;
    }
    if (strcmp(tokens[cur], "choose") == 0) {
        cur++;
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], "(") != 0) {
            tests("expecting '(' after choose", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(str1, &e1d, &e1d1, &e1d2, recDepth + 1)) {
            tests("expecting expr", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ",") != 0) {
            tests("expecting ,", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(str2, &e2d, &e2d1, &e2d2, recDepth + 1)) {
            tests("expecting expr", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ",") != 0) {
            tests("expecting ,", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(str3, &e3d, &e3d1, &e3d2, recDepth + 1)) {
            tests("expecting expr", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ",") != 0) {
            tests("expecting ,", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(str4, &e4d, &e4d1, &e4d2, recDepth + 1)) {
            tests("expecting expr", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ")") != 0) {
            tests("expecting )", recDepth);
            return 0;
        }
        cur++;
        *dim = 0;
        *dim1 = 0;
        *dim2 = 0;
        if (e1d || e2d || e3d || e4d) {
            tests("expecting scalar value in choose fn", recDepth);
            return 0;
        }
        sprintf(str, "choose(%s, %s, %s, %s)", str1, str2, str3, str4);
        return 1;
    }
    if (strcmp(tokens[cur], "(") == 0) {
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(str1, &e1d, &e1d1, &e1d2, recDepth + 1)) {
            tests("expecting expr inside (", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ")") != 0) {
            tests("expecting )", recDepth);
            return 0;
        }
        cur++;
        *dim = e1d;
        *dim1 = e1d1;
        *dim2 = e1d2;
        sprintf(str, "(%s)", str1);
        return 1;
    }
    if (id_optionallyIndexed(str1, &idd, &idd1, &idd2, recDepth + 1)) {
        *dim = idd;
        *dim1 = idd1;
        *dim2 = idd2;
        strcpy(str, str1);
        return 1;
    }
    return 0;
}
