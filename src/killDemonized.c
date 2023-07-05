#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void killDemonized();

int main(){
    killDemonized();
    return 0;
}

/**
 * @brief Read the demonized proccess PID and kills it
 * 
 */
void killDemonized(){
    FILE *f;
    pid_t pid;

#ifdef _WIN32
    // Windows-specific code to open the PID file
    f = fopen("..\\mydaemon.pid", "r");
#else
    // Linux-specific code to open the PID file
    f = fopen("../mydaemon.pid", "r");
#endif
    if(!f){
        printf("Could not open that file.\n");
        return;
    }

    //Read demonized proccess pid from the file
    fscanf(f, "%d", &pid);

    printf("Killing proccess with pid %d\n", pid);
    //To-Do: Send signal
    kill(pid, SIGTERM);
}