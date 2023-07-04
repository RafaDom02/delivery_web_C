#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void killDemonized(){
    FILE *f;
    pid_t pid;

#ifdef _WIN32
    // Windows-specific code to open the PID file
    f = fopen("C:\\Temp\\mydaemon.pid", "r");
#else
    // Linux-specific code to open the PID file
    f = fopen("/var/run/mydaemon.pid", "r");
#endif
    if(!f){
        printf("Could not open that file.\n");
        return;
    }

    //Read demonized proccess pid from the file
    fscanf(f, "%d", &pid);

    //To-Do: Send signal
    kill(pid, SIGTERM);

#ifdef _WIN32
    remove("C:\\Temp\\mydaemon.pid");
#else
    remove("/var/run/mydaemon.pid");
#endif
}

int main(){
    killDemonized();
    return 0;
}