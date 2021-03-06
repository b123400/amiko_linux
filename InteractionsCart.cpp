//  InteractionsCart.cpp
//  AmiKo-wx
//
//  Created by Alex Bettarini on 9 Jul 2020
//  Copyright © 2020 Ywesee GmbH. All rights reserved.

#include "InteractionsCart.hpp"
#include "InteractionsAdapter.hpp"
#include "Medication.hpp"

InteractionsCart::InteractionsCart()
{
    
}

// 38
int InteractionsCart::size()
{
    return cart.size();
}

// 43
wxString InteractionsCart::interactionsAsHtmlForAdapter_withTitles_andIds(
                            InteractionsAdapter *adapter,
                            wxArrayString &titles,  // out
                            wxArrayString &ids)     // out
{
    // 47
    // Check if there are meds in the "Medikamentenkorb"
    if (size() <= 1)
        return wxEmptyString;

    // 49
    // TODO: First sort them alphabetically
    wxArrayString sortedNames;
    std::map<wxString, Medication *>::iterator it;
    for (it = cart.begin(); it != cart.end(); ++it)
        sortedNames.Add( it->first );

    wxString htmlStr;

    // 51
    // Big loop
    for (wxString name1 : sortedNames) {
        for (wxString name2 : sortedNames) {
            if (name1 == name2)
                continue;

            // 55
            // Extract meds by names from interaction basket
            Medication *med1 = cart[name1];
            Medication *med2 = cart[name2];
            // Get ATC codes from interaction db
            wxArrayString m_code1 = wxSplit(med1->atccode, ';');
            wxArrayString m_code2 = wxSplit(med2->atccode, ';');
            wxArrayString atc1;
            wxArrayString atc2;
            if (m_code1.size() > 1)
                atc1 = wxSplit(m_code1[0], ',');

            if (m_code2.size() > 1)
                atc2 = wxSplit(m_code2[0], ',');
            
            wxString atc_code1;
            wxString atc_code2;
            if (atc1.size() <= 0)
                continue;
                
            for (auto atc_code1 : atc1) {
                if (atc2.size() <= 0)
                    continue;

                // 74
                for (auto atc_code2 : atc2) {
                    wxString html = adapter-> getInteractionHtmlBetween_and(atc_code1, atc_code2);
                    if (html.length() <= 0)
                        continue;
                    
                    // 76
                    // Replace all occurrences of ATC codes with med names
                    // except for the first one because it's used as an HTML tag ID
                    {
                        int startPos, endPos;
                        wxString leftString, rightString;

                        startPos = html.Find(atc_code1);
                        endPos = startPos + atc_code1.length();
                        leftString = html.Left(endPos);
                        rightString = html.Mid(endPos);
                        rightString.Replace(atc_code1, name1, true);
                        html = leftString + rightString;

                        startPos = html.Find(atc_code2);
                        endPos = startPos + atc_code2.length();
                        leftString = html.Left(endPos);
                        rightString = html.Mid(endPos);
                        rightString.Replace(atc_code2, name2, true);
                        html = leftString + rightString;
                    }

                    // 81
                    // Concatenate strings
                    htmlStr += html;

                    // Add to title and anchor lists
                    titles.Add( wxString::Format("%s \u2192 %s", name1, name2));
                    ids.Add( wxString::Format("%s-%s", atc_code1, atc_code2));
                } // for atc_code2
            } // for atc_code 1

        } // for name 2
    } // for name 1
    
    return htmlStr;
}
