#include <stdio.h>
#include <stdlib.h>

//Child creation and demonize proccess
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Signal behaviour
#include <signal.h>

/*************SELF CREATED LIBRARIES***************/
//Gets configuration written in server.conf
#include "conf.h"

#define MAX_PATH 100

#ifdef _WIN32
    #define PID_FILE ".\\mydaemon.pid"
#else
    #define PID_FILE "./mydaemon.pid"
#endif

void demonize();
void sigterm_handler();
void free_all();

/*************GLOBAL VARIABLES***************/
conf* config = NULL;

int main(){

    system("clear");

    demonize();

    config = get_conf();

    while(1);

    free_all();
    return EXIT_SUCCESS;
}

/**
 * @brief Creates a demonized child, killing the father and
 * giving the child the necesary permissions
 * 
 */
void demonize(){
    pid_t pid = fork();
    char cwd[MAX_PATH];
    FILE * f;
    
    //Error while forking
    if (pid < 0) exit(EXIT_FAILURE);
    //Kill the father proccess
    if (pid > 0) exit(EXIT_SUCCESS);
    //Creating a new session for the child
    if (setsid() < 0) exit(EXIT_FAILURE);

    //Close standard file descriptors (security)
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

#ifdef _WIN32
    char *lastSlash;
    // Windows-specific code to get the base directory
    // Adapt the path as needed
    if (GetModuleFileName(NULL, cwd, sizeof(cwd)) == 0) {
        exit(EXIT_FAILURE);
    }

    // Remove the executable filename to get the base directory
    lastSlash = strrchr(cwd, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0';
    }
#else
    // Linux-specific code to get the base directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        exit(EXIT_FAILURE);
    }
#endif

    // Change base directory
    if(chdir(cwd) != 0) exit(EXIT_FAILURE);

    // To-Do: Set desired permissions
    umask(0);

    //Store PID in a file

    // Windows-specific code to open the PID file
    f = fopen(PID_FILE, "w");

    if (!f){
        printf("PID file could not bo opened");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "%d", getpid());

    //Close PID file
    fclose(f);

    //Select signal handler for killing the demonized proccess
    signal(SIGTERM, sigterm_handler);
}

/**
 * @brief Signal handler to finalize the demonized proccess
 * 
 */
void sigterm_handler(){
    free_all();
    exit(EXIT_SUCCESS);
}

/**
 * @brief Free all memory used and removes PID file
 * 
 */
void free_all(){
    
    if(access(PID_FILE, F_OK) == 0) remove(PID_FILE);
    
    if(config) free_conf(config);
}