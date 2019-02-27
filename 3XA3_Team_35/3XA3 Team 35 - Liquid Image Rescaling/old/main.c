#include <gtk-3.0/gtk/gtk.h>

// gcc -rdynamic -o LiquidRescaleApp main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

GtkBuilder      *builder; 
GtkWidget       *window; 
GtkWidget       *grdControls;
GtkWidget       *dlgLoadImage;
//GtkStyleContext *context;
GtkCssProvider  *cssProvider;
GdkDisplay      *display;
GdkScreen       *screen;

int main(int argc, char *argv[])
{

    cssProvider = gtk_css_provider_new ();
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    grdControls = GTK_WIDGET(gtk_builder_get_object(builder, "grdControls"));
    dlgLoadImage = GTK_WIDGET(gtk_builder_get_object(builder, "dlgLoadImage"));
    
    /*
    context = gtk_widget_get_style_context(grdControls); 
    gtk_css_provider_load_from_path (cssProvider,
                                    "./style.css",
                                    NULL);
    gtk_style_context_add_provider (context,
                                    GTK_STYLE_PROVIDER(cssProvider),
                                    GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_save (context);
    */

    cssProvider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
 
    gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);

    gtk_builder_connect_signals(builder, NULL);
    
    g_object_unref(builder);
    g_object_unref (cssProvider);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}
 
// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}

void on_load_image() {

    //gtk_dialog_run(GTK_DIALOG(dlgLoadImage));
    //gtk_widget_hide(dlgLoadImage);
    Gtk::FileChooserDialog dialog("Open image",
	                              Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.add_button (Gtk::Stock::OPEN,
	                   Gtk::RESPONSE_ACCEPT);
	dialog.add_button (Gtk::Stock::CANCEL,
	                   Gtk::RESPONSE_CANCEL);

	Glib::RefPtr<Gtk::FileFilter> filter =
		Gtk::FileFilter::create();
	filter->add_pixbuf_formats();
	filter->set_name("Images");
	dialog.add_filter (filter);

	const int response = dialog.run();
	dialog.hide();

	switch (response)
	{
		case Gtk::RESPONSE_ACCEPT:
			image->set(dialog.get_filename());
			break;
		default:
			break;
    }
        
}

void on_btnLoadClose_clicked() {
    //gtk_widget_hide(dlgLoadImage);
}
void on_btnLoadOpen_clicked() {
    
}

void on_save_image() {

}