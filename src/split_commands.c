/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */

#include "split_commands.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

 /**
  * @brief Verilen komut satırını ayrıştırarak her bir komutu birer öğe olarak döndüren fonksiyon.
  *
  * Bu fonksiyon, noktalı virgül (";") karakterine göre bir satırı parçalar ve parçalanmış komutları
  * bir dizi olarak döner.
  *
  * @param line Ayrıştırılacak komut satırı.
  * @return char** Ayrıştırılmış komutları içeren bir dizi döner. Dizi NULL ile sonlandırılmıştır.
  * @warning Dönen diziyi serbest bırakmak için free() fonksiyonunu kullanmayı unutmayın.
  */
char** split_commands(char* line) {
    // Tampon boyutu ve pozisyonu tanımla
    int bufsize = 64; // İlk tampon boyutu
    int position = 0; // Komut dizisine yazılacak pozisyon

    // Komut parçalarını tutacak dinamik bellekten tahsis edilmiş dizi
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token; // Geçici değişken, her bir komutu tutar

    // Bellek tahsisi kontrolü
    if (!tokens) {
        fprintf(stderr, "Bellek tahsisi başarısız\n");
        exit(EXIT_FAILURE); // Hata durumunda programı sonlandır
    }

    // Komut satırını noktalı virgüle göre parçala
    token = strtok(line, ";");
    while (token != NULL) {
        // Ayrıştırılan komutu dizide sakla
        tokens[position++] = token;

        // Dizi kapasitesi dolduğunda tamponu genişlet
        if (position >= bufsize) {
            bufsize += 64; // Tampon boyutunu arttır
            tokens = realloc(tokens, bufsize * sizeof(char*)); // Belleği yeniden tahsis et

            // Yeniden tahsis başarı kontrolü
            if (!tokens) {
                fprintf(stderr, "Bellek tahsisi başarısız\n");
                exit(EXIT_FAILURE);
            }
        }

        // Bir sonraki komutu al
        token = strtok(NULL, ";");
    }

    // Komut dizisinin sonuna NULL ekle (sonlandırıcı)
    tokens[position] = NULL;
    return tokens; // Ayrıştırılmış komutları içeren diziyi döner
}