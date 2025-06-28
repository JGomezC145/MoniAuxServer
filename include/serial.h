//
// Created by jgome on 27/6/2025.
//

#ifndef SERIAL_H
#define SERIAL_H

#define SerialBaudRate B115200

int serial_open(const char *port_name);
int serial_read_line(char *buffer, int max_len);
void serial_close();

#endif //SERIAL_H
