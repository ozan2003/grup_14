#include "../include/builtins/increment.h"

/**
 * @brief Aldığı sayıyı 1 arttıran fonksiyon.
 * @param args Komut ve argümanlarını içeren dizi.
 */
int _increment(char** args)
{
    // If there's no number passed as an argument, we expect input from stdin
    int num = 0;

    if (args[1] != NULL)
    {
        num = atoi(args[1]); // Use the argument if available
    }
    else
    {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            num = atoi(
                buffer); // Read input from stdin (output of previous command)
        }
        else
        {
            fprintf(stderr, "Error reading input\n");
            return -1;
        }
    }

    // Print the incremented number
    printf("%d\n", num + 1);
    return 0;
}
