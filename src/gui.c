//
// Created by jgomez on 20/6/2025.
//

#include "gui.h"
#include <gtk/gtk.h>

// Esta función se ejecuta cuando se presiona el botón
static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);
    gtk_label_set_text(label, "Botón presionado");
}


int gui_start(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    // Crear ventana principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MoniAuxServer");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Crear contenedor vertical
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Crear etiqueta
    label = gtk_label_new("Esperando...");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    // Crear botón
    button = gtk_button_new_with_label("Enviar");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Conectar botón a acción
    g_signal_connect(button, "clicked", G_CALLBACK(on_send_button_clicked), label);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

