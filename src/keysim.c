#include "keysim.h"
#include <fcntl.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static int uinput_fd = -1;

int keysim_init() {
    struct uinput_setup usetup;

    uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0) {
        perror("open /dev/uinput");
        return -1;
    }

    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    for (int i = KEY_ESC; i <= KEY_MICMUTE; i++) {
        ioctl(uinput_fd, UI_SET_KEYBIT, i);
    }

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor  = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "MoniAuxServer Virtual Keyboard");

    ioctl(uinput_fd, UI_DEV_SETUP, &usetup);
    ioctl(uinput_fd, UI_DEV_CREATE);

    usleep(100000); // darle tiempo a crear

    return 0;
}

void keysim_close() {
    if (uinput_fd >= 0) {
        ioctl(uinput_fd, UI_DEV_DESTROY);
        close(uinput_fd);
        uinput_fd = -1;
    }
}

int keysim_send(int keycode) {
    if (uinput_fd < 0) return -1;

    struct input_event ev;

    memset(&ev, 0, sizeof(ev));
    ev.type = EV_KEY;
    ev.code = keycode;
    ev.value = 1;
    write(uinput_fd, &ev, sizeof(ev));

    ev.value = 0;
    write(uinput_fd, &ev, sizeof(ev));

    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    write(uinput_fd, &ev, sizeof(ev));

    return 0;
}
