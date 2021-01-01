//  InteractionsAdapter.cpp
//  AmiKo-wx
//
//  Created by Alex Bettarini on 17 Jun 2020
//  Copyright © 2020 Ywesee GmbH. All rights reserved.

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

#if 0
#include <wx/textfile.h>
#else
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#endif

//#include <wx/arrstr.h>
#include <wx/tokenzr.h>

#include "InteractionsAdapter.hpp"

InteractionsAdapter::InteractionsAdapter()
{
    //std::cerr << __PRETTY_FUNCTION__ << " constructor, this: " << this << std::endl;
}

// 36
bool InteractionsAdapter::openInteractionsCsvFile(wxString name)
{
#ifndef NDEBUG
    //std::clog << __PRETTY_FUNCTION__ << " " << name << std::endl;
#endif
     
    // ** A. Check first users documents folder
    wxString documentsDir = wxStandardPaths::Get().GetUserDataDir();
    wxString filePath( documentsDir + wxFILE_SEP_PATH + name + ".csv");
    if (wxFileName::Exists(filePath)) {
        std::clog << "Drug interactions csv found in UserData dir: " << filePath << std::endl;

        return readDrugInteractionMap(filePath);
    }
    
    // ** B. If no database is available, check if db is in app bundle
    documentsDir = wxStandardPaths::Get().GetResourcesDir();
    std::clog << __FUNCTION__ << __LINE__ << " TODO: check for DB in resources directory\n";
    
#ifndef NDEBUG
    std::cerr << "Didn't find " << filePath << std::endl;
#endif
    
    return false;
}

// 62
void InteractionsAdapter::closeInteractionsCsvFile()
{
    if (myDrugInteractionMap.size() > 0)
        myDrugInteractionMap.clear(); // removeAllObjects()
}

// 69
int InteractionsAdapter::getNumInteractions()
{
    return myDrugInteractionMap.size();
}

// 77
wxString InteractionsAdapter::getInteractionHtmlBetween_and(wxString atc1, wxString atc2)
{
    if (myDrugInteractionMap.size() > 0) {
        wxString key = wxString::Format("%s-%s", atc1, atc2);
        return myDrugInteractionMap[key];
    }

    return wxEmptyString;
}

// 86
// Read drug interactions csv line after line
bool InteractionsAdapter::readDrugInteractionMap(wxString filePath)
{
#ifndef NDEBUG
    //std::clog << __PRETTY_FUNCTION__ << " " << filePath << std::endl;
#endif

#if 0
    wxTextFile tfile;
    tfile.Open(filePath);
    wxString str = tfile.GetFirstLine();
    while (!tfile.Eof())
    {
        str = tfile.GetNextLine();
    }
#else
    wxFileInputStream input(filePath);
    wxTextInputStream text(input, wxT("\x09"), wxConvUTF8 );
    unsigned long lineNumber = 0;
    while(input.IsOk() && !input.Eof() )
    {
        wxString line = text.ReadLine() + wxT("\n");
        lineNumber++;
        if (line.length() == 0)
            continue;

        //wxArrayString a = wxSplit(line, '|'); // wxSplit can only take a single char
        wxArrayString a;
        wxStringTokenizer tokenizer(line, "|"); // TODO: "||" doesn't split by substrings
        while ( tokenizer.HasMoreTokens() )
        {
            wxString tk = tokenizer.GetNextToken();
            if (tk.length() == 0)
                continue;

            //std::clog << "Token: " << tk << std::endl;
            a.Add(tk);
        }

        if (a.GetCount() < 3) {
            std::clog << __FUNCTION__ << ", CSV line " << lineNumber
                << " <" << line << ">\n"
                << "array  count: " << a.GetCount() << std::endl;
            continue;
        }

        wxString key = a.Item(0) + "-" + a.Item(1);

        //myDrugInteractionMap[std::string(a.Item(2).c_str())] = std::string(key.c_str());
        myDrugInteractionMap[key] = a.Item(2);
    }
#endif
    
    return true;
}
