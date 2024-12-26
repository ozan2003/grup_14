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
  * @brief Kullanıcının girdiği komutu çalıştırır.
  * Tekli komutları, pipe'ları ve yönlendirmelerden sorumludur.
  * @param args Komut ve argümanlarını içeren dizi.
  */
int calistir(char** args);

#endif // CALISTIR_H
