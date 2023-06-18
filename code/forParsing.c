#include "header.h"


int forStmt(char *str, int recDepth) {
    printRemainingTokens("forStmt", recDepth);

    char sid1[N];//id
    char sid2[N];//id
    char se1[N];//exp1
    char se2[N];//exp2
    char se3[N];//exp3
    char se4[N];//exp4
    char se5[N];//exp5
    char se6[N];//exp6
    sid1[0] = sid2[0] = se4[0] = se5[0] = se6[0] = se1[0] = se2[0] = se3[0] = 0;
    int
            i1d, i1d1, i1d2,
            i2d, i2d1, i2d2,
            e1d, e1d1, e1d2,
            e2d, e2d1, e2d2,
            e3d, e3d1, e3d2,
            e4d, e4d1, e4d2,
            e5d, e5d1, e5d2,
            e6d, e6d1, e6d2;

    if (strcmp(tokens[cur], "for") != 0) {//actually this was controlled before entering here, but anyway, just put it.
        return 0;
    }
    cur++;
    CUR_BOUND_CONTROL //for stmt line cannot end with for
    if (strcmp(tokens[cur], "(") != 0) {
        tests("( expected after for", recDepth);
        return 0;
    }
    cur++;
    CUR_BOUND_CONTROL //for stmt line cannot end with (
    if (!id(sid1, &i1d, &i1d1, &i1d2, recDepth + 1)) {
        tests("id expected ", recDepth);
        return 0;
    }
    CUR_BOUND_CONTROL //for stmt line cannot end with id
    if (strcmp(tokens[cur], ",") == 0) {//nested for statement
        cur++;
        CUR_BOUND_CONTROL //for stmt line cannot end with ,
        if (!id(sid2, &i2d, &i2d1, &i2d2, recDepth + 1)) {
            tests("id expected ", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL //for stmt line cannot end with id
        if (strcmp(tokens[cur], "in") != 0) {
            tests("in expected", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL //for stmt line cannot end with in
        if (!expr(se1, &e1d, &e1d1, &e1d2, recDepth + 1)) {
            tests("expr expected", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL //for stmt line cannot end with expr
        if (strcmp(tokens[cur], ":") != 0) {
            tests("':' expected", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL //for stmt line cannot end with :
        if (!expr(se2, &e2d, &e2d1, &e2d2, recDepth + 1)) {
            tests("expr expected", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL //for stmt line cannot end with expr
        if (strcmp(tokens[cur], ":") != 0) {
            tests("':' expected", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL//for stmt line cannot end with :
        if (!expr(se3, &e3d, &e3d1, &e3d2, recDepth + 1)) {
            tests("expr expected", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ",") != 0) {
            tests("',' expected", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(se4, &e4d, &e4d1, &e4d2, recDepth + 1)) {
            tests("expr expected", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ":") != 0) {
            tests("':' expected", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL//for stmt line cannot end with :
        if (!expr(se5, &e5d, &e5d1, &e5d2, recDepth + 1)) {
            tests("expr expected", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL//for stmt line cannot end with :
        if (strcmp(tokens[cur], ":") != 0) {
            tests("':' expected", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (!expr(se6, &e6d, &e6d1, &e6d2, recDepth + 1)) {
            tests("expr expected", recDepth);
            return 0;
        }
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], ")") != 0) {
            tests("')' expected", recDepth);
            return 0;
        }
        cur++;
        CUR_BOUND_CONTROL
        if (strcmp(tokens[cur], "{") != 0) {
            tests("'{' expected", recDepth);
            return 0;
        }
        cur++;
        if (cur != tokenCount) {
            tests("unrecognizable tokens at the end of the line", recDepth);
            return 0;
        }
        //succesfull simple for statement
        //dimension check
        if (i1d || e1d || e2d || e3d || i2d || e4d || e5d || e6d) {
            tests("all loop variants should be scalar", recDepth);
            return 0;
        }
        sprintf(str, "\tfor ( %s = %s; %s < %s; %s += %s) {\n"//      i   = exp1 ; i < exp2 ; i += e3
                     "\t\tfor ( %s = %s; %s < %s; %s += %s) {", sid1, se1, sid1, se2, sid1, se3,
                //  j = exp4; j   < exp5 ;j += exp6
                /**/sid2, se4, sid2, se5, sid2, se6);
        indentation = 2;
        return 1;
    }
    if (strcmp(tokens[cur], "in") != 0) {
        tests("in expected", recDepth);
        return 0;
    }
    cur++;
    CUR_BOUND_CONTROL
    if (!expr(se1, &e1d, &e1d1, &e1d2, recDepth + 1)) {
        tests("expr expected", recDepth);
        return 0;
    }
    CUR_BOUND_CONTROL
    if (strcmp(tokens[cur], ":") != 0) {
        tests("':' expected", recDepth);
        return 0;
    }
    cur++;
    CUR_BOUND_CONTROL//for stmt line cannot end with :
    if (!expr(se2, &e2d, &e2d1, &e2d2, recDepth + 1)) {
        tests("expr expected", recDepth);
        return 0;
    }
    CUR_BOUND_CONTROL
    if (strcmp(tokens[cur], ":") != 0) {
        tests("':' expected", recDepth);
        return 0;
    }
    cur++;
    CUR_BOUND_CONTROL//for stmt line cannot end with :
    if (!expr(se3, &e3d, &e3d1, &e3d2, recDepth + 1)) {
        tests("expr expected", recDepth);
        return 0;
    }
    CUR_BOUND_CONTROL
    if (strcmp(tokens[cur], ")") != 0) {
        tests("')' expected", recDepth);
        return 0;
    }
    cur++;
    CUR_BOUND_CONTROL
    if (strcmp(tokens[cur], "{") != 0) {
        tests("'{' expected", recDepth);
        return 0;
    }
    cur++;
    if (cur != tokenCount) {
        tests("unrecognizable tokens at the end of the line", recDepth);
        return 0;
    }
    //succesfull simple for statement
    //dimension check
    if (i1d || e1d || e2d || e3d) {
        tests("all loop variants should be scalar", recDepth);
        return 0;
    }
    sprintf(str, "\tfor ( %s = %s; %s < %s; %s += %s) {", sid1, se1, sid1, se2, sid1, se3);
    indentation = 2;
    return 1;
}