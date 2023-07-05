#include <stdio.h>
#include "conf.h"

int main() {
    conf* myConfig = get_conf();

    if (myConfig != NULL) {
        printf("%s\n", myConfig->interface);
        printf("%s\n", myConfig->ip);
        printf("%d\n", myConfig->port);
        printf("%d\n", myConfig->childs);

        free_conf(myConfig);
    }

    return 0;
}