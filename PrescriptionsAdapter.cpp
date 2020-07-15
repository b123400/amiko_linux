//  PrescriptionsAdapter.cpp
//  AmiKo-wx
//
//  Created by Alex Bettarini on 23 Jun 2020
//  Copyright © 2020 Ywesee GmbH. All rights reserved.

#include <wx/wx.h>
#include <wx/arrstr.h>

#include "PrescriptionsAdapter.hpp"

PrescriptionsAdapter::PrescriptionsAdapter()
{
    
}

// 39
// Returns an array of filenames (wxString),
// just the basename with the extension ".amk" stripped off
wxArrayString PrescriptionsAdapter::listOfPrescriptionsForPatient(Patient *p)
{
    std::clog << __PRETTY_FUNCTION__ << " TODO" << std::endl;
    wxArrayString a;
    return a;
}

// 79
// Returns an array of filenames (wxString), the full path
wxArrayString PrescriptionsAdapter::listOfPrescriptionURLsForPatient(Patient *p)
{
    std::clog << __PRETTY_FUNCTION__ << " TODO" << std::endl;

    wxArrayString a;
    return a;
}
