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
 * @brief Programın ana çalışma döngüsünü başlatır.
 * Kullanıcıdan girdi alır, komutları parçalar ve gerekli işlevleri çağırır.
 *
 * Bu döngü, kullanıcının komutlarını girmesine ve programı kontrol etmesine izin verir.
 * Komutlar, noktalı virgül (;) ile ayrılabilir ve ayrı komutlar olarak çalıştırılabilir.
 */
void dongu(void)
{
    // Kullanıcıdan gelen satır, komutlar ve argümanlar için tanımlar
    char *line = NULL;        // Kullanıcıdan alınan girdi satırı
    char **commands = NULL;   // Girdi satırından ayrıştırılmış komutlar dizisi
    char **args = NULL;       // Her bir komutun ayrıştırılmış argümanları
    int status = 1;           // Program döngüsü durumu (1: devam, 0: sonlandırma)

    do {
        // Kullanıcıdan komut istemi yazdır ve standart çıktıyı temizle
        printf(PROMPT);
        fflush(stdout);

        // Kullanıcıdan bir satır okur
        line = read_line();
        if (!line) {
            fprintf(stderr, "Error reading line\n");
            break; // Satır okunamazsa döngü sonlandırılır
        }

        // Satırı noktalı virgüle (;) göre komutlara ayır
        commands = split_commands(line);
        if (!commands) {
            free(line); // Bellek temizlenir
            fprintf(stderr, "Error splitting commands\n");
            break;
        }

        // Ayrıştırılan her bir komut üzerinde dön
        for (int i = 0; commands[i] != NULL; i++) {
            // Komutun argümanlarını boşluklara göre ayır
            args = split_whitespace(commands[i]);
            if (!args) {
                fprintf(stderr, "Error splitting command\n");
                continue; // Hatalı bir komut atlanır
            }

            // "increment" adlı özel komutu kontrol et ve çalıştır
            if (args[0] != NULL && strcmp(args[0], "increment") == 0) {
                if (_increment(args) != 0) { // Hata durumu kontrolü
                    status = 0; // Programı sonlandırmak için durum değiştirilir
                    free(args);
                    break;
                }
            }
            // "quit" komutunu kontrol et ve programı sonlandır
            else if (args[0] != NULL && strcmp(args[0], "quit") == 0) {
                status = 0; // Döngü durumu değiştirilir
                free(args);
                break;
            }
            // Diğer komutları çalıştır
            else {
                calistir(args); // Komut ve argümanları işle
            }

            free(args); // Her komutun argümanlarının belleği serbest bırakılır
        }

        // Komut satırı ve komutlar dizisi için belleği temizle
        free(line);
        free(commands);
    } while (status); // Durum 0 olana kadar döngü devam eder
}
