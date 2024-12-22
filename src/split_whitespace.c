/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */
#include "../include/utils/split_whitespace.h"

#include <string.h>

/**
 * @brief String'i boşluklara göre ayırır ve döndürür.
 * @param line Ayırılacak string.
 * @return Boşluklara göre ayrılmış string dizisi.
 *
 * @warning Bu fonksiyon, bellek yönetimi için malloc ve realloc fonksiyonlarını
 * kullanır. Bu nedenle, kullanılan belleği serbest bırakmak için free
 * fonksiyonunu kullanmayı unutmayın.
 */

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