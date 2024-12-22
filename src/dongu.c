/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */

#include "../include/dongu.h"
#include "../include/calistir.h"
#include "../include/split_commands.h"

/**
 * @brief Programın çalışmasını sağlayan döngüyü başlatır.
 */
void dongu(void)
{
    char *line = NULL;
    char **commands = NULL;
    char **args = NULL;
    int status = 1;

    do {
        printf(PROMPT);
        fflush(stdout);

        line = read_line();
        if (!line) {
            fprintf(stderr, "Error reading line\n");
            break;
        }

        commands = split_commands(line);
        if (!commands) {
            free(line);
            fprintf(stderr, "Error splitting commands\n");
            break;
        }

        for (int i = 0; commands[i] != NULL; i++) {
            args = split_whitespace(commands[i]);
            if (!args) {
                fprintf(stderr, "Error splitting command\n");
                continue;
            }

            if (args[0] != NULL && strcmp(args[0], "increment") == 0) {
                if (_increment(args) != 0) {
                    status = 0;
                    free(args);
                    break;
                }
            } else if (args[0] != NULL && strcmp(args[0], "quit") == 0) {
                status = 0;
                free(args);
                break;
            } else {
                calistir(args);
            }

            free(args);
        }

        free(line);
        free(commands);
    } while (status);
}
