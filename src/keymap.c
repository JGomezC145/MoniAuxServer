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
    {"KEY_F13", KEY_F13},
    {"KEY_F14", KEY_F14},
    {"KEY_F15", KEY_F15},
    {"KEY_F16", KEY_F16},
    {"KEY_VOLUMEUP", KEY_VOLUMEUP},
    {"KEY_VOLUMEDOWN", KEY_VOLUMEDOWN},
    {"KEY_MUTE", KEY_MUTE},
    {"KEY_MEDIA_PLAYPAUSE", KEY_PLAYPAUSE},
    {"KEY_MEDIA_NEXT", KEY_NEXTSONG},
    {"KEY_MEDIA_PREV", KEY_PREVIOUSSONG},
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
            "Btn9=KEY_MEDIA_PLAYPAUSE\n"
            "BtnA=KEY_MEDIA_NEXT\n"
            "BtnB=KEY_MEDIA_PREV\n"
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
