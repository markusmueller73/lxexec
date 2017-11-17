/*
 * exec_gui_main.c
 *
 * Copyright 2017 Markus Müller <markus@markus-Linux>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "exec_gui_main.h"

int main( int argc , char *argv[] ){

	setlocale (LC_ALL, "");
	bindtextdomain( GETTEXT_PACKAGE, LOCALE_DIR );
	bind_textdomain_codeset( GETTEXT_PACKAGE, "UTF-8");
	textdomain ( GETTEXT_PACKAGE );

	dbg("GTK+ version %d.%d.%d found", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);

	if ( gtk_check_version( 2, 6, 0 ) != NULL ){
		fprintf( stderr, "Error - the GTK+ version is not compatible to this app.");
		return EXIT_FAILURE;
	}

	if ( gtk_init_check ( &argc, &argv ) == FALSE ){
		fprintf( stderr, "Error - can't initialize GTK+ 2.x.\n" );
		return EXIT_FAILURE;
	}

	GtkWidget *main_window = NULL;
	if ( create_window( &main_window ) == FALSE ){
		fprintf( stderr, "Error - can't create GTK+ toplevel window.\n" );
		return EXIT_FAILURE;
	}

	gtk_main();

	return EXIT_SUCCESS;
}

void dbg( char *msg , ... ){
    #ifdef DEBUG
    va_list args;
	fprintf( stdout, "DEBUG - " );
	va_start( args, msg );
	vfprintf( stdout, msg, args );
	va_end ( args );
	fprintf( stdout, "\n" );
    #endif
}

gboolean create_window ( GtkWidget **wnd ){

	GtkWidget *box_main = NULL;
	GtkWidget *box_text = NULL;
	GtkWidget *box_edit = NULL;
	GtkWidget *box_term = NULL;
	GtkWidget *box_btns = NULL;
	GtkWidget *txt = NULL;
	//GtkWidget *edt = NULL;
	//GtkWidget *chk = NULL;
	GtkWidget *btn_cancel = NULL;
	GtkWidget *btn_exec = NULL;
	struct wnd_widgets_s *wdgt = malloc( sizeof( struct wnd_widgets_s ) );

	*wnd = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	if ( *wnd == NULL )
		return FALSE;

	gtk_window_set_default_size( GTK_WINDOW( *wnd ), 400, -1 );
	gtk_window_set_title( GTK_WINDOW( *wnd ), _("Run") );
	gtk_window_set_icon_name( GTK_WINDOW( *wnd ), GTK_STOCK_EXECUTE );
	gtk_container_set_border_width( GTK_CONTAINER( *wnd ), 8 );

	g_signal_connect( *wnd, "destroy", G_CALLBACK( gtk_main_quit ), NULL );
    g_signal_connect( *wnd, "delete-event", G_CALLBACK( gtk_main_quit ), NULL );

   	box_main = gtk_vbox_new( FALSE, 4 );
	box_text = gtk_hbox_new( FALSE, 0 );
	box_edit = gtk_hbox_new( FALSE, 0 );
	box_term = gtk_hbox_new( FALSE, 0 );
	box_btns = gtk_hbox_new( FALSE, 0 );

	txt = gtk_label_new( _("Enter the command you want to execute:") );
	wdgt->combobox = gtk_combo_box_new_with_entry( );
	wdgt->checkbox = gtk_check_button_new_with_mnemonic( _("Open in _Terminal") );
	btn_cancel = gtk_button_new_with_label( "gtk-cancel" );
	btn_exec = gtk_button_new_with_label( "gtk-execute" );
	gtk_button_set_use_stock( GTK_BUTTON( btn_cancel ), TRUE );
	gtk_button_set_use_stock( GTK_BUTTON( btn_exec ), TRUE );

	g_signal_connect( G_OBJECT( wdgt->combobox ), "key_release_event", G_CALLBACK (edt_on_key_press), (gpointer) wdgt );
	g_signal_connect( G_OBJECT( btn_exec ), "clicked", G_CALLBACK (btn_exec_clicked), (gpointer) wdgt );
	g_signal_connect( G_OBJECT( btn_cancel ), "clicked", G_CALLBACK (btn_canc_clicked), NULL);

	gtk_container_add( GTK_CONTAINER( *wnd ), box_main );
	gtk_container_add( GTK_CONTAINER( box_main ), box_text );
	gtk_container_add( GTK_CONTAINER( box_main ), box_edit );
	gtk_container_add( GTK_CONTAINER( box_main ), box_term );
	gtk_container_add( GTK_CONTAINER( box_main ), box_btns );

	gtk_box_pack_start( GTK_BOX( box_text ), txt, FALSE, FALSE, 8 );
	gtk_box_pack_start( GTK_BOX( box_edit ), wdgt->combobox, TRUE, TRUE, 8 );
	gtk_box_pack_start( GTK_BOX( box_term ), wdgt->checkbox, FALSE, FALSE, 8 );
	gtk_box_pack_start( GTK_BOX( box_btns ), btn_cancel, TRUE, TRUE, 8 );
	gtk_box_pack_end( GTK_BOX( box_btns ), btn_exec, TRUE, TRUE, 8 );

	gtk_widget_show_all( *wnd );

	return TRUE;
}

gboolean edt_on_key_press( GtkWidget *widget, GdkEventKey *event, gpointer user_data )
{
	struct wnd_widgets_s *wdgt = (struct wnd_widgets_s *)user_data;
	char *command = gtk_combo_box_get_active_text( GTK_COMBO_BOX( wdgt->combobox ) );
	gboolean use_term = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( wdgt->checkbox ) );

	switch (event->keyval)
	{
	case GDK_Return:
		dbg("ENTER pressed [%s] [%d]", command, (int)use_term);
		break;
	case GDK_KP_Enter:
		dbg("Keypad ENTER pressed [%s] [%d]", command, (int)use_term);
		break;
	default:
	  return FALSE;
	}
	return TRUE;
}

gboolean btn_exec_clicked (GtkWidget *widget, gpointer user_data ){
	struct wnd_widgets_s *wdgt = (struct wnd_widgets_s *)user_data;
	char *command = gtk_combo_box_get_active_text( GTK_COMBO_BOX( wdgt->combobox ) );
	gboolean use_term = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( wdgt->checkbox ) );
	dbg("Execute button clicked [%s] [%d]", command, (int)use_term);
	return TRUE;
}
gboolean btn_canc_clicked (GtkWidget *widget, gpointer user_data ){
	dbg("Cancel button clicked");
	gtk_main_quit();
	return FALSE;
}
