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
  * @brief Verilen bir stringi boşluk karakterlerine göre parçalayıp bir dizi şeklinde geri döndüren fonksiyon.
  *
  * Bu fonksiyon, verilen stringi boşluklarla ayırarak her bir parçayı bir dizi elemanı olarak saklar.
  * Girdi stringinde bulunan her bir kelime veya parça, diziye eklenir.
  *
  * @param line Parçalanacak string.
  * @return char** Ayrıştırılmış kelimeleri veya parçaları içeren bir dizi döner.
  * @warning Bu fonksiyon malloc ve realloc ile bellek tahsisi yapar. Ayrılan belleğin free() fonksiyonu ile
  * serbest bırakılması gerekir.
  */
char** split_whitespace(char* line)
{
    // Tampon boyutu ve dizi üzerindeki pozisyonu tanımla
    int    bufsize = TOKEN_BUFFER_SIZE, position = 0;

    // Ayrıştırılan stringleri tutacak dizi için bellek tahsisi
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token; // Geçici değişken, bir kelimeyi saklar
    char** tokens_backup; // Bellek tahsisinde hata durumunda eski dizi için yedek

    // Bellek tahsisi kontrolü
    if (!tokens)
    {
        fprintf(stderr, "Bellek tahsisi hatası\n");
        exit(EXIT_FAILURE); // Hata durumunda program sonlandırılır
    }

    // String'i boşluklara göre parçala (TOKEN_DELIMITERS kullanılır)
    token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL)
    {
        // Ayrıştırılan kelimeyi dizide sakla
        tokens[position] = token;
        position++;

        // Dizi kapasitesini kontrol et ve gerekirse genişlet
        if (position >= bufsize)
        {
            bufsize += TOKEN_BUFFER_SIZE; // Tampon boyutunu artır
            tokens_backup = tokens; // Mevcut diziyi yedekle
            tokens = realloc(tokens, bufsize * sizeof(char*)); // Belleği genişlet

            // Bellek yeniden tahsis başarı kontrolü
            if (!tokens)
            {
                free(tokens_backup); // Eski belleği serbest bırak
                fprintf(stderr, "Bellek tahsisi başarısız\n");
                exit(EXIT_FAILURE); // Programı sonlandır
            }
        }

        // Sonraki kelimeyi al
        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    // Diziye sonlandırıcı olarak NULL ekle
    tokens[position] = NULL;
    return tokens; // Ayrıştırılmış string dizisini geri dön
}