
#ifdef __DO_TESTING__

#ifndef __TEST__
#define __TEST__

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

#include <gtkmm-3.0/gtkmm.h>
#include <glib.h>
#include <iostream>

#include "dialog.h"
#include "draw.h"
#include "rescale.h"

using namespace CppUnit;
using namespace PromptDialog;
using namespace Draw;
using namespace LiquidRescale;

class TestLiquidRescaleApp : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestLiquidRescaleApp);
    CPPUNIT_TEST(testGetFileExtension);
    CPPUNIT_TEST(testBestFitToDimensions);
    CPPUNIT_TEST(testDimensions);
    CPPUNIT_TEST(testLiquidRescaleImage);
    CPPUNIT_TEST(testBufferFromPixbuf);
    CPPUNIT_TEST(testPixbufFromCarver);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void){};
    void tearDown(void){};

protected:

    // dialog.h
    void testGetFileExtension(void){

       CPPUNIT_ASSERT(GetFileExtension("afe.bcd").compare("bcd") == 0);
       CPPUNIT_ASSERT(GetFileExtension("a.b").compare("b") == 0);
       CPPUNIT_ASSERT(GetFileExtension(".b").compare("b") == 0);
       CPPUNIT_ASSERT(GetFileExtension("a.").compare("") == 0);
       CPPUNIT_ASSERT(GetFileExtension("a").compare("") == 0);
       CPPUNIT_ASSERT(GetFileExtension("a.b.c").compare("c") == 0);
       CPPUNIT_ASSERT(GetFileExtension("a.b..").compare("") == 0);
       CPPUNIT_ASSERT(GetFileExtension("").compare("") == 0);
       CPPUNIT_ASSERT(GetFileExtension(".").compare("") == 0);

    };

    // draw.h 
    void testDimensions(void) {
        Dimensions x, y;

        x = {100,100}; y = {100,200};
        CPPUNIT_ASSERT(!(x == y));
        
        x = {100,100}; y = {200,100};
        CPPUNIT_ASSERT(!(x == y));
        
        x = {100,200}; y = {100,100};
        CPPUNIT_ASSERT(!(x == y));

        x = {200,100}; y = {100,100};
        CPPUNIT_ASSERT(!(x == y));

        x = {100,100}; y = {100,100};
        CPPUNIT_ASSERT(x == y);
    }

    void testBestFitToDimensions(void) {

        Dimensions src, display, res;
        int buff;

        src = {100,100}; display = {100,100}; buff = 0; res = {100,100};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        src = {100,100}; display = {100,100}; buff = 10; res = {80,80};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        src = {50,100}; display = {100,100}; buff = 10; res = {40,80};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        src = {1,2}; display = {100,100}; buff = 10; res = {40,80};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        src = {100,100}; display = {100,40}; buff = 10; res = {20,20};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        src = {100,100}; display = {40,100}; buff = 10; res = {20,20};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        src = {200,400}; display = {100,40}; buff = 10; res = {10,20};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        src = {200,400}; display = {40,100}; buff = 10; res = {20,40};
        CPPUNIT_ASSERT(bestFitToDimensions(src,display,buff) == res);

        // EXCPETION TESTING

        src = {100,100}; display = {100,100}; buff = -10;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);
        
        src = {0,100}; display = {100,100}; buff = 10;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);

        src = {100,0}; display = {100,100}; buff = 10;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);
        
        src = {100,100}; display = {0,100}; buff = 10;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);

        src = {100,100}; display = {100,0}; buff = 10;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);

        src = {20,20}; display = {20,40}; buff = 10;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);

        src = {20,20}; display = {40,20}; buff = 10;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);

        src = {20,20}; display = {20,40}; buff = 20;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);

        src = {20,20}; display = {40,20}; buff = 20;
        CPPUNIT_ASSERT_THROW(bestFitToDimensions(src,display,buff), invalid_argument);

        src = {1,1}; display = {1,1}; buff = 0;
        CPPUNIT_ASSERT_NO_THROW(bestFitToDimensions(src,display,buff));

    }

    // rescale.h
    void testLiquidRescaleImage(void) {

        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB,
                                                                true, 8, 100, 100); 

        Glib::RefPtr<Gdk::Pixbuf> res;

        res = liquidRescaleImage(pixbuf, 100, 100);
        CPPUNIT_ASSERT(res->get_width() == 100 && res->get_height() == 100);

        res = liquidRescaleImage(pixbuf, 200, 300);
        CPPUNIT_ASSERT(res->get_width() == 200 && res->get_height() == 300);
        
        res = liquidRescaleImage(pixbuf, 200, 50);
        CPPUNIT_ASSERT(res->get_width() == 200 && res->get_height() == 50);

        res = liquidRescaleImage(pixbuf, 50, 300);
        CPPUNIT_ASSERT(res->get_width() == 50 && res->get_height() == 300);

        res = liquidRescaleImage(pixbuf, 50, 25);
        CPPUNIT_ASSERT(res->get_width() == 50 && res->get_height() == 25);

        res = liquidRescaleImage(pixbuf, 2, 2);
        CPPUNIT_ASSERT(res->get_width() == 2 && res->get_height() == 2);

        CPPUNIT_ASSERT_THROW(liquidRescaleImage(pixbuf, 1, 10), invalid_argument);
        CPPUNIT_ASSERT_THROW(liquidRescaleImage(pixbuf, 10, 1), invalid_argument);
        CPPUNIT_ASSERT_THROW(liquidRescaleImage(pixbuf, 1, 1), invalid_argument);

        pixbuf = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB, true, 8, 1, 1);
        CPPUNIT_ASSERT_THROW(liquidRescaleImage(pixbuf, 10, 10), invalid_argument);

        pixbuf = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB, true, 8, 1, 10);
        CPPUNIT_ASSERT_THROW(liquidRescaleImage(pixbuf, 10, 10), invalid_argument);
        
        pixbuf = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB, true, 8, 10, 1);
        CPPUNIT_ASSERT_THROW(liquidRescaleImage(pixbuf, 10, 10), invalid_argument);

    }

    void testBufferFromPixbuf(void) {

        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB,
                                                                true, 8, 100, 200); 
        guchar *buffer;

        buffer = bufferFromPixbuf(pixbuf);

        guint8* srcPixelData = pixbuf->get_pixels();
        int srcPixelWidth = pixbuf->get_rowstride();

        for (gint y = 0; y < pixbuf->get_height(); y++) {
            for (gint x = 0; x < pixbuf->get_width(); x++) {
                guchar * pixel = pixelFromPixbufData(srcPixelData, srcPixelWidth, x, y);
                for (gint c = 0; c < CHANNELS; c++) {
                    CPPUNIT_ASSERT(buffer[(y * pixbuf->get_width() + x) * CHANNELS + c] == (guchar)pixel[c]);
                }
            }
        }
    }

    void testPixbufFromCarver(void) {

        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create(	Gdk::COLORSPACE_RGB,
                                                                true, 8, 100, 200); 

        guchar *buffer = bufferFromPixbuf(pixbuf);

        LqrCarver *carver; 
        carver = lqr_carver_new(buffer, pixbuf->get_width(), pixbuf->get_height(), CHANNELS);
        lqr_carver_init(carver, MAX_STEP, RIGIDITY);

        guchar *outbuffer = bufferFromPixbuf(pixbufFromCarver(carver));

        for (gint i = 0; i < CHANNELS * pixbuf->get_width() * pixbuf->get_height(); i++) {
            CPPUNIT_ASSERT(buffer[i] == outbuffer[i]);
        }
    }


private:
    bool compareBuffers();

};

namespace Testing {

    CPPUNIT_NS::TestResult testresult;	
	CPPUNIT_NS::TestResultCollector collectedresults;
	CPPUNIT_NS::TestRunner testrunner;

    static bool initialized = false;

    static int init() {

        if (initialized) return 2;

        CPPUNIT_TEST_SUITE_REGISTRATION( TestLiquidRescaleApp );

		testresult.addListener (&collectedresults);
        testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());

        initialized = true;
        return 0;
    }

    static int run() {

        if (!initialized) return 2;

        testrunner.run(testresult);

        std::ofstream xmlFileOut("LiquidRescaleTestResults.xml");
        XmlOutputter xmlOut(&collectedresults, xmlFileOut);
        xmlOut.write();

        return (collectedresults.testFailures() == 0) ? 0 : 1;

    }
}

#endif
#endif