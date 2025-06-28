#include "gui.h"
#include "serial.h"
#include <gtk/gtk.h>
#include <string.h>

static GtkLabel *status_label;

static gboolean poll_serial(gpointer user_data) {
    char buffer[128];
    int n = serial_read_line(buffer, sizeof(buffer));
    if (n > 0) {
        if (strcmp(buffer, "Btn1") == 0) {
            gtk_label_set_text(status_label, "Botón físico presionado");
            printf("Catched! %s\n", buffer);
        }
    }
    return TRUE;
}

int gui_start(int argc, char *argv[]) {
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

    g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    if (serial_open("/dev/ttyACM0") != 0) {
        gtk_label_set_text(GTK_LABEL(label), "Error abriendo /dev/ttyACM0");
    } else {
        g_timeout_add(100, poll_serial, NULL);
    }

    gtk_widget_show_all(window);
    gtk_main();

    serial_close();
    return 0;
}
