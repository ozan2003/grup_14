#include "../include/builtins/increment.h"

/**
 * @brief Aldığı sayıyı 1 arttıran fonksiyon.
 * @param args Komut ve argümanlarını içeren dizi.
 */
int _increment(char** args)
{
    // Check if we have input
    if (args[1] == NULL)
    {
        fprintf(stderr, "Increment requires a number\n");
        return -1;
    }

    // Convert input to integer
    int num = atoi(args[1]);

    // Print incremented number
    printf("%d\n", num + 1);

    return 0;
}