#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "ndsh.h"

int demo_cmd1(int argc, char *argv[])
{
    printf("This is demo command 1\n");
    return 0;
}

int demo_cmd2(int argc, char *argv[])
{
    printf("This is demo command 2\n");
    return 0;
}

ndsh_cmd_t command_list[] = {
    {
        .name = "cmd1",
        .func = demo_cmd1,
    },
    {
        .name = "cmd2",
        .func = demo_cmd2,
    },
};

size_t command_count = sizeof(command_list) / sizeof(ndsh_cmd_t);

int main(int argc, char *argv[])
{
    struct termios termios_new;
    struct termios termios_orig;

    tcgetattr(STDIN_FILENO, &termios_orig);
    tcgetattr(STDIN_FILENO, &termios_new);

    termios_new.c_lflag &= ~(ICANON);
    termios_new.c_lflag &= ~(ECHO);
    termios_new.c_cc[VMIN] = 1;
    termios_new.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_new);

    int retcode = ndsh(argc, argv);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_orig);
    return retcode;
}