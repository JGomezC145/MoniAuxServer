#include "keymap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input-event-codes.h>
#include <sys/stat.h>
#include <gtk/gtk.h>

#define MAX_MAPPINGS 64

typedef struct {
    char command[32];
    int keycode;
} KeyMapping;

static KeyMapping mappings[MAX_MAPPINGS];
static int mapping_count = 0;

static struct {
    const char *name;
    int code;
} key_lookup_table[] = {
    {"KEY_A", 30},
    {"KEY_B", 48},
    {"KEY_C", 46},
    {"KEY_D", 32},
    {"KEY_E", 18},
    {"KEY_F", 33},
    {"KEY_G", 34},
    {"KEY_H", 35},
    {"KEY_I", 23},
    {"KEY_J", 36},
    {"KEY_K", 37},
    {"KEY_L", 38},
    {"KEY_M", 50},
    {"KEY_N", 49},
    {"KEY_O", 24},
    {"KEY_P", 25},
    {"KEY_Q", 16},
    {"KEY_R", 19},
    {"KEY_S", 31},
    {"KEY_T", 20},
    {"KEY_U", 22},
    {"KEY_V", 47},
    {"KEY_W", 17},
    {"KEY_X", 45},
    {"KEY_Y", 21},
    {"KEY_Z", 44},
    {"KEY_1", 2},
    {"KEY_2", 3},
    {"KEY_3", 4},
    {"KEY_4", 5},
    {"KEY_5", 6},
    {"KEY_6", 7},
    {"KEY_7", 8},
    {"KEY_8", 9},
    {"KEY_9", 10},
    {"KEY_0", 11},
    {"KEY_ENTER", 28},
    {"KEY_ESC", 1},
    {"KEY_BACKSPACE", 14},
    {"KEY_TAB", 15},
    {"KEY_SPACE", 57},
    {"KEY_MINUS", 12},
    {"KEY_EQUAL", 13},
    {"KEY_LEFTSHIFT", 42},
    {"KEY_RIGHTSHIFT", 54},
    {"KEY_LEFTCTRL", 29},
    {"KEY_RIGHTCTRL", 97},
    {"KEY_LEFTALT", 56},
    {"KEY_RIGHTALT", 100},
    {"KEY_CAPSLOCK", 58},
    {"KEY_F1", 59},
    {"KEY_F2", 60},
    {"KEY_F3", 61},
    {"KEY_F4", 62},
    {"KEY_F5", 63},
    {"KEY_F6", 64},
    {"KEY_F7", 65},
    {"KEY_F8", 66},
    {"KEY_F9", 67},
    {"KEY_F10", 68},
    {"KEY_F11", 87},
    {"KEY_F12", 88},
    {"KEY_F13", 183},
    {"KEY_F14", 184},
    {"KEY_F15", 185},
    {"KEY_MUTE", 113},
    {"KEY_VOLUMEUP", 115},
    {"KEY_VOLUMEDOWN", 114},
    {"KEY_MEDIA_PLAYPAUSE", 164},
    {"KEY_MEDIA_NEXT", 163},
    {"KEY_MEDIA_PREV", 165},
    {NULL, 0}
};


static int resolve_keycode(const char *keyname) {
    for (int i = 0; key_lookup_table[i].name != NULL; i++) {
        if (strcmp(key_lookup_table[i].name, keyname) == 0) {
            return key_lookup_table[i].code;
        }
    }
    return -1;
}

int keymap_load(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    char line[128];
    mapping_count = 0;

    while (fgets(line, sizeof(line), file) && mapping_count < MAX_MAPPINGS) {
        if (line[0] == '#' || strlen(line) < 3) continue;

        char *eq = strchr(line, '=');
        if (!eq) continue;

        *eq = '\0';
        char *cmd = line;
        char *key = eq + 1;

        // Limpia fin de línea
        cmd[strcspn(cmd, "\r\n")] = '\0';
        key[strcspn(key, "\r\n")] = '\0';

        int code = resolve_keycode(key);
        if (code != -1) {
            strncpy(mappings[mapping_count].command, cmd, sizeof(mappings[mapping_count].command));
            mappings[mapping_count].keycode = code;
            mapping_count++;
        }
    }

    fclose(file);
    return 0;
}



int keymap_reload_or_create_default(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        FILE *f = fopen(filename, "w");
        if (!f) {
            return -1;
        }

        fprintf(f,
            "# Botones físicos\n"
            "Btn1=KEY_F13\n"
            "Btn2=KEY_F14\n"
            "Btn3=KEY_F15\n"
            "#Btn4=define here\n"
            "#Btn5=define here\n"
            "#Btn6=define here\n"
            "#Btn7=define here\n"
            "#Btn8=define here\n"
            "Btn9=KEY_MEDIA_PREV\n"
            "BtnA=KEY_MEDIA_PLAYPAUSE\n"
            "BtnB=KEY_MEDIA_NEXT\n"
            "#BtnC=define here\n"
            "\n"
            "# Encoder\n"
            "EncoderR=KEY_VOLUMEUP\n"
            "EncoderL=KEY_VOLUMEDOWN\n"
            "EncoderSW=KEY_MUTE\n"
        );

        fclose(f);
    }

    return keymap_load(filename);
}



int keymap_lookup(const char *command) {
    for (int i = 0; i < mapping_count; i++) {
        if (strcmp(mappings[i].command, command) == 0) {
            return mappings[i].keycode;
        }
    }
    return -1;
}

void keymap_clear() {
    mapping_count = 0;
}
