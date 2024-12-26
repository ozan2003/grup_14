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
 * @brief Girilen bir komut satırını parçalayarak ayrı öğeler haline getirir.
 * Komutlar noktalı virgül (;) karakterine göre ayrılır.
 * 
 * @param line Ayrıştırılacak komut satırı.
 * @return char** Ayrıştırılmış komut dizisini döner.
 */
char** split_commands(char* line) {
    // İlk tampon boyutu ve mevcut pozisyonu tanımla
    int bufsize = 64, position = 0;

    // Komut parçalarını tutacak dinamik bellekten yer ayırılmış dizi
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token; // Geçici token değişkeni

    // Bellek tahsisi kontrolü
    if (!tokens) {
        fprintf(stderr, "allocation error\n"); // Bellek hatası mesajı
        exit(EXIT_FAILURE); // Programı hatayla sonlandır
    }

    // Komut satırını noktalı virgüle göre parçala
    token = strtok(line, ";");
    while (token != NULL) {
        // Ayrıştırılan komut parçasını diziye ekle
        tokens[position++] = token;

        // Dizi kapasitesi dolduysa tamponu genişlet
        if (position >= bufsize) {
            bufsize += 64; // Tampon boyutunu arttır
            tokens = realloc(tokens, bufsize * sizeof(char*)); // Bellek yeniden tahsis edilir

            // Bellek tahsisi kontrolü
            if (!tokens) {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        // Bir sonraki tokenı al
        token = strtok(NULL, ";");
    }

    // Dizi sonuna NULL ekle (sonlandırıcı)
    tokens[position] = NULL;
    return tokens; // Ayrıştırılmış komutlar dizisini döner
}
