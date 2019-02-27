
#ifndef __DRAW__
#define __DRAW__

#include <gtkmm-3.0/gtkmm.h>
#include <glib.h>
#include <iostream>
#include "dialog.h"

using namespace Gtk;
using namespace std;
using namespace Glib;

namespace Draw {

    struct Dimensions {
        int width;
		int height;
		bool operator==(const Dimensions& rhs) {
			return this->width==rhs.width && this->height==rhs.height;
		}
    };

    static Dimensions bestFitToDimensions(Dimensions src, Dimensions display, int buffer) {
		
		// Assert valid parameters.
		if (src.width < 1 || src.height < 1) throw invalid_argument("Invalid src dimensions.");
		if (display.width < 1 || display.height < 1) throw invalid_argument("Invalid display dimensions.");
		if (buffer < 0) throw invalid_argument("Invalid buffer value.");
		if (display.width - buffer * 2 < 1) throw invalid_argument("Invalid buffer size for given display width.");
		if (display.height - buffer * 2 < 1) throw invalid_argument("Invalid buffer size for given display height.");

		// Determine ratios.
		double imageRatio = (double)src.width / (double)src.height;
		double mainRatio = (double)(display.width - buffer * 2) / (double)(display.height - buffer * 2);
        
        // Create dimensions structure to hold output.
        Dimensions dims;

		// Fit the image to the screen.
		if (imageRatio > mainRatio) {
			dims.width = display.width - buffer * 2;
			dims.height = (int) ((double)dims.width / imageRatio);
		}
		else {
			dims.height = display.height - buffer * 2;
			dims.width = (int) ((double)dims.height * imageRatio);
        }
        
        return dims;
	}

    static void drawImage(Window& parent, RefPtr<Gdk::Pixbuf> src, Image *display, int buffer) {

        // Get dimensions from src and display.
        Draw::Dimensions 	srcDims = {src->get_width(), src->get_height()},
		 					displayDims = {display->get_width(), display->get_height()};

        // Determine best way to fit image on the display.
        Dimensions size = bestFitToDimensions(srcDims, displayDims, buffer);

		// If the resized image does not scale well into the display dimensions, ask 
		// the user if they want to display the actual image (resizing the window).
		if (size.width < 1 || size.height < 1) {
			
			// If user decides not to display image, display a blank image that's the size of the display window.
			if (!PromptDialog::displayMessage(parent, "Image proportions will cause window to expand. Display image?")) {
				size = displayDims;
				src = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB,
											true, 8,
											size.width,
											size.height); 
			}
			// If user decides to display image, set the size equal to the size of the actual image source.
			else {
				size = srcDims;
			}																	
		}

		// Create blank Pixbuf to scale source image on to.
		RefPtr<Gdk::Pixbuf> imageOut = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB,
															true, 8,
															size.width,
															size.height); 

		// Write the source image into the output image.
		src->scale(	imageOut,
                    0, 0,
                    size.width,
                    size.height,
                    0, 0,
                    ((double)size.width)/srcDims.width,
                    ((double)size.height)/srcDims.height,
                    Gdk::INTERP_BILINEAR);
		
		// Display image.
		display->set(imageOut);
	}
}

#endif