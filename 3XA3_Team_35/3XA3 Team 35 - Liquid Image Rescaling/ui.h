
#ifndef __UI__
#define __UI__

#include <gtkmm-3.0/gtkmm.h>
#include <glib.h>
#include <iostream>
#include "draw.h"
#include "dialog.h"
#include "rescale.h"

using namespace Gtk;
using namespace std;
using namespace Glib;

namespace UI {

    const string 		STYLESHEET_LOCATION = "./style.css";
    const string 		IMAGE_FILTER_NAME = "Images";
    const string 		DEFAULT_EXTENSION = "png";
    const int 			IMAGE_BUFFER = 50;

    Window 				*wndMain;
    Button 				*btnLoadImage, 
                		*btnSaveImage, 
                		*btnScale;
    SpinButton  		*spnWidth,
                		*spnHeight;
    Grid        		*grdMainLayout, 
                		*grdControls;
    Image       		*imgDisplay;

    RefPtr<Gdk::Pixbuf> imageSrc;
    RefPtr<Gdk::Pixbuf>	imageOut;
	RefPtr<FileFilter> 	filter;
	
	static Main kit;

	static bool running = false;
	static bool initialized = false;

	// FUNCTION DECLARATIONS
	static int run();
	static int init();

	static int init_filters();
	static int init_ui();
	static int init_styles(string fname);
	static int init_handlers();   

    static void on_load_image();
    static void on_save_image();
	static void on_scale_image();

	static int run() {

		// Don't run if already running.
		if (running) return 2;
		
		// Start program.
		running = true;
		kit.run(*wndMain);
		running = false;

		return 0;
	}

	static int init() {
		
		// Don't initialize if running.
		if (running) return 2;

		initialized = false;

		// Initialize UI.
		if (init_ui()) return 1;
		if (init_filters()) return 1;
		if (init_styles(STYLESHEET_LOCATION)) return 1;
		if (init_handlers()) return 1;

		// Disable save and scale buttons by default (no image to save on startup).
		btnSaveImage->set_sensitive(false);
		btnScale->set_sensitive(false);

		initialized = true;
		return 0;
	}

	static int init_ui() {

		// Load the Glade file:
		RefPtr<Builder> builder;
		try
		{
			builder = Builder::create_from_file("window_main.glade");
		}
		catch (const FileError & ex)
		{
			std::cerr << ex.what() << std::endl;
			return 1;
		}
		
		// Get widgets (UI components) from builder.
		builder->get_widget("wndMain", wndMain);
		builder->get_widget("btnLoadImage", btnLoadImage);
		builder->get_widget("btnSaveImage", btnSaveImage);
		builder->get_widget("btnScale", btnScale);
		builder->get_widget("spnWidth", spnWidth);
		builder->get_widget("spnHeight", spnHeight);
		builder->get_widget("grdMainLayout", grdMainLayout);
		builder->get_widget("grdControls", grdControls);
		builder->get_widget("imgDisplay", imgDisplay);

		return 0;
	}

	static int init_filters() {

		filter = Gtk::FileFilter::create();
		filter->add_pixbuf_formats();
		filter->set_name(IMAGE_FILTER_NAME);

		return 0;
	}

	static int init_styles(string fname) {

		// Create the style objects for the UI components.
		RefPtr<CssProvider> css_provider = CssProvider::create();
		RefPtr<StyleContext> style_context = StyleContext::create();

		// Load the CSS styles for the UI components.
		try {
			if(css_provider->load_from_path(fname)){
				RefPtr<Gdk::Screen> screen = wndMain->get_screen();
				style_context->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
			}
		}
		catch (const FileError & ex) {
			return 1;
		}

		return 0;
	}

	static int init_handlers() {

		// Try to link UI signals to event handlers.
		try {
			btnLoadImage->signal_clicked().connect (sigc::ptr_fun(&on_load_image));
			btnSaveImage->signal_clicked().connect (sigc::ptr_fun(&on_save_image));
			btnScale->signal_clicked().connect (sigc::ptr_fun(&on_scale_image));
		} catch (exception & e) {
			return 1;
		}

		return 0;
	}   

    static void on_load_image() {

		// Create variable to store file name in.
		string filename;

		// Get file name from dialog. If unsuccessful, return.
		if (!PromptDialog::openImageDialog(*wndMain, filter, filename)) return;

		// Load the image file chosen by the user.
		RefPtr<Gdk::Pixbuf> temp = Gdk::Pixbuf::create_from_file(filename);

		// Force image size requirements.
		if (temp->get_width() < 2 || temp->get_height() < 2) {
			PromptDialog::displayMessage(*wndMain, "File size too small. Must be at least 2x2 pixels.");
			return;
		}

		// Set the image source equal to the temp image with an alpha channel.
		imageSrc = temp->add_alpha(false,0,0,0);
		
		// Enable save button permanently.
		btnSaveImage->set_sensitive(true);
		btnScale->set_sensitive(true);

		spnWidth->set_value(imageSrc->get_width());
		spnHeight->set_value(imageSrc->get_height());

		// Draw image to image display.
		Draw::drawImage(*wndMain, imageSrc, imgDisplay, IMAGE_BUFFER);

	}

    static void on_save_image() {

		// If there is no image loaded then do not open save dialog box.
		if (!imageSrc) return;

		// Create error message string in case of error when saving.
		string err;
		
		if (!PromptDialog::saveImageDialog(*wndMain, filter, imageSrc, err)) {
			// Display error message when error occurs.
			PromptDialog::displayMessage(*wndMain, err);
		}
	}

	static void on_scale_image() {
		
		// If there is no image loaded then do not scale anything.
		if (!imageSrc) return;
		
		imageSrc = LiquidRescale::liquidRescaleImage(imageSrc, spnWidth->get_value_as_int(), spnHeight->get_value_as_int());
		Draw::drawImage(*wndMain, imageSrc, imgDisplay, IMAGE_BUFFER);
	
	}
};

#endif