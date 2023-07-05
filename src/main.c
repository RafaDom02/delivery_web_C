#include <stdio.h>
#include <stdlib.h>

//Child creation and demonize proccess
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Signal behaviour
#include <signal.h>

#define MAX_PATH 100

void demonize();
void sigterm_handler(int signal);
void free_all();

int main(){
    system("clear");

    demonize();



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
    char* lastSlash;
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
#ifdef _WIN32
    // Windows-specific code to open the PID file
    f = fopen("..\\mydaemon.pid", "w");
#else
    // Linux-specific code to open the PID file
    f = fopen("../mydaemon.pid", "w");
#endif
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
 * @param signal signal recieved
 */
void sigterm_handler(int signal){
    free_all();
    exit(EXIT_SUCCESS);
}

/**
 * @brief Free all memory used and removes PID file
 * 
 */
void free_all(){
#ifdef _WIN32
    remove("..\\mydaemon.pid");
#else
    remove("../mydaemon.pid");
#endif

}