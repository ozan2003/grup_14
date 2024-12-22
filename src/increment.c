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
  * @brief Aldığı sayıyı 1 arttıran fonksiyon.
  * @param args Komut ve argümanlarını içeren dizi.
  * @return 0: Başarıyla tamamlandı, -1: Hata.
  */
int _increment(char** args) {
    int num = 0;
    char buffer[256] = { 0 };
    FILE* file = NULL;

    // Durum 1: Argüman doğrudan sağlanmış
    if (args[1] != NULL) {
        // Dosya yönlendirmesi olup olmadığını kontrol et
        if (strcmp(args[1], "<") == 0 && args[2] != NULL) {
            file = fopen(args[2], "r");
            if (!file) {
                fprintf(stderr, "Dosya açma hatası: %s\n", args[2]);
                return -1;
            }

            // Dosyadan ilk satırı oku
            if (fgets(buffer, sizeof(buffer), file) != NULL) {
                // Yeni satır karakterlerini kaldır
                buffer[strcspn(buffer, "\r\n")] = '\0';
                num = atoi(buffer);
                fclose(file);
            }
            else {
                fprintf(stderr, "Dosya okuma hatası veya dosya boş\n");
                fclose(file);
                return -1;
            }
        }
        else {
            // Doğrudan sayı argümanı
            num = atoi(args[1]);
        }
    }
    // Durum 2: Argüman yok, stdin'den oku
    else {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\r\n")] = '\0';
            num = atoi(buffer);
        }
        else if (feof(stdin)) {
            return 0;
        }
        else {
            fprintf(stderr, "Girdi okuma hatası\n");
            return -1;
        }
    }

    // Girdi doğrulama
    if (num == 0 && buffer[0] != '0') {
        fprintf(stderr, "Geçersiz girdi: %s\n", buffer);
        return -1;
    }

    // Arttırılmış sayıyı yazdır
    printf("%d\n", num + 1);
    return 0;
}
