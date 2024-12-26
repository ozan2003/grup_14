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
  * @brief Verilen sayıyı 1 artırıp çıktılayan fonksiyon.
  *
  * Bu fonksiyon, girdi olarak alınan bir tam sayıyın değerini artırır ve sonucu standart çıktıya yazar.
  * Girdi dosyadan, argümandan veya standart girdiden alınabilir.
  *
  * @param args Komut ve argüman dizisini tutar.
  * @return 0: Başarıyla tamamlanma, -1: Hata durumunda.
  */
int _increment(char** args) {
    int num = 0;                // Girdi olarak alınan tam sayıyı saklamak için değişken
    char buffer[256] = { 0 };   // Girdi için kullanılan tampon
    FILE* file = NULL;          // Dosya okuma için tanımlanan dosya pointerı

    // Durum 1: Argüman verilmişse işle
    if (args[1] != NULL) {
        // Eğer bir dosya yönlendirmesi mevcutsa kontrol et
        if (strcmp(args[1], "<") == 0 && args[2] != NULL) {
            // Dosyayı aç ve okuma modunda kullan
            file = fopen(args[2], "r");
            if (!file) {
                fprintf(stderr, "Dosya açılamadı: %s\n", args[2]);
                return -1; // Dosya açılamıyorsa hata dön
            }

            // Dosyadan ilk satırı oku
            if (fgets(buffer, sizeof(buffer), file) != NULL) {
                // Tampondaki yeni satır karakterlerini temizle
                buffer[strcspn(buffer, "\r\n")] = '\0';
                num = atoi(buffer); // Stringi tam sayıya dönüştür
                fclose(file); // Dosyayı kapat
            }
            else {
                fprintf(stderr, "Dosya okunamıyor veya boş\n");
                fclose(file);
                return -1; // Dosya boş veya okunamadıysa hata dön
            }
        }
        else {
            // Argüman bir tam sayıysa çevir
            num = atoi(args[1]);
        }
    }
    // Durum 2: Argüman verilmemişse standart girdiden oku
    else {
        // Standart girdiden bir satır al
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Tampondaki yeni satır karakterlerini temizle
            buffer[strcspn(buffer, "\r\n")] = '\0';
            num = atoi(buffer); // Stringi tam sayıya dönüştür
        }
        else if (feof(stdin)) {
            return 0; // Girdi bitmişse fonksiyondan çık
        }
        else {
            fprintf(stderr, "Standart girdi okunamıyor\n");
            return -1; // Girdi okunamadıysa hata dön
        }
    }

    // Girdiyi kontrol et: Geçersiz sayı girişi durumunda hata mesajı ver
    if (num == 0 && buffer[0] != '0') {
        fprintf(stderr, "Geçersiz girdi: %s\n", buffer);
        return -1; // Hatalı girdi durumu
    }

    // Sayıyı 1 artır ve sonucu ekrana yazdır
    printf("%d\n", num + 1);
    return 0; // Başarıyla çalışma tamamlandı
}