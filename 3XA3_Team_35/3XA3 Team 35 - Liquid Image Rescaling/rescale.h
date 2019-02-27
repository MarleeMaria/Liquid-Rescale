
#ifndef __RESCALE__
#define __RESCALE__

#include <gtkmm-3.0/gtkmm.h>
#include <glib.h>
#include <iostream>
#include <lqr.h>

using namespace Gtk;
using namespace std;

namespace LiquidRescale {

    const gint CHANNELS = 4;
    const gint MAX_STEP = 1; 
    const gfloat RIGIDITY = 0;

    guchar * pixelFromPixbufData(guint8* pixData, int pixWidth, int x, int y) {

        int offset = y*pixWidth + x*CHANNELS;
        return &pixData[offset];

    }

    Glib::RefPtr<Gdk::Pixbuf> pixbufFromCarver(LqrCarver* carver) {

        gint x, y;
        guchar *rgb;
        gint w, h;

        // Assert that the image is RGB.
        g_assert(lqr_carver_get_channels(carver) == CHANNELS);

        // Create a pixbuf to save to.
        w = lqr_carver_get_width(carver);
        h = lqr_carver_get_height(carver);

        Glib::RefPtr<Gdk::Pixbuf> newPixbuf = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB,
                                                                    true, 8, w, h);

        guint8* pixbufPixelData = newPixbuf->get_pixels();
        int pixelWidth = newPixbuf->get_rowstride();

        // Initialize carver reading.
        lqr_carver_scan_reset(carver);

        // Save RGB data to pixbuf.
        while (lqr_carver_scan(carver, &x, &y, &rgb)) {

            // Set the pixel to the new bitmap.
            guchar* pixel = pixelFromPixbufData(pixbufPixelData,
                                                pixelWidth,
                                                x,
                                                y);

            pixel[0] = rgb[0];
            pixel[1] = rgb[1];
            pixel[2] = rgb[2];
            pixel[3] = rgb[3];
        }

        return newPixbuf;

    }

    guchar * bufferFromPixbuf(Glib::RefPtr<Gdk::Pixbuf> src) {

        gint x, y, c;
        gint w, h;
        guchar *buffer;

        w = src->get_width();
        h = src->get_height();

        // Try to reserve memory for the buffer.
        buffer = g_try_new(guchar, CHANNELS * w * h);
        g_assert(buffer != NULL);

        guint8* srcPixelData = src->get_pixels();
        int srcPixelWidth = src->get_rowstride();

        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) {
                guchar * pixel = pixelFromPixbufData(srcPixelData, srcPixelWidth, x, y);
                for (c = 0; c < CHANNELS; c++) {
                    buffer[(y * w + x) * CHANNELS + c] = (guchar)pixel[c];
                }
            }
        }
        return buffer;
    }

    Glib::RefPtr<Gdk::Pixbuf> liquidRescaleImage(Glib::RefPtr<Gdk::Pixbuf> src, int newWidth, int newHeight) {

        if (src->get_width() < 2 || src->get_height() < 2) throw invalid_argument("Source image dimensions invalid. Width and height must be greater than one.");
        if (newWidth < 2) throw invalid_argument("Desired width invalid. Width must be greater than one.");
        if (newHeight < 2) throw invalid_argument("Desired height invalid. Height must be greater than one.");

        guchar *rgb_buffer = bufferFromPixbuf(src);
        LqrCarver *carver;
        carver = lqr_carver_new(rgb_buffer, src->get_width(), src->get_height(), CHANNELS);

        lqr_carver_init(carver, MAX_STEP, RIGIDITY);
        lqr_carver_resize(carver, newWidth, newHeight);

        Glib::RefPtr<Gdk::Pixbuf> newPixbuf = pixbufFromCarver(carver);

        lqr_carver_destroy(carver);

        return newPixbuf;

    }

}

#endif
