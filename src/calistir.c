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
 * @brief Tek bir komutu çalıştırır. 
 * Komutun girdi (örneğin bir dosya) ve çıktı dosyaları ile arka planda çalışma durumunu işler.
 * @param args Komut ve argümanlarını içeren dizi.
 * @param input_file Komutun okuyacağı dosyanın adı. NULL ise standart girdiyi kullanır.
 * @param output_file Komutun yazacağı dosyanın adı. NULL ise standart çıktıyı kullanır.
 * @param is_background Komutun arka planda çalışıp çalışmayacağını belirtir. 1 ise arka planda çalışır.
 */
void execute_single_command(char** args,
    char* input_file,
    char* output_file,
    int    is_background);

/**
 * @brief Pipe (boru) işlemleriyle birden fazla komutun zincirleme çalışmasını sağlar. 
 * @param args Komut ve argümanlarını içeren dizi.
 * @param input_file Zincirde ilk komut için girdi dosyası.
 * @param output_file Zincirde son komutun çıktısını yazacağı dosya.
 * @param is_background Zincirleme komutların arka planda çalışma durumunu belirtir.
 */
void execute_pipeline(char** args,
    char* input_file,
    char* output_file,
    int    is_background);

/**
 * @brief Kullanıcının girdiği bir komut dizisini çalıştırır.
 * Komutlar tekli, pipe (boru) işlemleri veya girdi-çıktı yönlendirmeleriyle çalışabilir.
 * Bu fonksiyon, gerekli ayrıştırmaları yaparak ilgili komut çalıştırma fonksiyonunu çağırır.
 * @param args Komut ve argümanlarını içeren dizi.
 * @return 1 (başarılı işlem) veya 0 (boş komut).
 */
int calistir(char** args)
{
    if (args == NULL || args[0] == NULL)
    {
        return 0; // Hiçbir komut verilmemişse işlem yapmadan çık
    }

    // Girdi ve çıktı dosyaları, arka plan bayrağı ve pipe sayısı gibi özellikleri tanımla
    char* input_file = NULL;
    char* output_file = NULL;
    int   is_background = 0;
    int   num_pipes = 0; // Pipe sayısını hesapla

    // Argümanları analiz et ve yönlendirmeleri belirle
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0 && args[i + 1] != NULL)
        {
            input_file = args[i + 1];
            args[i] = NULL; // Yönlendirme karakterini kaldır
        }
        else if (strcmp(args[i], ">") == 0 && args[i + 1] != NULL)
        {
            output_file = args[i + 1];
            args[i] = NULL; // Yönlendirme karakterini kaldır
        }
        else if (strcmp(args[i], "&") == 0)
        {
            is_background = 1; // Arka plan bayrağını aktif et
            args[i] = NULL;
        }
        else if (strcmp(args[i], "|") == 0)
        {
            num_pipes++; // Pipe sayısını arttır
        }
    }

    // Pipe sayısına bağlı olarak doğru fonksiyonu çağır
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
 * @brief Tek bir komutu çalıştırır. 
 * Gerekirse girdi ve çıktı yönlendirmesi yapar ve arka planda çalışma seçeneğini kontrol eder.
 */
void execute_single_command(char** args,
    char* input_file,
    char* output_file,
    int    is_background)
{
    // "increment" adlı özel bir komutun çalıştırılmasını kontrol et
    if (strcmp(args[0], "increment") == 0)
    {
        // Girdiyi yönlendirme (eğer gerekliyse)
        if (input_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("Girdi dosyası hatası");
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
        // Çocuk süreç (komutun çalıştırılacağı yer)

        // Girdi dosyasının standart girdiye yönlendirilmesi
        if (input_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("Girdi dosyası hatası");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Çıktı dosyasının standart çıktıya yönlendirilmesi
        if (output_file)
        {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Çıktı dosyası hatası");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Komutu çalıştır (execvp)
        execvp(args[0], args);
        perror("Çalıştırma başarısız");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Fork hatası
        perror("Fork hatası");
    }
    else
    {
        // Ana süreç: Arka plan durumuna göre çocuk süreci bekle
        if (!is_background)
        {
            waitpid(pid, NULL, 0);
        }
    }
}

/**
 * @brief Pipe ile birden fazla komutun zincirleme olarak çalıştırılmasını sağlar.
 * Her bir komutun girdisi bir önceki komutun çıktısı olarak kullanılır.
 */
void execute_pipeline(char** args,
    char* input_file,
    char* output_file,
    int    is_background)
{
    int   pipes[2][2]; // Pipe dizileri: İki boru hattının birden fazla komut zinciri için kullanılması
    int   input_fd = STDIN_FILENO; // Varsayılan olarak standart girdi kullanılır
    pid_t pid; // Yeni süreç oluşturmak için pid tanımlanır
    int   cmd_index = 0; // Şu anki komut indeksi

    while (args[cmd_index] != NULL)
    {
        // Şu anki komutun argümanlarını ayır ve bir diziye aktar
        char* current_cmd[128]; // Maksimum 128 argüman
        int   i = 0;
        while (args[cmd_index] != NULL && strcmp(args[cmd_index], "|") != 0)
        {
            current_cmd[i++] = args[cmd_index++];
        }
        current_cmd[i] = NULL; // Komut dizisinin sonunu belirtmek için NULL ekle

        // Pipe sembolünü atla, bir sonraki komutla devam et
        if (args[cmd_index] != NULL)
            cmd_index++;

        // Gerekliyse yeni bir pipe oluştur, bir önceki pipe kapanmadan bir sonraki bağlantıyı sağlar
        if (args[cmd_index] != NULL && pipe(pipes[cmd_index % 2]) < 0)
        {
            perror("Pipe oluşturma hatası");
            return;
        }

        pid = fork(); // Yeni bir çocuk süreç oluştur
        if (pid == 0)
        { 
            // Çocuk süreç başlangıcı: Girdi kaynağını belirle

            if (input_fd != STDIN_FILENO)
            {
                // Eğer bir önceki pipe kullanılıyorsa, bunu standart girdiye yönlendir
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            else if (input_file)
            {
                // İlk komutun girdisi bir dosyadansa, bu dosyayı standart girdiye yönlendir
                int fd = open(input_file, O_RDONLY);
                if (fd < 0)
                {
                    perror("Girdi dosyası hatası");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Çıktı kaynağını belirle
            if (args[cmd_index] != NULL)
            {
                // Bir sonraki komut mevcutsa, çıktıyı bir pipe'a yönlendir
                dup2(pipes[cmd_index % 2][1], STDOUT_FILENO);
                close(pipes[cmd_index % 2][0]);
                close(pipes[cmd_index % 2][1]);
            }
            else if (output_file)
            {
                // Zincirin son komutunun çıktısı bir dosyayı hedefliyorsa, bu dosyayı standart çıktıya yönlendir
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("\u00c7ıktı dosyası hatası");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // "increment" adlı özel bir komutu kontrol et
            if (strcmp(current_cmd[0], "increment") == 0)
            {
                _increment(current_cmd);
                exit(EXIT_SUCCESS);
            }

            // Komutu çalıştır (execvp ile gerçek komut yönetilir)
            execvp(current_cmd[0], current_cmd);
            perror("\u00c7alıştırma başarısız");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            // Fork hatası
            perror("Fork hatası");
            return;
        }

        // Ana süreç için temizleme işlemleri: Kullanılan kaynakları serbest bırak
        if (input_fd != STDIN_FILENO)
        {
            close(input_fd);
        }
        if (args[cmd_index] != NULL)
        {
            // Bir sonraki komutun girdisi olarak kullanmak üzere pipe'ın okuyucu ucu input_fd'ye atanır
            input_fd = pipes[cmd_index % 2][0];
            close(pipes[cmd_index % 2][1]);
        }
    }

    // Zincirdeki tüm komutlar için çocuk süreçler tamamlanana kadar bekle
    if (!is_background)
    {
        while (wait(NULL) > 0)
            ;
    }
}
