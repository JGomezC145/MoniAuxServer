#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/gui.h"
#include "serial.h"


int main(const int argc, char *argv[]) {
    //Verifica si se ejecuta como root /dev/uinput
    if (geteuid() != 0) {
        fprintf(stderr, "Este programa necesita ejecutarse como root.\n");
        exit(1);
    }

    char *port_name = SerialPortName;
    // detecta si se introduce "-p" seguido de un puerto serie
    if (argc > 1 && strcmp(argv[1], "-p") == 0 && argc > 2) {
        // Si se especifica un puerto, lo imprimimos
        printf("Puerto serie especificado: %s\n", argv[2]);
        port_name = argv[2];
    } else {
        // Si no se especifica, usamos el puerto por defecto
        printf("Usando puerto serie por defecto: /dev/ttyACM1\n");
        port_name = SerialPortName;
    }

    return gui_start(argc, argv, port_name);
}
