/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */

#include "../include/utils/read_line.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

 /**
  * @brief Standart girdiden bir satır okuyup geri döndüren fonksiyon.
  *
  * Bu fonksiyon, karakter karakter okuyarak standart girdiden tam bir satır alır.
  * Girdi boyutunun tamponu aştığı durumlarda bellek realloc ile otomatik olarak artırılır.
  *
  * @return char* Okunan tam satırı içeren bir string döner. Sonunda NULL karakteri bulunur.
  * @warning Dönen string belleğin free() ile temizlenmesi gereklidir.
  */
char* read_line(void)
{
    // Tampon boyutunu ve okuma pozisyonunu belirle
    int   bufsize = BUFFER_SIZE; // Başlangıçta tahsis edilen tampon boyutu
    int   position = 0;         // Tampona yazılacak pozisyon
    char* buffer = malloc(sizeof(char) * bufsize); // Tampon için dinamik bellek tahsisi
    char  ch;                  // Okunan karakterin tutulduğu değişken

    // Bellek tahsisi başarı kontrolü
    if (!buffer)
    {
        fprintf(stderr, "Bellek ayırılırken hata oluştu\n");
        exit(EXIT_FAILURE); // Bellek ayırılamadıysa program sonlandırılır
    }

    while (true)
    {
        // Standart girdiden bir karakter oku
        ch = getchar();

        // Eğer EOF veya yeni satır karakteri geldiyse işlemi bitir
        if (ch == EOF || ch == '\n')
        {
            buffer[position] = '\0'; // Stringi sonlandırmak için NULL karakteri ekle
            return buffer; // Tamamlanan stringi geri dön
        }
        else
        {
            buffer[position] = ch; // Okunan karakteri tampona yaz
        }
        position++;

        // Tampon kapasitesi dolarsa daha fazla bellek tahsis et
        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE; // Tampon boyutunu artır
            buffer = realloc(buffer, bufsize); // Belleği yeniden tahsis et

            // Yeniden tahsis başarı kontrolü
            if (!buffer)
            {
                fprintf(stderr, "Bellek yeniden ayırılamadı\n");
                exit(EXIT_FAILURE); // Yeniden tahsis hatasında program sonlandırılır
            }
        }
    }

    return buffer; // Bu satır normal şartlarda işlenmez
}