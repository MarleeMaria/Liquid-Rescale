
#include "ui.h"

// Build me:
// g++ -rdynamic -o LiquidRescaleApp main.cpp -Wall `pkg-config --cflags --libs gtkmm-3.0 lqr-1 glib-2.0 cppunit` -export-dynamic

//#define __DO_TESTING__

#ifdef __DO_TESTING__
	#include "test.h"
#endif

int main(int argc, char *argv[])
{
	int r;

	#ifdef __DO_TESTING__
		r = Testing::init();
		if (r == 0) r = Testing::run();
	#endif
	#ifndef __DO_TESTING__
		r = UI::init();
		if (r == 0) r = UI::run();
	#endif

	return r;
}