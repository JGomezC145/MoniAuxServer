#ifndef KEYMAP_H
#define KEYMAP_H

int keymap_load(const char *filename);
int keymap_lookup(const char *command); // Devuelve keycode o -1 si no existe
int keymap_reload_or_create_default(const char *filename);


void keymap_clear();

#endif
