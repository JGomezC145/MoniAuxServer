#include "serial.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static int serial_fd = -1;

int serial_open(const char *port_name) {
    printf("Abriendo puerto: %s\n", port_name);
    serial_fd = open(port_name, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd < 0) {
        perror("open");
        return -1;
    }

    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    cfsetospeed(&tty, SerialBaudRate);
    cfsetispeed(&tty, SerialBaudRate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 10;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int serial_read_line(char *buffer, int max_len) {
    int i = 0;
    char c;
    while (i < max_len - 1) {
        int n = read(serial_fd, &c, 1);
        if (n < 0) {
            perror("read");
            return -1;
        } else if (n == 0) {
            // No hay datos disponibles
            break;
        }
        if (n > 0) {
            if (c == '\n') break;
            buffer[i++] = c;
        } else {
            break;
        }
    }

    buffer[i] = '\0';
    if (i > 0) printf("Serial recibido: '%s'\n", buffer);
    return i;
}

void serial_close() {
    if (serial_fd >= 0) {
        close(serial_fd);
        serial_fd = -1;
    }
}
