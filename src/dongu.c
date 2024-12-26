/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */

#include "../include/dongu.h"
#include "../include/calistir.h"
#include "../include/split_commands.h"

 /**
  * @brief Programın temel işlevini yürüten ana döngüyü başlatır.
  * Kullanıcıdan girdi alarak bu girdişleri ayrıştırır ve uygun fonksiyonları çağırır.
  *
  * Bu döngü, kullanıcının komut girmesi ve programı kontrol etmesine olanak tanır.
  * Komutlar, noktalı virgül (;) ile ayrılır ve ayrı ayrı işlenir.
  */
void dongu(void)
{
    // Kullanıcıdan alınan girdiler ve ayrıştırılan bilgiler için değişkenler
    char* line = NULL;        // Kullanıcıdan alınan girdi satırı
    char** commands = NULL;   // Ayrıştırılmış komutlar dizisi
    char** args = NULL;       // Komut argümanlarını tutacak dizi
    int status = 1;           // Programın çalışma durumu (1: devam, 0: sonlanma)

    do {
        // Komut istemini yaz ve standart çıktıyı temizle
        printf(PROMPT);
        fflush(stdout);

        // Kullanıcıdan bir satır okuma
        line = read_line();
        if (!line) {
            fprintf(stderr, "Satır okuma hatası\n");
            break; // Satır okunamazsa döngü sonlanır
        }

        // Satırı noktalı virgüle (;) göre komutlara ayır
        commands = split_commands(line);
        if (!commands) {
            free(line); // Hafıza temizlenir
            fprintf(stderr, "Komutları ayırma hatası\n");
            break;
        }

        // Her komut için döngüye gir
        for (int i = 0; commands[i] != NULL; i++) {
            // Komutun argümanlarını boşluklara göre ayrıştır
            args = split_whitespace(commands[i]);
            if (!args) {
                fprintf(stderr, "Komut ayrıştırma hatası\n");
                continue; // Hatalı komut atlanır
            }

            // "increment" komutunu kontrol edip çalıştır
            if (args[0] != NULL && strcmp(args[0], "increment") == 0) {
                if (_increment(args) != 0) { // Hata durumunda kontrol
                    status = 0; // Programı durdurmak için durum değiştir
                    free(args);
                    break;
                }
            }
            // "quit" komutunu kontrol ederek programı sonlandır
            else if (args[0] != NULL && strcmp(args[0], "quit") == 0) {
                status = 0; // Durumu 0 yaparak döngüyü sonlandır
                free(args);
                break;
            }
            // Diğer komutları çalıştır
            else {
                calistir(args); // Komut ve parametreleri işle
            }

            free(args); // Her komutun argümanlarının hafızasını temizle
        }

        // Satır ve komutların hafızasını serbest bırak
        free(line);
        free(commands);
    } while (status); // Durum 0 olmadığı sürece döngü devam eder
}