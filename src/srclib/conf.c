#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf.h"

#define CONF_PATH "server.conf"
#define MAX_LINE 20

void get_data(conf* config);
void assign_value(conf* config, const char* line, const enum value val);
char* obtainValue(const char* line);
void removeNewLine(char* str);
conf* get_conf();
void free_conf(conf* config);

conf* get_conf() {
    conf* config = (conf*)malloc(sizeof(conf));
    if (!config) return NULL;

    get_data(config);

    return config;
}

void get_data(conf* config) {
    FILE* f;
    char line[MAX_LINE];
    enum value val = INTERFACE;

    f = fopen(CONF_PATH, "r");

    if (!f) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    while (fgets(line, MAX_LINE, f) != NULL) {
        assign_value(config, line, val);
        val++;
    }

    fclose(f);
}

void assign_value(conf* config, const char* line, const enum value val) {
    int port, childs;
    char *aux;

    if (val == INTERFACE) {
        config->interface = obtainValue(line);
        if (!(config->interface)) return;

        removeNewLine(config->interface);
    } 
    else if (val == IP) {
        config->ip = obtainValue(line);
        if (!(config->ip)) return;

        removeNewLine(config->ip);
    } 
    else if (val == PORT) {
        aux = obtainValue(line);
        port = atoi(aux);
        config->port = port;
        free(aux);
    } 
    else if (val == CHILDS) {
        aux = obtainValue(line);
        childs = atoi(aux);
        config->childs = childs;
        free(aux);
    }
}

char* obtainValue(const char* line) {
    char* value = NULL;

    // Find the equal sign in the string
    const char* equalSign = strchr(line, '=');

    if (equalSign != NULL) {
        // Find the first space after the equal sign
        const char* space = strchr(equalSign, ' ');

        if (space != NULL) {
            // Calculate the length of the value between the equal sign and the space
            size_t length = space - (equalSign + 1);

            // Allocate memory for the value
            value = (char*)malloc((length + 1) * sizeof(char));

            if (value != NULL) {
                // Copy the value between the equal sign and the space to the new string
                strncpy(value, equalSign + 1, length);
                value[length] = '\0';
            }
        } else {
            // No space found, assume the string continues until the end
            size_t length = strlen(equalSign + 1);

            // Allocate memory for the value
            value = (char*)malloc((length + 1) * sizeof(char));

            if (value != NULL) {
                // Copy the value after the equal sign to the new string
                strcpy(value, equalSign + 1);
            }
        }
    }

    return value;
}

void removeNewLine(char* str) {
    char* newline = strchr(str, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
}

void free_conf(conf* config){
    if (!config) return;
    if (config->interface) free(config->interface);
    if (config->ip) free(config->ip);
    free(config);
}

/*int main() {
    conf* myConfig = get_conf();

    if (myConfig != NULL) {
        printf("%s\n", myConfig->interface);
        printf("%s\n", myConfig->ip);
        printf("%d\n", myConfig->port);
        printf("%d\n", myConfig->childs);

        free_conf(myConfig);
    }

    return 0;
}*/
