#include "../include/calistir.h"

/**
 * @brief Kullanıcının girdiği komutu çalıştırır.
 * Tekli komutları, pipe'ları ve redirection'lardan sorumludur.
 * @param args Komut ve argümanlarını içeren dizi.
 */
void calistir(char** args)
{
    // Check if command is empty
    if (args == NULL || args[0] == NULL)
    {
        return;
    }

    // Variables for tracking redirections and pipes
    int input_redirect  = 0;
    int output_redirect = 0;
    int is_background   = 0;
    int has_pipes       = 0;

    // Temporary storage for input/output filenames
    char* input_file  = NULL;
    char* output_file = NULL;

    // Scan arguments to detect special cases
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            input_redirect = 1;
            if (args[i + 1] != NULL)
            {
                input_file = args[i + 1];
                // Remove redirection markers from args
                args[i]     = NULL;
                args[i + 1] = NULL;
            }
        }
        if (strcmp(args[i], ">") == 0)
        {
            output_redirect = 1;
            if (args[i + 1] != NULL)
            {
                output_file = args[i + 1];
                // Remove redirection markers from args
                args[i]     = NULL;
                args[i + 1] = NULL;
            }
        }
        if (strcmp(args[i], "|") != 0)
        {
            has_pipes = 1;
        }
        if (strcmp(args[i], "&") == 0)
        {
            is_background = 1;
            args[i]       = NULL; // Remove & from arguments
        }
    }

    // Create pipe array for multiple commands
    int pipes[2][2];
    int num_pipes = 0;

    // Count number of pipes
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            num_pipes++;
        }
    }

    // Handling no pipes scenario
    if (num_pipes == 0)
    {
        pid_t pid = fork();
        if (pid == 0)
        { // Child process
            // Handle input redirection
            if (input_redirect)
            {
                int input_fd = open(input_file, O_RDONLY);
                if (input_fd == -1)
                {
                    perror("Giriş dosyası bulunamadı");
                    exit(EXIT_FAILURE);
                }
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Handle output redirection
            if (output_redirect)
            {
                int output_fd =
                    open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd == -1)
                {
                    perror("Çıkış dosyası hatası");
                    exit(EXIT_FAILURE);
                }
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }

            // Execute command
            execvp(args[0], args);
            perror("Komut çalıştırılamadı");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("Fork hatası");
            return;
        }

        // Parent process
        if (!is_background)
        {
            waitpid(pid, NULL, 0);
        }
        return;
    }

    // Pipe handling (complex scenario)
    pid_t pid;
    int   input_fd = STDIN_FILENO;

    // Scan and split commands by pipes
    char** current_cmd = args;
    int    pipe_index  = 0;

    while (current_cmd != NULL)
    {
        // Find next command or end
        char** next_cmd = NULL;
        for (int i = 0; current_cmd[i] != NULL; i++)
        {
            if (strcmp(current_cmd[i], "|") == 0)
            {
                current_cmd[i] = NULL; // Terminate current command
                next_cmd       = &current_cmd[i + 1];
                break;
            }
        }

        // Create pipe for command chain
        if (next_cmd != NULL)
        {
            if (pipe(pipes[pipe_index % 2]) == -1)
            {
                perror("Pipe oluşturma hatası");
                return;
            }
        }

        pid = fork();
        if (pid == 0)
        { // Child process
            // Redirect input if not first command
            if (input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Redirect output to next pipe or final output
            if (next_cmd != NULL)
            {
                dup2(pipes[pipe_index % 2][1], STDOUT_FILENO);
                close(pipes[pipe_index % 2][0]);
                close(pipes[pipe_index % 2][1]);
            }
            else if (output_redirect)
            {
                int output_fd =
                    open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }

            // Execute command
            execvp(current_cmd[0], current_cmd);
            perror("Komut çalıştırılamadı");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("Fork hatası");
            return;
        }

        // Parent process
        if (input_fd != STDIN_FILENO)
        {
            close(input_fd);
        }

        if (next_cmd != NULL)
        {
            input_fd = pipes[pipe_index % 2][0];
            close(pipes[pipe_index % 2][1]);
            pipe_index++;
        }

        current_cmd = next_cmd;
    }

    // Wait for all child processes if not background
    if (!is_background)
    {
        while (wait(NULL) > 0)
            ;
    }
}
