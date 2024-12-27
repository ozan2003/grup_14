/**
 * Grup üyeleri:
 * - Eren Çoban - G221210090
 * - Onur Eğrikılıç - G221210066
 * - Ozan Malcı - B211210039
 * - Semih Öztürk - G221210003
 * - Yiğit Talha Adagülü - G221210093
 */
#ifndef CALISTIR_H
#define CALISTIR_H

#include <fcntl.h>    // Dosya kontrol seçenekleri (open bayrakları)
#include <stdio.h>    // Standart giriş/çıkış
#include <stdlib.h>   // Standart kütüphane fonksiyonları (exit, malloc, vb.)
#include <string.h>   // String manipülasyon fonksiyonları
#include <sys/wait.h> // Süreç sonlandırma bekleme..
#include <unistd.h>   // POSIX işletim sistemi API'si (fork, exec, pipe)
#include <signal.h>   // Sinyal işleme fonksiyonları

#include "./builtins/increment.h"

/**
 * @brief Tek bir komutçuğu çalıştırır.
 * Gerekirse komut için girdi ve çıktı dosyaları yönlendirilir ve arka planda
 * çalışma durumu düzenlenir.
 * @param args Komut ve argüman dizisi.
 * @param input_file Okunacak dosyanın ismi. NULL olduğunda standart girdi
 * kullanılır.
 * @param output_file Yazılacak dosyanın ismi. NULL olduğunda standart çıktı
 * kullanılır.
 * @param is_background Komutun arka planda çalışma durumu. 1 ise arka planda
 * çalışır.
 */
void execute_single_command(char** args,
                            char*  input_file,
                            char*  output_file,
                            int    is_background);

/**
 * @brief Pipe (boru hattı) kullanarak birden fazla komutun ardışık çalışmasını
 * sağlar.
 * @param args Komut ve argüman dizisi.
 * @param input_file Zincirin ilk komutunun girdisini sağlayan dosya.
 * @param output_file Zincirin son komutunun çıktısını kaydedecek dosya.
 * @param is_background Zincirleme komutların arka planda çalışma durumu.
 */
void execute_pipeline(char** args,
                      char*  input_file,
                      char*  output_file,
                      int    is_background);

/**
 * @brief Kullanıcının girdiği komutu çalıştırır.
 * Tekli komutları, pipe'ları ve yönlendirmelerden sorumludur.
 * @param args Komut ve argümanlarını içeren dizi.
 */
int calistir(char** args);

#endif // CALISTIR_H
