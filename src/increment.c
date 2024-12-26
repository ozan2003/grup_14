/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/builtins/increment.h"

/**
 * @brief Aldığı sayıyı 1 arttırarak çıktı veren bir fonksiyon.
 *
 * Bu fonksiyon, girdi olarak verilen sayıyın değerini alır, 1 arttırır ve sonucu standart çıktıya yazar.
 * Girdi bir dosyadan, argümandan veya standart girdiden alınabilir.
 *
 * @param args Komut ve argümanlarını içeren dizi.
 * @return 0: Başarıyla tamamlandı, -1: Hata durumunda.
 */
int _increment(char** args) {
    int num = 0;                // Sayıyı tutacak değişken
    char buffer[256] = { 0 };   // Girdi için tampon
    FILE* file = NULL;          // Dosya okuma için dosya pointerı

    // Durum 1: Argüman doğrudan verilmiş
    if (args[1] != NULL) {
        // Dosya yönlendirmesi olup olmadığını kontrol et
        if (strcmp(args[1], "<") == 0 && args[2] != NULL) {
            // Dosyayı aç
            file = fopen(args[2], "r");
            if (!file) {
                fprintf(stderr, "Dosya açma hatası: %s\n", args[2]);
                return -1; // Dosya açılamadıysa hata dön
            }

            // Dosyadan ilk satırı oku
            if (fgets(buffer, sizeof(buffer), file) != NULL) {
                // Yeni satır karakterlerini tampondan kaldır
                buffer[strcspn(buffer, "\r\n")] = '\0';
                num = atoi(buffer); // Tampondaki stringi tam sayıya çevir
                fclose(file); // Dosyayı kapat
            }
            else {
                fprintf(stderr, "Dosya okuma hatası veya dosya boş\n");
                fclose(file);
                return -1; // Dosya boş veya okunamıyorsa hata dön
            }
        }
        else {
            // Doğrudan bir sayı argümanı verilmişse onu çevir
            num = atoi(args[1]);
        }
    }
    // Durum 2: Argüman yok, standart girdiden oku
    else {
        // Standart girdiden satır oku
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Yeni satır karakterlerini kaldır
            buffer[strcspn(buffer, "\r\n")] = '\0';
            num = atoi(buffer); // Tampondaki stringi tam sayıya çevir
        }
        else if (feof(stdin)) {
            return 0; // Girdi bitmişse (EOF), çık
        }
        else {
            fprintf(stderr, "Girdi okuma hatası\n");
            return -1; // Standart girdi okunamıyorsa hata dön
        }
    }

    // Girdi doğrulama: Sayı geçersizse hata mesajı yaz
    if (num == 0 && buffer[0] != '0') {
        fprintf(stderr, "Geçersiz girdi: %s\n", buffer);
        return -1; // Geçersiz girdi hata döner
    }

    // Sayıyı 1 arttır ve sonucu yazdır
    printf("%d\n", num + 1);
    return 0; // Başarıyla tamamlandı
}
