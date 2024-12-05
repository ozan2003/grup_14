#include "../include/dongu.h"
#include "../include/calistir.h"

/**
 * @brief Programın çalışmasını sağlayan döngüyü başlatır.
 */
void dongu(void)
{
    char*  line;
    char** args;
    int    status = 1; // Initialize status to run the loop

    do
    {
        printf(PROMPT);
        fflush(stdout);

        // Read and parse the input line
        line = read_line();
        args = split_whitespace(line);

        // Check for the "quit" command
        if (args[0] != NULL && strcmp(args[0], "quit") == 0)
        {
            status = 0; // Set status to 0 to exit the loop
        }
        else
        {
            // Run the command
            calistir(args);
        }

        // Free allocated memory
        free(line);
        free(args);
    } while (status); // Loop while status is 1
}
