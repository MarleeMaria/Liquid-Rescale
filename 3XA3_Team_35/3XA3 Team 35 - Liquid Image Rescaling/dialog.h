
#ifndef __DIALOG__
#define __DIALOG__

#include <gtkmm-3.0/gtkmm.h>

using namespace Gtk;
using namespace std;
using namespace Glib;

namespace PromptDialog {

    static string GetFileExtension(const string& FileName) {
        if(FileName.find_last_of(".") != string::npos)
            return FileName.substr(FileName.find_last_of(".")+1);

        return "";
    }

	static bool displayMessage(Window& parent, const string message) {

		MessageDialog dialog(parent, message);
		dialog.add_button (Stock::CANCEL, RESPONSE_CANCEL);

		const int response = dialog.run();
		dialog.hide();		

		if (response == RESPONSE_OK) return true;
		return false; 

	}

    static bool openImageDialog(Window& parent, RefPtr<FileFilter> filter, string& openFileName) {

        // Create file loading dialog.
		FileChooserDialog dialog(parent, "Open an Image", FILE_CHOOSER_ACTION_OPEN);
		
		// Edit dialog interface.
		dialog.add_button (Stock::OPEN, RESPONSE_ACCEPT);
		dialog.add_button (Stock::CANCEL, RESPONSE_CANCEL);
		dialog.add_filter (filter);

		// Run dialog and wait for a response.
		const int response = dialog.run();
		dialog.hide();

		// If unsuccessful, return false. Else, continue.
        if (response != RESPONSE_ACCEPT) return false;
        
        // Set file name to given pointer.
        openFileName = dialog.get_filename();

        // Return successful dialog.
        return true;

    } 

    static bool saveImageDialog(Window& parent, RefPtr<FileFilter> filter, RefPtr<Gdk::Pixbuf> src, string& errMessage) {

        FileChooserDialog dialog(parent, "Save an Image", FILE_CHOOSER_ACTION_SAVE);
		
		dialog.add_button (Stock::SAVE, RESPONSE_ACCEPT);
		dialog.add_button (Stock::CANCEL, RESPONSE_CANCEL);
		dialog.add_filter (filter);

		dialog.set_select_multiple(false);
		dialog.set_do_overwrite_confirmation(true);

		
		const int response = dialog.run();
		dialog.hide();

		if (response == RESPONSE_ACCEPT) {
			try {
				// Handle PNG/no extensions.
				if (GetFileExtension(dialog.get_filename()).compare("png") == 0) {
					src->save(dialog.get_filename(), "png");
				}
				// Handle JPG and JPEG images.
				else if (GetFileExtension(dialog.get_filename()).compare("jpg") == 0 || GetFileExtension(dialog.get_filename()).compare("jpeg") == 0) {
					src->save(dialog.get_filename(), "jpeg");
				}
				// Handle other/no extensions.
				else {
					errMessage = "Save unsuccessful. Please include a valid extension (png/jpg/jpeg) in file name.";
					return false;
				}
			}
			catch(exception e) { 
				// unsuccessful save.
				errMessage = "Save unsuccessful. Please refrain from using special characters in file name.";
				return false;
			}
			catch(Glib::Error & e) {
				// unsuccessful save.
				errMessage = "Save unsuccessful. Please refrain from using special characters in file name.";
				return false;
			}
		}
        else if (response != RESPONSE_CANCEL && response != RESPONSE_DELETE_EVENT) {
			// unsuccessful dialog.
			errMessage = "Something went wrong when saving. Please try again.";
			return false;		
		}

		// Return successful dialog.
        return true;

    }

}

#endif