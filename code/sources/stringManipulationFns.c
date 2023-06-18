#include <header.h>


/// \return new string with spaces before and after each special char,
char *spacedString(char *str) {
    int i, j = 0, count = 0;
    while (str[count++] != 0);
    char *result = (char *) malloc(count * 3 * sizeof(char));
    for (i = 0; i < count; i++) {
        if (isSpecialCharacter(str[i])) {
            if (i > 0) {
                if (str[i - 1] != ' ')
                    result[j++] = ' ';
            }
            result[j++] = str[i];
            if ((i < count - 2)) {
                if (!(str[i + 1] == ' ' || isSpecialCharacter(str[i + 1])))
                    result[j++] = ' ';
            }
        } else {
            result[j++] = str[i];
        }
    }
    result = (char *) realloc(result, j * sizeof(char));
    return result;
}

/// \return is ch one of the specified special characters
int isSpecialCharacter(char ch) {
    for (int i = 0; i < spCharCount; i++) {
        if (spChars[i] == ch)
            return 1;
    }
    return 0;
}

//put \0 on # in given string
void deleteComments(char *line) {
    int count = 0, i;
    while (line[count++] != 0);
    for (i = 0; i < count-1; i++) {
        if (line[i] == '#') {
            line[i] = 0;
        }
    }
}

//fills the tokens array with tokens generated from given string.
void tokenizeLine(char *line) {
    test(line, "next to tokenize",0);
    int i;
    for ( i = 0; i < tokenCount; i++) {

        if (tokens[i] != NULL) {
            free(tokens[i]);
            tokens[i] = NULL;
        }
    }
    tokenCount = 0;
    cur = 0;
    char *token;
    char **lp = &line;
    char *lineBeginning = line;
    while ((token = strtok_r(line, " \n\t", lp)) != NULL) {
        tokens[tokenCount++] = strdup(token);
    }
    testd(tokenCount,"tokenized, token count",0);
    if(tokenCount)test(tokens[0],"first token",0);
    free(lineBeginning);
}



void readAllLines(FILE *fp) {
    allLines = (char **) malloc(sizeof(char *) * NUM_LINES);
    char line[LINE_LENGTH];
    while (fgets(line, LINE_LENGTH, fp) != NULL) {
        if (lineCount == NUM_LINES) {
            tests("max num of lines reached",0);
            exit(1);
        }
        deleteComments(line);
        allLines[lineCount++] = strdup(line);
    }
    fclose(fp);
    allLines = (char **) realloc(allLines, sizeof(char *) * lineCount);
}


