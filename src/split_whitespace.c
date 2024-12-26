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
 * @brief Bir stringi boşluklara göre ayırarak bir dizi şeklinde döndürür.
 *
 * @param line Ayırılacak string.
 * @return char** Boşluklara göre ayrılmış stringlerin dizisi.
 *
 * @warning Bu fonksiyon malloc ve realloc ile bellek tahsisi yapar. Ayrılan belleğin 
 * free fonksiyonu ile serbest bırakılması gerekir.
 */
char** split_whitespace(char* line)
{
    // Token dizisi için tampon boyutu ve dizi üzerindeki pozisyonu tanımla
    int    bufsize = TOKEN_BUFFER_SIZE, position = 0;
    
    // Ayrıştırılan stringleri tutmak için bellek tahsisi yap
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token; // Geçici token değişkeni
    char** tokens_backup; // Bellek yeniden tahsisi durumunda eski dizi için yedek

    // Bellek tahsisi kontrolü
    if (!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE); // Hata durumunda program sonlandırılır
    }

    // String'i boşluklara göre parçala (TOKEN_DELIMITERS ile ayrım)
    token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL)
    {
        // Ayrıştırılan token dizisine ekle
        tokens[position] = token;
        position++;

        // Dizi kapasitesi dolarsa tamponu genişlet
        if (position >= bufsize)
        {
            bufsize += TOKEN_BUFFER_SIZE; // Tampon boyutunu arttır
            tokens_backup = tokens; // Mevcut diziyi yedekle
            tokens = realloc(tokens, bufsize * sizeof(char*)); // Bellek yeniden tahsis edilir

            // Yeniden tahsis başarısızlığı durumunda eski belleği serbest bırak
            if (!tokens)
            {
                free(tokens_backup);
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        // Bir sonraki token'i al
        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    // Dizinin sonuna NULL ekle (sonlandırıcı)
    tokens[position] = NULL;
    return tokens; // Ayrıştırılmış string dizisini döner
}
