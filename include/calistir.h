#ifndef CALISTIR_H
#define CALISTIR_H

#include <fcntl.h>    // File control options (open flags)
#include <stdio.h>    // Standard input/output
#include <stdlib.h>   // Standard library functions (exit, malloc, etc.)
#include <string.h>   // String manipulation functions
#include <sys/wait.h> // Wait for process termination
#include <unistd.h>   // POSIX operating system API (fork, exec, pipe)
#include <signal.h>   // Signal handling functions

#include "./builtins/increment.h"

/**
 * @brief Kullanıcının girdiği komutu çalıştırır.
 * Tekli komutları, pipe'ları ve redirection'lardan sorumludur.
 * @param args Komut ve argümanlarını içeren dizi.
 */
void calistir(char** args);


#endif // CALISTIR_H