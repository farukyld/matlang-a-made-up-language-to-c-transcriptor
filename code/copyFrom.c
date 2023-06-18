#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "stddef.h"

typedef struct Matrix {
    int m, n;
    float **value;
} Matrix;

void matrixInit(Matrix *m) {
    int i, j;
    m->value = (float **) malloc(sizeof(float *) * (m->m));
    for (i = 0; i < m->m; i++) {
        m->value[i] = (float *) malloc(sizeof(float) * (m->n));
        for (j = 0; j < m->n; j++) {
            m->value[i][j] = 0;
        }
    }
}

Matrix *matrixSub(Matrix *a, Matrix *b) {
    int m = a->m, n = a->n, i, j;
    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    res->m = m;
    res->n = n;
    matrixInit(res);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            res->value[i][j] = a->value[i][j] - b->value[i][j];
        }
    }
    return res;
}

Matrix *matrixAdd(Matrix *a, Matrix *b) {
    int m = a->m, n = a->n, i, j;
    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    res->m = m;
    res->n = n;
    matrixInit(res);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            res->value[i][j] = a->value[i][j] + b->value[i][j];
        }
    }
    return res;
}

Matrix *matMul(Matrix *a, Matrix *b) {
    int m = a->m, n = b->n, o = a->n, i, j, k;
    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    res->m = m;
    res->n = n;
    matrixInit(res);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < o; k++) {
                res->value[i][j] += a->value[i][k] * b->value[k][j];
            }
        }
    }
    return res;
}

Matrix *matScMul(Matrix *a, float sc) {
    int m = a->m, n = a->n, i, j;
    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    res->m = m;
    res->n = n;
    matrixInit(res);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            res->value[i][j] = a->value[i][j] * sc;
        }
    }
    return res;
}

Matrix *scMatMul(float sc, Matrix *a) {
    return matScMul(a, sc);
}

void assignMM(Matrix *lhs, Matrix *rhs) {
    int m = lhs->m, n = lhs->n, i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            lhs->value[i][j] = rhs->value[i][j];
        }
    }
}

void assignML(Matrix *lhs, char *floatList) {

    int m = lhs->m, n = lhs->n, i, j, k = 0;
    char *floatString;

    char *mallocced = (char *) malloc((strlen(floatList) + 1) * sizeof(char));
    strcpy(mallocced, floatList);
    char **flp = &mallocced;

    char *floatTokens[m * n];
    for (i = 0; i < m * n; i++) {
        if ((floatString = strtok_r(mallocced, " ", flp)) != NULL) {
            floatTokens[j++] = floatString;
        }
    }

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            lhs->value[i][j] = atof(floatTokens[k++]);
        }
    }
}

Matrix *trM(Matrix *a) {
    int m = a->m, n = a->n, i, j;
    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    res->m = n;
    res->n = m;
    matrixInit(res);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            res->value[j][i] = a->value[i][j];
        }
    }
    return res;
}

float trS(float a) {
    return a;
}

float choose(float selector, float zero, float pos, float neg) {
    return selector ? (selector > 0 ? pos : neg) : zero;
}

void printsep() {
    //            0123456789012
    char sep[] = "-------------\n";
    printf("%s", sep);
}

void floatToStr(char *dest, float f) {
    if ((int) f == f) {
        sprintf(dest, "%d", (int) f);
    } else {
        sprintf(dest, "%f", f);
    }
}

void printS(float s) {
    char floatStr[9];
    floatToStr(floatStr, s);
    printf("%s\n", floatStr);
}

void printM(Matrix *a) {
    int n = a->n, m = a->m, i, j;
    char *out = (char *) malloc((m * n * 12 + n + 50) * sizeof(char));
    char *beginning;
    out[0] = 0;
    beginning = out;
    char step[12];
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            floatToStr(step, a->value[i][j]);
            out += sprintf(out, "%12s", step);
        }
        out += sprintf(out, "\n");
    }
    printf("%s", beginning);
}

float scAdd(float a, float b) {
    return a + b;
}

float scSub(float a, float b) {
    return a - b;
}

int main() {
//start appending here

}