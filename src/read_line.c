#include "../include/utils/read_line.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Bir satır okur ve döndürür.
 * @return Okunan satır.
 *
 * @warning Bu fonksiyon, bellek yönetimi için malloc ve realloc fonksiyonlarını
 * kullanır. Bu nedenle, kullanılan belleği serbest bırakmak için free
 * fonksiyonunu kullanmayı unutmayın.
 */
char* read_line(void)
{
    int   bufsize  = BUFFER_SIZE;
    int   position = 0;
    char* buffer   = malloc(sizeof(char) * bufsize);
    char  ch;

    if (!buffer)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        // Read a character
        ch = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (ch == EOF || ch == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = ch;
        }
        position++;

        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer)
            {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return buffer;
}