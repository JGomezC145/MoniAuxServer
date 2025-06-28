#include "gui.h"
#include "serial.h"
#include <gtk/gtk.h>
#include <string.h>

#include "keymap.h"
#include "keysim.h"

static GtkLabel *status_label;

static gboolean poll_serial(gpointer user_data) {
    char buffer[128];
    int n = serial_read_line(buffer, sizeof(buffer));
    // Limpiar '\r' si existe
    char *p = strchr(buffer, '\r');
    if (p) *p = '\0';

    int keycode = keymap_lookup(buffer);
    if (keycode != -1) {
        gtk_label_set_text(status_label, buffer);
        keysim_send(keycode);
    } else {
        gtk_label_set_text(status_label, "Comando no mapeado");
    }



    /* if (n > 0) {
    //     //printf("Recibido: >>>%s<<<\n", buffer);  // Para depurar en terminal
    //
    //     if (strncmp(buffer, "Btn", 3) == 0 || strncmp(buffer, "Encoder", 7) == 0) {
    //         gtk_label_set_text(status_label, buffer);
    //     }
    //
    //     if (strcmp(buffer, "Btn1") == 0) {
    //         gtk_label_set_text(status_label, "Botón 1");
    //         keysim_send(KEY_B);
    //     } else if (strcmp(buffer, "Btn2") == 0) {
    //         gtk_label_set_text(status_label, "Botón 2");
    //         keysim_send(KEY_ENTER);
    //     } else if (strcmp(buffer, "Btn6") == 0) {
    //         gtk_label_set_text(status_label, "Botón 2");
    //         keysim_send(KEY_F5);
    //     } else if (strcmp(buffer, "EncoderL") == 0) {
    //         gtk_label_set_text(status_label, "Encoder izquierda");
    //         keysim_send(KEY_VOLUMEDOWN);
    //     } else if (strcmp(buffer, "EncoderR") == 0) {
    //         gtk_label_set_text(status_label, "Encoder derecha");
    //         keysim_send(KEY_VOLUMEUP);
    //     } else if (strcmp(buffer, "EncoderSW") == 0) {
    //         gtk_label_set_text(status_label, "Encoder presionado");
    //         keysim_send(KEY_PLAYPAUSE);
    //     }
    //
     }*/
    return TRUE;
}

//reload keymap
static void on_reload_clicked(GtkButton *btn, gpointer user_data) {
    int result = keymap_reload_or_create_default("keymap.cfg");
    if (result == 0)
        gtk_label_set_text(status_label, "Mapeo cargado correctamente");
    else
        gtk_label_set_text(status_label, "Error cargando keymap.cfg");

}


// Crea una funcion para cerrar la app
static void close_app(GtkWidget *widget, gpointer data) {
    serial_close();
    keysim_close();
    serial_close();
    gtk_main_quit();
}

int gui_start(int argc, char *argv[], const char *serialPort) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MoniAuxServer");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *label = gtk_label_new("Esperando...");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    status_label = GTK_LABEL(label);

    GtkWidget *button = gtk_button_new_with_label("Enviar");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    GtkWidget *reload_button = gtk_button_new_with_label("Recargar mapeo");
    gtk_box_pack_start(GTK_BOX(vbox), reload_button, FALSE, FALSE, 0);


    g_signal_connect(reload_button, "clicked", G_CALLBACK(on_reload_clicked), NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(close_app), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(close_app), NULL);

    gtk_widget_show_all(window);

    {
        int result = keymap_reload_or_create_default("keymap.cfg");
        if (result == 0)
            gtk_label_set_text(status_label, "Mapeo cargado correctamente");
        else
            gtk_label_set_text(status_label, "Error cargando keymap.cfg");

    }

    if (keymap_load("keymap.cfg") != 0) {
        gtk_label_set_text(status_label, "No se pudo cargar keymap.cfg");
    }

    if (serial_open(serialPort) != 0) {
        gtk_label_set_text(GTK_LABEL(label), "Error abriendo /dev/ttyACM0");
    } else {
        g_timeout_add(100, poll_serial, NULL);
    }

    if (keysim_init() != 0) {
        gtk_label_set_text(status_label, "Error con uinput");
    }


    gtk_main();

    serial_close();
    keysim_close();
    serial_close();
    return 0;
}
