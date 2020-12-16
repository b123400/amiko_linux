//  PrescriptionPrintout.cpp
//  AmiKo-wx
//
//  Created by Alex Bettarini on 10 Dec 2020
//  Copyright © 2020 Ywesee GmbH. All rights reserved.

#include <wx/font.h>

#include "PrescriptionPrintout.hpp"
#include "MainWindow.h"


// Global page setup data
extern wxPageSetupDialogData* g_pageSetupData;

bool PrescriptionPrintout::OnPrintPage(int page)
{
#ifndef NDEBUG
    std::clog << __PRETTY_FUNCTION__ << std::endl;
#endif

    wxDC *dc = GetDC();
    if (!dc || !dc->IsOk())
        return false;

#if 0 //ndef NDEBUG
    if (page == 1)
        DrawPageOne();
    else if (page == 2)
#endif
        DrawPageTwo();

#ifndef NDEBUG
    // Draw page numbers at top left corner of printable area, sized so that
    // screen size of text matches paper size.
    MapScreenSizeToPage();

    dc->DrawText(wxString::Format("PAGE %d of %d", page, numPages), 0, 0);
#endif

    return true;
}

bool PrescriptionPrintout::OnBeginDocument(int startPage, int endPage)
{
    if (!wxPrintout::OnBeginDocument(startPage, endPage))
        return false;

    return true;
}

void PrescriptionPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
    *minPage = 1;
    *maxPage = numPages;
    *selPageFrom = 1;
    *selPageTo = numPages;
}

bool PrescriptionPrintout::HasPage(int pageNum)
{
#ifndef NDEBUG
    //std::clog << __FUNCTION__ << " page:" << pageNum << std::endl;
#endif
    return (pageNum <= numPages);
    //return (pageNum == 1 || pageNum == 2);
    //return (pageNum == 1);
}

void PrescriptionPrintout::DrawPageOne()
{
#ifndef NDEBUG
    std::clog << __PRETTY_FUNCTION__ << std::endl;
#endif

    // You might use THIS code if you were scaling graphics of known size to fit
    // on the page. The commented-out code illustrates different ways of scaling
    // the graphics.

    // We know the graphic is 230x350. If we didn't know this, we'd need to
    // calculate it.
    wxCoord maxX = 230;
    wxCoord maxY = 350;

    // This sets the user scale and origin of the DC so that the image fits
    // within the paper rectangle (but the edges could be cut off by printers
    // that can't print to the edges of the paper -- which is most of them. Use
    // this if your image already has its own margins.
//    FitThisSizeToPaper(wxSize(maxX, maxY));
//    wxRect fitRect = GetLogicalPaperRect();

    // This sets the user scale and origin of the DC so that the image fits
    // within the page rectangle, which is the printable area on Mac and MSW
    // and is the entire page on other platforms.
//    FitThisSizeToPage(wxSize(maxX, maxY));
//    wxRect fitRect = GetLogicalPageRect();

    // This sets the user scale and origin of the DC so that the image fits
    // within the page margins as specified by g_PageSetupData, which you can
    // change (on some platforms, at least) in the Page Setup dialog. Note that
    // on Mac, the native Page Setup dialog doesn't let you change the margins
    // of a wxPageSetupDialogData object, so you'll have to write your own dialog or
    // use the Mac-only wxMacPageMarginsDialog, as we do in this program.
    FitThisSizeToPageMargins(wxSize(maxX, maxY), *g_pageSetupData);
    wxRect fitRect = GetLogicalPageMarginsRect(*g_pageSetupData);

    // This sets the user scale and origin of the DC so that the image appears
    // on the paper at the same size that it appears on screen (i.e., 10-point
    // type on screen is 10-point on the printed page) and is positioned in the
    // top left corner of the page rectangle (just as the screen image appears
    // in the top left corner of the window).
//    MapScreenSizeToPage();
//    wxRect fitRect = GetLogicalPageRect();

    // You could also map the screen image to the entire paper at the same size
    // as it appears on screen.
//    MapScreenSizeToPaper();
//    wxRect fitRect = GetLogicalPaperRect();

    // You might also wish to do you own scaling in order to draw objects at
    // full native device resolution. In this case, you should do the following.
    // Note that you can use the GetLogicalXXXRect() commands to obtain the
    // appropriate rect to scale to.
//    MapScreenSizeToDevice();
//    wxRect fitRect = GetLogicalPageRect();

    // Each of the preceding Fit or Map routines positions the origin so that
    // the drawn image is positioned at the top left corner of the reference
    // rectangle. You can easily center or right- or bottom-justify the image as
    // follows.

    // This offsets the image so that it is centered within the reference
    // rectangle defined above.
    wxCoord xoff = (fitRect.width - maxX) / 2;
    wxCoord yoff = (fitRect.height - maxY) / 2;
    OffsetLogicalOrigin(xoff, yoff);

    // This offsets the image so that it is positioned at the bottom right of
    // the reference rectangle defined above.
//    wxCoord xoff = (fitRect.width - maxX);
//    wxCoord yoff = (fitRect.height - maxY);
//    OffsetLogicalOrigin(xoff, yoff);

    MainWindow* vc = (MainWindow *)wxTheApp->GetTopWindow();
    vc->Draw(*GetDC());
}

void PrescriptionPrintout::DrawPageTwo()
{
#ifndef NDEBUG
    std::clog << __PRETTY_FUNCTION__ << std::endl;
#endif

    // You might use THIS code to set the printer DC to ROUGHLY reflect
    // the screen text size. This page also draws lines of actual length
    // 5cm on the page.

    // Compare this to DrawPageOne(), which uses the really convenient routines
    // from wxPrintout to fit the screen image onto the printed page. This page
    // illustrates how to do all the scaling calculations yourself, if you're so
    // inclined.

    wxDC *dc = GetDC();

    // Get the logical pixels per inch of screen and printer
    int ppiScreenX, ppiScreenY;
    GetPPIScreen(&ppiScreenX, &ppiScreenY);
    int ppiPrinterX, ppiPrinterY;
    GetPPIPrinter(&ppiPrinterX, &ppiPrinterY);

    // This scales the DC so that the printout roughly represents the screen
    // scaling. The text point size _should_ be the right size but in fact is
    // too small for some reason. This is a detail that will need to be
    // addressed at some point but can be fudged for the moment.
    float scale = (float)((float)ppiPrinterX/(float)ppiScreenX);

    // Now we have to check in case our real page size is reduced (e.g. because
    // we're drawing to a print preview memory DC)
    int pageWidth, pageHeight;
    int w, h;
    dc->GetSize(&w, &h);
    GetPageSizePixels(&pageWidth, &pageHeight);

    // If printer pageWidth == current DC width, then this doesn't change. But w
    // might be the preview bitmap width, so scale down.
    float overallScale = scale * (float)(w/(float)pageWidth);
    dc->SetUserScale(overallScale, overallScale);

    // Calculate conversion factor for converting millimetres into logical
    // units. There are approx. 25.4 mm to the inch. There are ppi device units
    // to the inch. Therefore 1 mm corresponds to ppi/25.4 device units. We also
    // divide by the screen-to-printer scaling factor, because we need to
    // unscale to pass logical units to DrawLine.

    float logUnitsFactor = (float)(ppiPrinterX/(scale*25.4));

#if 1
    MainWindow* vc = (MainWindow *)wxTheApp->GetTopWindow();
    vc->DrawPrescription(this, dc, logUnitsFactor);
#else
    static bool alternate = false;
    alternate = !alternate;
    if (alternate)
    {
        MainWindow* vc = (MainWindow *)wxTheApp->GetTopWindow();
        vc->Draw2(this, dc, logUnitsFactor);
        return;
    }

    // Alternate drawing foir debugging
    // Draw 50 mm by 50 mm L shape
    float logUnits = (float)(50*logUnitsFactor);
    dc->SetPen(* wxBLACK_PEN);
    dc->DrawLine(50, 250, (long)(50.0 + logUnits), 250);
    dc->DrawLine(50, 250, 50, (long)(250.0 + logUnits));

    dc->SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);
    dc->SetBrush(*wxTRANSPARENT_BRUSH);

    { // GetTextExtent demo:
        wxString words[7] = { "This ", "is ", "GetTextExtent ",
                             "testing ", "string. ", "Enjoy ", "it!" };
        long x = 200, y= 250;

        dc->SetFont(wxFontInfo(15).Family(wxFONTFAMILY_SWISS));

        for (int i = 0; i < 7; i++)
        {
            wxCoord wordWidth, wordHeight;
            wxString word = words[i];
            word.Remove( word.Len()-1, 1 );
            dc->GetTextExtent(word, &wordWidth, &wordHeight);
            dc->DrawRectangle(x, y, wordWidth, wordHeight);
            dc->GetTextExtent(words[i], &wordWidth, &wordHeight);
            dc->DrawText(words[i], x, y);
            x += wordWidth;
        }
    }

    wxFont m_testFont = wxFontInfo(10).Family(wxFONTFAMILY_SWISS);
    dc->SetFont(m_testFont);

    dc->DrawText("Some test text", 200, 300 );

    // TESTING

    int leftMargin = 20;
    int rightMargin = 20;
    int topMargin = 20;
    int bottomMargin = 20;

    int pageWidthMM, pageHeightMM;
    GetPageSizeMM(&pageWidthMM, &pageHeightMM);

    float leftMarginLogical = (float)(logUnitsFactor*leftMargin);
    float topMarginLogical = (float)(logUnitsFactor*topMargin);
    float bottomMarginLogical = (float)(logUnitsFactor*(pageHeightMM - bottomMargin));
    float rightMarginLogical = (float)(logUnitsFactor*(pageWidthMM - rightMargin));

    dc->SetPen(* wxRED_PEN);
    dc->DrawLine( (long)leftMarginLogical, (long)topMarginLogical,
        (long)rightMarginLogical, (long)topMarginLogical);
    dc->DrawLine( (long)leftMarginLogical, (long)bottomMarginLogical,
        (long)rightMarginLogical,  (long)bottomMarginLogical);

    WritePageHeader(this, dc, "A header", logUnitsFactor);
#endif
}

// Writes a header on a page. Margin units are in millimetres.
bool PrescriptionPrintout::WritePageHeader(wxPrintout *printout, wxDC *dc, const wxString&text, float mmToLogical)
{
#ifndef NDEBUG
    std::clog << __PRETTY_FUNCTION__ << " text:" << text << std::endl;
#endif

    int pageWidthMM, pageHeightMM;

    printout->GetPageSizeMM(&pageWidthMM, &pageHeightMM);
    wxUnusedVar(pageHeightMM);

    int leftMargin = 10;
    int topMargin = 10;
    int rightMargin = 10;

    float leftMarginLogical = (float)(mmToLogical*leftMargin);
    float topMarginLogical = (float)(mmToLogical*topMargin);
    float rightMarginLogical = (float)(mmToLogical*(pageWidthMM - rightMargin));

    wxCoord xExtent, yExtent;
    dc->GetTextExtent(text, &xExtent, &yExtent);

    float xPos = (float)(((((pageWidthMM - leftMargin - rightMargin)/2.0)+leftMargin)*mmToLogical) - (xExtent/2.0));
    dc->DrawText(text, (long)xPos, (long)topMarginLogical);

    dc->SetPen(* wxBLACK_PEN);
    dc->DrawLine( (long)leftMarginLogical, (long)(topMarginLogical+yExtent),
                  (long)rightMarginLogical, (long)topMarginLogical+yExtent );

    return true;
}