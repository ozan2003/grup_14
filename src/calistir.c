/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */

#include "../include/calistir.h"
#include "../include/split_commands.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

 /**
  * @brief Tek bir komutçuğu çalıştırır.
  * Gerekirse komut için girdi ve çıktı dosyaları yönlendirilir ve arka planda çalışma durumu düzenlenir.
  * @param args Komut ve argüman dizisi.
  * @param input_file Okunacak dosyanın ismi. NULL olduğunda standart girdi kullanılır.
  * @param output_file Yazılacak dosyanın ismi. NULL olduğunda standart çıktı kullanılır.
  * @param is_background Komutun arka planda çalışma durumu. 1 ise arka planda çalışır.
  */
void execute_single_command(char** args,
    char* input_file,
    char* output_file,
    int    is_background);

/**
 * @brief Pipe (boru hattı) kullanarak birden fazla komutun ardışık çalışmasını sağlar.
 * @param args Komut ve argüman dizisi.
 * @param input_file Zincirin ilk komutunun girdisini sağlayan dosya.
 * @param output_file Zincirin son komutunun çıktısını kaydedecek dosya.
 * @param is_background Zincirleme komutların arka planda çalışma durumu.
 */
void execute_pipeline(char** args,
    char* input_file,
    char* output_file,
    int    is_background);

/**
 * @brief Kullanıcı tarafından girilen bir komut dizisini çalıştırır.
 * Komutlar tekli, pipe (boru hattı) veya girdi-çıktı yönlendirmeleriyle çalışabilir.
 * Bu fonksiyon, uygun ayrıştırmaları yaparak ilgili çalıştırma fonksiyonunu çağırır.
 * @param args Komut ve argüman dizisi.
 * @return 1 (başarılı işlem) veya 0 (boş komut).
 */
int calistir(char** args)
{
    if (args == NULL || args[0] == NULL)
    {
        return 0; // Komut verilmediyse işlem yapmadan çık
    }

    // Girdi/çıktı dosyaları, arka plan bayrağı ve pipe miktarı gibi değişkenleri tanımla
    char* input_file = NULL;
    char* output_file = NULL;
    int   is_background = 0;
    int   num_pipes = 0; // Pipe miktarını belirle

    // Argümanları analiz ederek yönlendirmeleri belirle
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0 && args[i + 1] != NULL)
        {
            input_file = args[i + 1];
            args[i] = NULL; // Yönlendirme karakterini temizle
        }
        else if (strcmp(args[i], ">") == 0 && args[i + 1] != NULL)
        {
            output_file = args[i + 1];
            args[i] = NULL; // Yönlendirme karakterini temizle
        }
        else if (strcmp(args[i], "&") == 0)
        {
            is_background = 1; // Arka plan bayrağını aktif et
            args[i] = NULL;
        }
        else if (strcmp(args[i], "|") == 0)
        {
            num_pipes++; // Pipe sayısını artır
        }
    }

    // Pipe sayısına göre uygun fonksiyonu çağır
    if (num_pipes == 0)
    {
        execute_single_command(args, input_file, output_file, is_background);
    }
    else
    {
        execute_pipeline(args, input_file, output_file, is_background);
    }
    return 1;
}

/**
 * @brief Belirli bir komutu çalıştırır.
 * Gerektiğinde girdi ve çıktı dosyaları ile çalışma durumu düzenlenir.
 */
void execute_single_command(char** args,
    char* input_file,
    char* output_file,
    int    is_background)
{
    // "increment" adlı özel bir komutun işlenip işlenmediğini kontrol et
    if (strcmp(args[0], "increment") == 0)
    {
        // Girdi dosyası yönlendirmesi yap (eğer mevcutsa)
        if (input_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("Girdi dosyası açılamadı");
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // "increment" fonksiyonunu çağır ve işlemi sonlandır
        _increment(args);
        return;
    }

    // Yeni bir süreç oluştur (fork)
    pid_t pid = fork();
    if (pid == 0)
    {
        // Çocuk süreç: Komutun işleneceği yer

        // Girdi dosyasını standart girdiye yönlendir
        if (input_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("Girdi dosyası açılamadı");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Çıktı dosyasını standart çıktıya yönlendir
        if (output_file)
        {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Çıktı dosyası açılamadı");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Komutun kendisini çalıştır (execvp)
        execvp(args[0], args);
        perror("Komut çalıştırılamadı");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Fork oluşturulamadı
        perror("Fork hatası");
    }
    else
    {
        // Ana süreç: Arka plan durumuna bağlı olarak çocuk süreci bekle
        if (!is_background)
        {
            waitpid(pid, NULL, 0);
        }
    }
}

/**
 * @brief Pipe yapısı kullanarak birden fazla komutun zincirleme çalışmasını sağlar.
 * Her komut, bir önceki komutun çıktısını girdi olarak kullanır.
 */
void execute_pipeline(char** args,
    char* input_file,
    char* output_file,
    int    is_background)
{
    int   pipes[2][2]; // Birden fazla komut zinciri için pipe dizileri
    int   input_fd = STDIN_FILENO; // Varsayılan olarak standart girdi
    pid_t pid; // Yeni süreçlerin kimliklerini tutmak için tanımlanır
    int   cmd_index = 0; // Mevcut komut indeksi

    while (args[cmd_index] != NULL)
    {
        // Mevcut komutun argümanlarını ayrıp bir diziye aktar
        char* current_cmd[128]; // Maksimum 128 argüman desteklenir
        int   i = 0;
        while (args[cmd_index] != NULL && strcmp(args[cmd_index], "|") != 0)
        {
            current_cmd[i++] = args[cmd_index++];
        }
        current_cmd[i] = NULL; // Dizi sonlandırılır

        // Pipe sembolünü atla ve bir sonraki komutla devam et
        if (args[cmd_index] != NULL)
            cmd_index++;

        // Yeni bir pipe oluştur, zincirin bir sonraki bağlantısını sağla
        if (args[cmd_index] != NULL && pipe(pipes[cmd_index % 2]) < 0)
        {
            perror("Pipe oluşturulamadı");
            return;
        }

        pid = fork(); // Yeni bir çocuk süreç oluştur
        if (pid == 0)
        {
            // Çocuk süreç: Girdi kaynağını ve çıktı hedefini belirle

            if (input_fd != STDIN_FILENO)
            {
                // Bir önceki pipe girdisini standart girdiye yönlendir
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            else if (input_file)
            {
                // İlk komutun girdisi bir dosya ise bunu yönlendir
                int fd = open(input_file, O_RDONLY);
                if (fd < 0)
                {
                    perror("Girdi dosyası açılamadı");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Çıktı hedefini belirle
            if (args[cmd_index] != NULL)
            {
                // Bir sonraki komut mevcutsa, çıktıyı pipe'a yönlendir
                dup2(pipes[cmd_index % 2][1], STDOUT_FILENO);
                close(pipes[cmd_index % 2][0]);
                close(pipes[cmd_index % 2][1]);
            }
            else if (output_file)
            {
                // Zincirin son komutunun çıktısı bir dosya ise bunu ayarla
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("\u00c7ıktı dosyası açılamadı");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // "increment" komutunu kontrol et ve çalıştır
            if (strcmp(current_cmd[0], "increment") == 0)
            {
                _increment(current_cmd);
                exit(EXIT_SUCCESS);
            }

            // Komutu çalıştır (execvp)
            execvp(current_cmd[0], current_cmd);
            perror("Komut çalıştırılamadı");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            // Fork oluşturulamadı
            perror("Fork hatası");
            return;
        }

        // Ana süreç: Kaynakları serbest bırak ve bir sonraki adıma hazırlan
        if (input_fd != STDIN_FILENO)
        {
            close(input_fd);
        }
        if (args[cmd_index] != NULL)
        {
            // Bir sonraki komutun girdisi olarak pipe okuyucu ucunu ayarla
            input_fd = pipes[cmd_index % 2][0];
            close(pipes[cmd_index % 2][1]);
        }
    }

    // Tüm komutların tamamlanmasını bekle
    if (!is_background)
    {
        while (wait(NULL) > 0)
            ;
    }
}