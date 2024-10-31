#ifndef _NDSH_H
#define _NDSH_H 1

#include "ndsh_config.h"

typedef struct {
    const char *name;
    int (*func)(int argc, char *argv[]);
} ndsh_cmd_t;

#ifndef PROMPT
#define PROMPT ">> "
#endif

#ifndef MAX_CMD_LEN
#define MAX_CMD_LEN 256
#endif

#ifndef MAX_CMD_ARGS
#define MAX_CMD_ARGS 32
#endif

#ifndef command_dispatcher
#define command_dispatcher static_command_dispatcher
extern ndsh_cmd_t command_list[];
extern size_t command_count;
#endif

int ndsh(int argc, char *argv[]);

#endif /* ifndef _NDSH_H */
