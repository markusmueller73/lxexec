/*
 * exec_gui_main.h
 *
 * Copyright 2017 Markus Müller <markus@markus-Linux>
 *
 */

#ifndef _EXEC_GUI_H_

#define _EXEC_GUI_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <libintl.h>
#include <locale.h>

#define DEBUG
#define _(STRING) gettext(STRING)
#define GETTEXT_PACKAGE "lxpanel"
#define LOCALE_DIR "/usr/share/locale/"

struct wnd_widgets_s{
    GtkWidget *combobox;
    GtkWidget *checkbox;
};

void dbg( char * , ... );
gboolean create_window ( GtkWidget ** );
gboolean edt_on_key_press (GtkWidget *, GdkEventKey *, gpointer );
gboolean wnd_on_key_press (GtkWidget *, GdkEventKey *, gpointer );
gboolean btn_exec_clicked (GtkWidget *, gpointer );
gboolean btn_canc_clicked (GtkWidget *, gpointer );

#endif
