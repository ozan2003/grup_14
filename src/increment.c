#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/builtins/increment.h"

/**
 * @brief Aldığı sayıyı 1 arttıran fonksiyon.
 * @param args Komut ve argümanlarını içeren dizi.
 * @return 0: Başarıyla tamamlandı, -1: Hata.
 */
int _increment(char **args) {
    int num = 0;
    char buffer[256] = {0};
    FILE *file = NULL;

    // Case 1: Argument provided directly
    if (args[1] != NULL) {
        // Check if it's a file redirection
        if (strcmp(args[1], "<") == 0 && args[2] != NULL) {
            file = fopen(args[2], "r");
            if (!file) {
                fprintf(stderr, "Error opening file: %s\n", args[2]);
                return -1;
            }
            
            // Read first line from file
            if (fgets(buffer, sizeof(buffer), file) != NULL) {
                // Remove newline characters
                buffer[strcspn(buffer, "\r\n")] = '\0';
                num = atoi(buffer);
                fclose(file);
            } else {
                fprintf(stderr, "Error reading file or empty file\n");
                fclose(file);
                return -1;
            }
        } else {
            // Direct number argument
            num = atoi(args[1]);
        }
    } 
    // Case 2: No argument, read from stdin
    else {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\r\n")] = '\0';
            num = atoi(buffer);
        } else if (feof(stdin)) {
            return 0;
        } else {
            fprintf(stderr, "Error reading input\n");
            return -1;
        }
    }

    // Validate input
    if (num == 0 && buffer[0] != '0') {
        fprintf(stderr, "Invalid input: %s\n", buffer);
        return -1;
    }

    // Print incremented number
    printf("%d\n", num + 1);
    return 0;
}
