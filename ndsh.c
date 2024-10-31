#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "ndsh.h"

static int static_command_dispatcher(int argc, char *argv[])
{
    for (size_t i = 0; i < command_count; i++)
    {
        if (strcmp(argv[0], command_list[i].name) == 0)
        {
            return command_list[i].func(argc, argv);
        }
    }
    return -1;
}

static int _run_command(char *buffer)
{
    char *argv[MAX_CMD_ARGS];

    int argc = 0;
    char *state;
    
    argv[0] = NULL;
    
    char *tok = strtok_r(buffer, " ", &state);
    while (tok)
    {
        argv[argc++] = tok;
        argv[argc] = NULL;
        if (argc == (MAX_CMD_ARGS - 1))
        {
            break;
        }
        tok = strtok_r(NULL, " ", &state);
    }
    printf("argc = %d, argv = [", argc);
    for (int i=0;i<argc;i++)
    {
        printf("\"%s\"", argv[i]);
        if (i < (argc - 1))
        {
            printf(", ");
        }
    }
    printf("]\n");

    if (argc > 0)
    {
        // lookup the command by name
        return command_dispatcher(argc, argv);
    }
    return 0;
}

int ndsh(int argc, char *argv[])
{
    int exit_code = -1;
    char *buffer = (char *)malloc(MAX_CMD_LEN);
    if (buffer)
    {
        char c;
        char old_c = '\0';
        buffer[0] = '\0';
        int cursor = 0;
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
        while (1)
        {
            int nread;
            if ((nread = read(STDIN_FILENO, &c, 1)) != 1)
            {
                return nread;
            }

            if (((c == '\n') && (old_c != '\r')) || (c == '\r'))
            {
                write(STDOUT_FILENO, "\n", 1);
                exit_code = _run_command(buffer);
                if (exit_code) {
                    break;
                }
                buffer[0] = '\0';
                cursor = 0;
                write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
            }
            else if (((c == '\x08') || (c == '\x7f')))
            {
                if (cursor > 0)
                {
                    // it's a backspace - write a backspace then a space to cover what was there then back again
                    write(STDOUT_FILENO, "\x08 \x08", 3);
                    // and remove the character from the buffer
                    cursor--;
                    buffer[cursor] = '\0';
                }
            }
            else if (c == '\x1b')
            {
                // escape sequence
                nread = read(STDIN_FILENO, &c, 1);
                if (nread != 1)
                {
                    exit_code = nread;
                    break;
                }
                if (c == '[')
                {
                    nread = read(STDIN_FILENO, &c, 1);
                    if (nread != 1)
                    {
                        exit_code = nread;
                        break;
                    }
                    if (c == 'A')
                    {
                        // Up
                    }
                    else if (c == 'B')
                    {
                        // Down
                    }
                    else if (c == 'C')
                    {
                        // right

                    }
                    else if (c == 'D')
                    {
                        // left

                    }
                }
            }
            else if (c == '\x04')
            {
                // Ctrl - D (end of file)
                exit_code = 0;
                break;
            }
            else if ((c >= ' ') && (c <= '~'))
            {
                if (cursor < (MAX_CMD_LEN - 1))
                {
                    write(STDOUT_FILENO, &c, 1);
                    buffer[cursor++] = c;
                    buffer[cursor] = '\0';
                }
            }
            else
            {
                printf("Unhandled character: 0x%02x\n", (unsigned int)c);
            }
            old_c = c;
        }
        free(buffer);
    }
    return exit_code;
}