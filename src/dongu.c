#include "../include/dongu.h"
#include "../include/calistir.h"

/**
 * @brief Programın çalışmasını sağlayan döngüyü başlatır.
 */
void dongu(void)
{
    char* line;
    char** args;
    int status;
    
    do
    {
        printf(PROMPT);
        fflush(stdout);

        line = read_line();
        args = split_whitespace(line);

        // TODO: Girilen komutlar burada çalıştırılacak. Düzenleme lazım olabilir.
        calistir(args);

        free(line);
        free(args);
    } while (status);
    
}