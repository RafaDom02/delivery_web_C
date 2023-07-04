#ifndef _CONF_H
#define _CONF_H

#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <locale.h>

typedef struct
{
    char* interface;
    char* ip;
    int port;
    int childs;
}conf;

enum value {
    INTERFACE,
    IP,
    PORT,
    CHILDS
};


conf* get_conf();

#endif