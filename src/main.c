//#include <stdio.h>

#include <stdio.h>

#include "../include/gui.h"


int main(const int argc, char *argv[]) {
    // imprime el count de argumentos y los mismos:
    printf("Cantidad de argumentos: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argumento %d: %s\n", i, argv[i]);
    }

    return gui_start(argc, argv);
}
