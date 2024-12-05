#include "../include/dongu.h"

int main()
{
    dongu();

    return 0;
    /*printf(PROMPT);
    fflush(stdout);

    char*  line   = read_line();
    char** tokens = split_whitespace(line);

    for (int i = 0; tokens[i] != NULL; i++)
    {
        printf("%d-%s\n",i, tokens[i]);
    }

    free(tokens);
    free(line);

    return 0;*/
}