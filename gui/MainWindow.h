#ifndef __MainWindow__
#define __MainWindow__

/**
@file
Subclass of MainWindowBase, which is generated by wxFormBuilder.
*/

#include <set>
#include <wx/fswatcher.h>
#include "MainWindowBase.h"
#include "DataStore.hpp"    // for FAVORITES_SET
#include "Patient.hpp"      // for PAT_DIC
#include "HealthCard.hpp"

//#define TEST_MIME_TYPE
#ifdef TEST_MIME_TYPE
#include <wx/mimetype.h>
#endif

//// end generated include

class DBAdapter;
class InteractionsAdapter;
class InteractionsHtmlView;
class FullTextDBAdapter;
class Medication;
class FullTextEntry;
class TableViewDelegate;
class PrescriptionsAdapter;
class PrescriptionItem;
class PatientSheet;
class OperatorIDSheet;
class FullTextSearch;
class DataStore;
#include "PrescriptionsCart.hpp"
class wxPrintout;

/** Implementing MainWindowBase */
class MainWindow : public MainWindowBase
{
	protected:
		// Handlers for MainWindowBase events.
		void OnUpdateUI( wxUpdateUIEvent& event );
		void OnSearchNow( wxCommandEvent& event );
		void OnButtonPressed( wxCommandEvent& event );
		void OnSimplebookPageChanged( wxBookCtrlEvent& event );
		void OnSimplebookPageChanging( wxBookCtrlEvent& event );
		void OnSearchFiNow( wxCommandEvent& event );
		void OnPerformFindAction( wxCommandEvent& event );
		void OnSearchPatient( wxCommandEvent& event );
		void OnTreeBeginLabelEdit( wxTreeEvent& event );
		void OnTreeEndLabelEdit( wxTreeEvent& event );
		void OnTreeItemMenu( wxTreeEvent& event );
		void OnNewPrescription( wxCommandEvent& event );
		void OnCheckForInteractions( wxCommandEvent& event );
		void OnSavePrescription( wxCommandEvent& event );
		void OnSendPrescription( wxCommandEvent& event );
		void OnDeletePrescription( wxCommandEvent& event );
		void OnSelectionDidChange( wxDataViewEvent& event );
		void OnToolbarAction( wxCommandEvent& event );
		void OnExportWordListSearchResults( wxCommandEvent& event );
		void OnPrintDocument( wxCommandEvent& event );
		void OnShowAboutPanel( wxCommandEvent& event );
		void OnUpdateAipsDatabase( wxCommandEvent& event );
		void OnLoadAipsDatabase( wxCommandEvent& event );
		void OnLoadPrescription( wxCommandEvent& event );
		void OnManagePatients( wxCommandEvent& event );
		void OnSetOperatorIdentity( wxCommandEvent& event );
		void OnSmartCardTick( wxTimerEvent& event );
	public:
		/** Constructor */
		MainWindow( wxWindow* parent );
	//// end generated class members

private:
    virtual ~MainWindow();
    void fadeInAndShow();
    void resetDataInTableView();
    void setSearchState(int searchState, int btnId);
    void hideTextFinder();
    void updateSearchResults();
    void updateTableView();
    void switchTabs(int item);
    void stopProgressIndicator();
    void addTitle_andPackInfo_andMedId(wxString title, wxString packinfo, long medId);
    void addTitle_andAuthor_andMedId(wxString title, wxString author, long medId);
    void addTitle_andAtcCode_andAtcClass_andMedId(wxString title, wxString atccode, wxString atcclass, long medId);
    void addTitle_andRegnrs_andAuthor_andMedId(wxString title, wxString regnrs, wxString author, long medId);
    void addTitle_andApplications_andMedId(wxString title, wxString applications, long medId);

    void addKeyword_andNumHits_andHash(wxString keyword, unsigned long numHits, wxString hash);

    bool openSQLiteDatabase();
    bool openFullTextDatabase();
    bool openInteractionsCsvFile();
    void searchAnyDatabasesWith(wxString searchQuery);
    std::vector<Medication *> retrieveAllFavorites();
    std::vector<FullTextEntry *> retrieveAllFTFavorites();
    void updateExpertInfoView(wxString anchor);
    void pushToMedBasket(Medication *med);
    void updateInteractionsView();
    void updatePrescriptionsView();
    void updatePrescriptionHistory();
    void updateFullTextSearchView(wxString contentStr);
    void finishedDownloading();
    //void updateButtons(); // __deprecated The framework will call OnUpdateUI() instead
    void saveFavorites();
    void loadFavorites(DataStore *favorites);
    void tappedOnStar(int row);
    void loadPrescription_andRefreshHistory(wxString filename, bool refresh);
public:
    Medication * getShortMediWithId(long mid);
    void storeAllPrescriptionComments();
    void addItem_toPrescriptionCartWithId(PrescriptionItem *item, int n);
    void prescriptionDoctorChanged();
    void prescriptionPatientChanged();
    void prescriptionPatientDeleted();
    void resetPrescriptionHistory();
    void newHealthCardData(PAT_DICT &dict);
#ifndef __APPLE_
    void OnQuit( wxCommandEvent& event);
#endif
    void OnDraw_Prescription(wxPrintout *printout, wxDC *dc, float mmToLogical, int page);
    void OnDraw_Label(wxDC *dc, float mmToLogical);
    void initPrint(const wxString &printerName);
    void terminatePrint();

    // .mm 212
    #define NUM_ACTIVE_PRESCRIPTIONS   3
    static PrescriptionsCart mPrescriptionsCart[NUM_ACTIVE_PRESCRIPTIONS];

private:
    void setOperatorID();
    void printMedicineLabel();
    void savePrescription();

    // 113
    void printTechInfo();
    void printPrescription();
    wxArrayString csvGetInputListFromFile();
    void csvProcessKeywords(wxArrayString keywords);
    void csvOutputResult();
    void searchKeyword_inMedication_chapters_regnr(wxString aKeyword,
                                                   Medication *med,
                                                   std::set<wxString> chSet,
                                                   wxString rn);
    
    // not in amiko-osx
    int m_findCount;
    wxColour fiSearchFieldBGColor; //GetBackgroundColour()
    void mySectionTitles_reloadData(); // [mySectionTitles reloadData]
    void myPrescriptionsTableView_reloadData(int cartNo);
    int myPrescriptionsTableView_rowForView();
    void removeItemFromPrescription();
    int printPrescriptionNumPages();
    void getprintItemRangeForPage(const int page, int *firstItem, int *lastItem);

    void deletePrescription_returnCode_contextInfo(wxWindowModalDialogEvent& event);

#ifdef TEST_MIME_TYPE
    wxMimeTypesManager *m_mimeDatabase;
    wxFileTypeInfo *ftInfo;
#endif
    
    // 60
    HealthCard *healthCard;

    // .m 105
    int mUsedDatabase;
    // .m 110
    bool mSearchInteractions;
    bool mPrescriptionMode;

    // .m 138
    wxString csv;
    Medication *csvMedication;

    // .m 149
    Medication *mMed;
    DBAdapter *mDb;
    InteractionsAdapter *mInteractions;
    InteractionsHtmlView *mInteractionsView;
    FullTextDBAdapter *mFullTextDb;
    FullTextEntry *mFullTextEntry;
    FullTextSearch *mFullTextSearch;

public:
    PatientSheet *mPatientSheet;
private:
    OperatorIDSheet *mOperatorIDSheet;
    PrescriptionsAdapter *mPrescriptionAdapter;
    [[deprecated]] std::vector<DataObject *> doArray ; // __deprecated
public:
    wxArrayString favoriteKeyData;
 
    // .m 165
    FAVORITES_SET favoriteMedsSet;     // set of RegNrs
    FAVORITES_SET favoriteFTEntrySet;  // set of hashIDs

private:
    // .m 168
    std::vector<Medication *> searchResults;
    std::vector<FullTextEntry *> searchResultsFT;

    // .m 170
    wxArrayString mListOfSectionIds;  // full paths
    wxArrayString mListOfSectionTitles;
    
    // .m 179
    wxString mAnchor;
    wxString mFullTextContentStr;
    
    // .m 183
    bool mSearchInProgress;
    float m_alpha;
    float m_delta;
    bool possibleToOverwrite;
    bool modifiedPrescription;  // if true, presenting save/overwrite option makes sense

    wxFileSystemWatcher *fsWatcher;
    
    void OnLboxSelect(wxCommandEvent& event);
    void OnLboxDClick(wxCommandEvent& event);
    
    // Delegate
    void cellProcessing(int row);
    void OnHtmlLinkClicked(wxHtmlLinkEvent& event);
    void OnHtmlCellHover(wxHtmlCellEvent &event);
    void OnHtmlCellClicked(wxHtmlCellEvent &event);
    void OnNavigationRequest(wxWebViewEvent& evt);
    void OnTitleChanged(wxWebViewEvent& evt);
    void OnDocumentLoaded(wxWebViewEvent& evt);
    void OnDropFiles(wxDropFilesEvent& event);
    void OnFileWatcherUpdated(wxFileSystemWatcherEvent& event);

    // MLItemCellView.m:179
    void selectBasket(int cartNumber);

private:
    
    DECLARE_EVENT_TABLE()
};

#endif // __MainWindow__
