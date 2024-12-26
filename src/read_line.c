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
 * @brief Kullanıcıdan bir satır girdi alır ve string olarak döner.
 *
 * Bu fonksiyon, standart girdiden karakter karakter okuyarak tam bir satırı alır.
 * Bellek yönetimi malloc ve realloc ile yapılır. Girdi boyutu tamponu aştığında
 * bellek otomatik olarak genişletilir.
 *
 * @return char* Kullanıcıdan alınan satırı döner. NULL karakter ile sonlandırılmıştır.
 * @warning Dönen stringi serbest bırakmak için free() kullanmayı unutmayın.
 */
char* read_line(void)
{
    // Tampon boyutu ve pozisyonu tanımla
    int   bufsize = BUFFER_SIZE; // İlk tampon boyutu
    int   position = 0;         // Yazılacak pozisyon
    char* buffer = malloc(sizeof(char) * bufsize); // Tampon için bellek tahsisi
    char  ch;                  // Okunan karakter

    // Bellek tahsisi kontrolü
    if (!buffer)
    {
        fprintf(stderr, "Bellek ayırma hatası\n");
        exit(EXIT_FAILURE); // Hata durumunda programı sonlandır
    }

    while (true)
    {
        // Standart girdiden bir karakter oku
        ch = getchar();

        // EOF (dosya sonu) veya yeni satır karakteriyle karşılaşırsak işlemi sonlandır
        if (ch == EOF || ch == '\n')
        {
            buffer[position] = '\0'; // Sonuna null karakter ekle
            return buffer; // Okunan satırı dön
        }
        else
        {
            buffer[position] = ch; // Karakteri tampona yaz
        }
        position++;

        // Tampon boyutunu aşarsak belleği genişlet
        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE; // Tampon boyutunu arttır
            buffer = realloc(buffer, bufsize); // Belleği yeniden tahsis et

            // Bellek tahsisi kontrolü
            if (!buffer)
            {
                fprintf(stderr, "Bellek ayırma hatası\n");
                exit(EXIT_FAILURE); // Hata durumunda programı sonlandır
            }
        }
    }

    return buffer; // Okunan stringi dön (bu satıra normal şartlarda ulaşılmaz)
}
