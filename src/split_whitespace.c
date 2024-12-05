#include "../include/utils/split_whitespace.h"

#include <string.h>

char** split_whitespace(char* line)
{
    int    bufsize = TOKEN_BUFFER_SIZE, position = 0;
    char** tokens = malloc((bufsize) * sizeof(char*));
    char * token, **tokens_backup;

    if (!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += TOKEN_BUFFER_SIZE;
            tokens_backup = tokens;
            tokens        = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                free(tokens_backup);
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}