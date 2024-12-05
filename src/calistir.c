#include "../include/calistir.h"

void execute_single_command(char** args,
                            char*  input_file,
                            char*  output_file,
                            int    is_background);

void execute_pipeline(char** args,
                      char*  input_file,
                      char*  output_file,
                      int    is_background);

/**
 * @brief Kullanıcının girdiği komutu çalıştırır.
 * Tekli komutları, pipe'ları ve redirection'lardan sorumludur.
 * @param args Komut ve argümanlarını içeren dizi.
 */
void calistir(char** args)
{
    if (args == NULL || args[0] == NULL)
    {
        return; // No command to run
    }

    // Variables for redirection, pipes, and background
    char* input_file    = NULL;
    char* output_file   = NULL;
    int   is_background = 0;
    int   num_pipes     = 0;

    // Parse arguments for redirection, pipes, and background
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0 && args[i + 1] != NULL)
        {
            input_file = args[i + 1];
            args[i]    = NULL;
        }
        else if (strcmp(args[i], ">") == 0 && args[i + 1] != NULL)
        {
            output_file = args[i + 1];
            args[i]     = NULL;
        }
        else if (strcmp(args[i], "&") == 0)
        {
            is_background = 1;
            args[i]       = NULL;
        }
        else if (strcmp(args[i], "|") == 0)
        {
            num_pipes++;
        }
    }

    // Single command or pipeline handling
    if (num_pipes == 0)
    {
        execute_single_command(args, input_file, output_file, is_background);
    }
    else
    {
        execute_pipeline(args,
                         //num_pipes,
                         input_file,
                         output_file,
                         is_background);
    }
}

void execute_single_command(char** args,
                            char*  input_file,
                            char*  output_file,
                            int    is_background)
{
    // Check for the custom "increment" command
    if (strcmp(args[0], "increment") == 0)
    {
        // Redirect input if necessary
        if (input_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("Input file error");
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Call the increment function
        _increment(args);
        return;
    }

    pid_t pid = fork();
    if (pid == 0)
    { // Child process
        // Handle input redirection
        if (input_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("Input file error");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Handle output redirection
        if (output_file)
        {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Output file error");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Execute the command
        execvp(args[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("Fork error");
    }
    else
    {
        // Parent process
        if (!is_background)
        {
            waitpid(pid, NULL, 0); // Wait for child process
        }
    }
}

void execute_pipeline(char** args,
                      //int    num_pipes,
                      char*  input_file,
                      char*  output_file,
                      int    is_background)
{
    int   pipes[2][2];
    int   input_fd = STDIN_FILENO;
    pid_t pid;
    int   cmd_index = 0;

    while (args[cmd_index] != NULL)
    {
        // Find the next command
        char* current_cmd[128];
        int   i = 0;
        while (args[cmd_index] != NULL && strcmp(args[cmd_index], "|") != 0)
        {
            current_cmd[i++] = args[cmd_index++];
        }
        current_cmd[i] = NULL;

        // Skip the pipe symbol
        if (args[cmd_index] != NULL)
            cmd_index++;

        // Create a pipe if needed
        if (args[cmd_index] != NULL && pipe(pipes[cmd_index % 2]) < 0)
        {
            perror("Pipe creation error");
            return;
        }

        pid = fork();
        if (pid == 0)
        { // Child process
            // Redirect input from previous pipe or input file
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
                    perror("Input file error");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Handle output to the next pipe or final output
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
                    perror("Output file error");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // Check for the custom "increment" command
            if (strcmp(current_cmd[0], "increment") == 0)
            {
                _increment(current_cmd);
                exit(EXIT_SUCCESS);
            }

            execvp(current_cmd[0], current_cmd);
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("Fork error");
            return;
        }

        // Parent process: Close pipes and update input_fd
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
