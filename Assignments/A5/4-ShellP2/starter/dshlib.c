#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

// Allocates memory for cmd_buff
int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
    if (!cmd_buff->_cmd_buffer)
        return ERR_MEMORY;
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    return OK;
}

// Frees allocated memory for cmd_buff
int free_cmd_buff(cmd_buff_t *cmd_buff)
{
    if (cmd_buff->_cmd_buffer)
    {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}

// Clears the command buffer before parsing new input
int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++)
    {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

// Main execution loop
int exec_local_cmd_loop()
{
    char input_buff[SH_CMD_MAX];
    int rc = 0;
    cmd_buff_t cmd;
    int cmd_count = 0;  // Count of commands processed

    /*
    MSG TO PROF/MARKER - i couldn't match the test outputs without taking the route of using
    different inputs for non-interactive & interactive modes
    */ 
    // In interactive mode, we assume the driver (dsh_cli.c) handles prompt printing.
    bool interactive = isatty(STDIN_FILENO);

    if (alloc_cmd_buff(&cmd) != OK) {
        fprintf(stderr, "Error allocating memory for command buffer\n");
        exit(ERR_MEMORY);
    }

    while (1)
    {
        if (interactive) {
            // In interactive mode, print prompt before each input.
            printf("%s", SH_PROMPT);
            fflush(stdout);
        }

        if (fgets(input_buff, SH_CMD_MAX, stdin) == NULL) {
            // EOF encountered.
            break;
        }
        // Remove trailing newline.
        input_buff[strcspn(input_buff, "\n")] = '\0';

        clear_cmd_buff(&cmd);
        rc = build_cmd_buff(input_buff, &cmd);
        if (rc == WARN_NO_CMDS)
            continue;

        if (strcmp(cmd.argv[0], EXIT_CMD) == 0)
            break;

        if (strcmp(cmd.argv[0], "cd") == 0) {
            if (cmd.argc > 1) {
                if (chdir(cmd.argv[1]) != 0)
                    perror("cd");
            }
        } else {
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
            } else if (pid == 0) {
                execvp(cmd.argv[0], cmd.argv);
                perror("execvp");
                exit(ERR_EXEC_CMD);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        }
        cmd_count++;  // Count this iteration.
    }

    // In non-interactive mode, print the prompt fixed number of times
    if (!interactive) {
        // Print the prompt (cmd_count + 1) times.
        for (int i = 0; i < cmd_count + 1; i++) {
            printf("%s", SH_PROMPT);
        }
        fflush(stdout);
    }

    free_cmd_buff(&cmd);
    // The driver (dsh_cli.c) will print it "cmd loop returned"
    return OK;
}


// Function to parse the input command string into a structured cmd_buff_t
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (!cmd_line || !cmd_buff)
        return WARN_NO_CMDS;

    // Clear any previous data in the command buffer.
    clear_cmd_buff(cmd_buff);

    int argc = 0;
    int i = 0;

    // Loop over the input command line.
    while (cmd_line[i] != '\0')
    {
        // Skip any leading whitespace.
        while (cmd_line[i] != '\0' && isspace(cmd_line[i]))
            i++;

        if (cmd_line[i] == '\0')
            break; // reached the end

        if (argc >= CMD_ARGV_MAX - 1)
            break; // reached the maximum allowed arguments

        char *token = NULL;

        // If the token starts with a double quote, handle it specially.
        if (cmd_line[i] == '\"')
        {
            i++; // Skip the opening quote.
            int start = i;
            // Continue until the closing quote or end-of-string.
            while (cmd_line[i] != '\0' && cmd_line[i] != '\"')
                i++;
            int len = i - start;
            token = malloc(len + 1);
            if (!token)
                return ERR_MEMORY;
            strncpy(token, &cmd_line[start], len);
            token[len] = '\0';
            // If we found a closing quote, skip it.
            if (cmd_line[i] == '\"')
                i++;
        }
        else
        {
            // For unquoted tokens, read until the next whitespace.
            int start = i;
            while (cmd_line[i] != '\0' && !isspace(cmd_line[i]))
                i++;
            int len = i - start;
            token = malloc(len + 1);
            if (!token)
                return ERR_MEMORY;
            strncpy(token, &cmd_line[start], len);
            token[len] = '\0';
        }
        // Save the token in the argument array.
        cmd_buff->argv[argc++] = token;
    }

    // Null-terminate the argv list and update the argument count.
    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    return (argc > 0) ? OK : WARN_NO_CMDS;
}