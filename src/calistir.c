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
  * @param args Komut ve argümanlarını içeren dizi.
  * @param input_file Girdi dosyasının adı.
  * @param output_file Çıktı dosyasının adı.
  * @param is_background Arka planda çalışıp çalışmayacağını belirten bayrak.
  */
void execute_single_command(char** args,
    char* input_file,
    char* output_file,
    int    is_background);

/**
 * @brief Pipe'lı komutları çalıştırır.
 * @param args Komut ve argümanlarını içeren dizi.
 * @param input_file Girdi dosyasının adı.
 * @param output_file Çıktı dosyasının adı.
 * @param is_background Arka planda çalışıp çalışmayacağını belirten bayrak.
 */
void execute_pipeline(char** args,
    char* input_file,
    char* output_file,
    int    is_background);

/**
 * @brief Kullanıcının girdiği komutu çalıştırır.
 * Tekli komutları, pipe'ları ve yönlendirmelerden sorumludur.
 * @param args Komut ve argümanlarını içeren dizi.
 */
int calistir(char** args)
{
    if (args == NULL || args[0] == NULL)
    {
        return 0; // Çalıştırılacak komut yok
    }

    // Yönlendirme, pipe ve arka plan için değişkenler
    char* input_file = NULL;
    char* output_file = NULL;
    int   is_background = 0;
    int   num_pipes = 0;

    // Yönlendirme, pipe ve arka plan için argümanları ayrıştır
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0 && args[i + 1] != NULL)
        {
            input_file = args[i + 1];
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">") == 0 && args[i + 1] != NULL)
        {
            output_file = args[i + 1];
            args[i] = NULL;
        }
        else if (strcmp(args[i], "&") == 0)
        {
            is_background = 1;
            args[i] = NULL;
        }
        else if (strcmp(args[i], "|") == 0)
        {
            num_pipes++;
        }
    }

    // Tekli komut ya da pipe işlemine göre çağrı yap
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
 */
void execute_single_command(char** args,
    char* input_file,
    char* output_file,
    int    is_background)
{
    // Özel "increment" komutunu kontrol et
    if (strcmp(args[0], "increment") == 0)
    {
        // Gerekirse girdiyi yönlendir
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

        // Increment fonksiyonunu çağır
        _increment(args);
        return;
    }

    pid_t pid = fork();
    if (pid == 0)
    { // Çocuk süreç
        // Girdi yönlendirmesi
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

        // Çıktı yönlendirmesi
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

        // Komutu çalıştır
        execvp(args[0], args);
        perror("Çalıştırma başarısız");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("Fork hatası");
    }
    else
    {
        // Ana süreç
        if (!is_background)
        {
            waitpid(pid, NULL, 0); // Çocuk süreci bekle
        }
    }
}

/**
 * @brief Pipe'lı komutları çalıştırır.
 */
void execute_pipeline(char** args,
    char* input_file,
    char* output_file,
    int    is_background)
{
    int   pipes[2][2];
    int   input_fd = STDIN_FILENO;
    pid_t pid;
    int   cmd_index = 0;

    while (args[cmd_index] != NULL)
    {
        // Bir sonraki komutu bul
        char* current_cmd[128];
        int   i = 0;
        while (args[cmd_index] != NULL && strcmp(args[cmd_index], "|") != 0)
        {
            current_cmd[i++] = args[cmd_index++];
        }
        current_cmd[i] = NULL;

        // Pipe sembolünü atla
        if (args[cmd_index] != NULL)
            cmd_index++;

        // Gerekirse bir pipe oluştur
        if (args[cmd_index] != NULL && pipe(pipes[cmd_index % 2]) < 0)
        {
            perror("Pipe oluşturma hatası");
            return;
        }

        pid = fork();
        if (pid == 0)
        { // Çocuk süreç
            // Önceki pipe'tan ya da girdi dosyasından yönlendir
            if (input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            else if (input_file)
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

            // Çıktıyı bir sonraki pipe'a ya da son çıktıya yönlendir
            if (args[cmd_index] != NULL)
            {
                dup2(pipes[cmd_index % 2][1], STDOUT_FILENO);
                close(pipes[cmd_index % 2][0]);
                close(pipes[cmd_index % 2][1]);
            }
            else if (output_file)
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

            // Özel "increment" komutunu kontrol et
            if (strcmp(current_cmd[0], "increment") == 0)
            {
                _increment(current_cmd);
                exit(EXIT_SUCCESS);
            }

            execvp(current_cmd[0], current_cmd);
            perror("Çalıştırma başarısız");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("Fork hatası");
            return;
        }

        // Ana süreç: Pipe'ları kapat ve input_fd'yi güncelle
        if (input_fd != STDIN_FILENO)
        {
            close(input_fd);
        }
        if (args[cmd_index] != NULL)
        {
            input_fd = pipes[cmd_index % 2][0];
            close(pipes[cmd_index % 2][1]);
        }
    }

    if (!is_background)
    {
        while (wait(NULL) > 0)
            ;
    }
}
