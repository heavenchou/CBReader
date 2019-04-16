// ---------------------------------------------------------------------------

#ifndef mainH
#define mainH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Menus.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.WebBrowser.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.TreeView.hpp>

#include "setting.h"
#include "bookcase.h"
#include "series.h"
#include "navtree.h"
#include "cbxml.h"
#include <FMX.ActnList.hpp>
#include <FMX.StdActns.hpp>
#include <System.Actions.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Controls3D.hpp>
#include <FMX.Layers3D.hpp>
#include <FMX.Viewport3D.hpp>
#include <System.Math.Vectors.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.ScrollBox.hpp>
#include <System.Rtti.hpp>
#include <FMX.MultiView.hpp>
#include <System.RegularExpressions.hpp>
#include <FMX.DialogService.hpp>
#include "option.h"
#include "../../Monster/src/monster.h"
#include <FMX.Ani.hpp>
#include <FMX.Memo.hpp>
#include <FMX.ImgList.hpp>
#include <System.ImageList.hpp>
#include <FMX.Effects.hpp>
// ---------------------------------------------------------------------------

class TfmMain : public TForm
{
__published: // IDE-managed Components
	TMainMenu *MainMenu;
	TPanel *pnNav;
	TTabControl *TabControl1;
	TPanel *pnFoot;
	TTabItem *TabItem1;
	TWebBrowser *WebBrowser;
	TToolBar *ToolBar1;
	TCornerButton *btOpenBookcase;
	TMenuItem *MenuItem3;
	TSplitter *Splitter1;
	TTabControl *tcMainFunc;
	TTabItem *TabItem2;
	TTreeView *tvNavTree;
	TTabItem *TabItem3;
	TPanel *Panel3;
	TEdit *edFindSutra_SutraName;
	TEdit *edFindSutra_Byline;
	TButton *btFindSutra;
	TSplitter *Splitter2;
	TTabItem *TabItem4;
	TStringGrid *sgFindSutra;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TPanel *Panel4;
	TLabel *Label1;
	TLabel *lbGoSutra_Book;
	TLabel *lbGoSutra_SutraNum;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *lbGoBook_Book;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TComboBox *cbGoSutra_BookId;
	TEdit *edGoSutra_SutraNum;
	TEdit *edGoSutra_Juan;
	TEdit *edGoSutra_Page;
	TEdit *edGoSutra_Field;
	TEdit *edGoSutra_Line;
	TEdit *edGoBook_Vol;
	TEdit *edGoBook_Page;
	TEdit *edGoBook_Field;
	TEdit *edGoBook_Line;
	TEdit *edGoByKeyword;
	TComboBox *cbGoBook_BookId;
	TButton *btGoSutra;
	TButton *btGoBook;
	TButton *btGoByKeyword;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn7;
	TEdit *edFindSutra_VolFrom;
	TEdit *edFindSutra_VolTo;
	TEdit *edFindSutra_SutraFrom;
	TEdit *edFindSutra_SutraTo;
	TLabel *Label15;
	TLabel *lbFindSutra_SutraNum;
	TLabel *lbFindSutra_SutraName;
	TLabel *Label34;
	TLabel *lbFindSutra_Book;
	TComboBox *cbFindSutra_BookId;
	TLabel *Label17;
	TLabel *Label18;
	TCornerButton *btOption;
	TTabItem *TabItem5;
	TPanel *Panel5;
	TLabel *Label19;
	TEdit *edTextSearch;
	TStringGrid *sgTextSearch;
	TStringColumn *StringColumn9;
	TStringColumn *StringColumn10;
	TStringColumn *StringColumn11;
	TStringColumn *StringColumn12;
	TStringColumn *StringColumn13;
	TStringColumn *StringColumn14;
	TStringColumn *StringColumn15;
	TButton *btTextSearch;
	TLabel *lbSearchMsg;
	TStringColumn *StringColumn17;
	TCheckBox *cbSearchRange;
	TStyleBook *sbMid;
	TPanel *Panel8;
	TButton *btOpenBuleiNav;
	TButton *btOpenBookNav;
	TCornerButton *btPrevJuan;
	TCornerButton *btNextJuan;
	TMenuItem *mmiAbout;
	TMenuItem *mmiUpdate;
	TCornerButton *btNavWidthSwitch;
	TCornerButton *btMuluWidthSwitch;
	TFloatAnimation *fanNavWidth;
	TPanel *Panel1;
	TPanel *pnMulu;
	TFloatAnimation *fanMuluWidth;
	TTreeView *tvMuluTree;
	TPanel *Panel7;
	TLabel *Label20;
	TSplitter *Splitter3;
	TLabel *Label21;
	TRadioButton *rbFontSmall;
	TRadioButton *rbFontMid;
	TRadioButton *rbFontBig;
	TStyleBook *sbBig;
	TStyleBook *sbSmall;
	TImageList *ImageList1;
	TButton *btOpenSimpleNav;
	TMenuBar *MenuBar;
	TMenuItem *wmiOPtion;
	TMenuItem *wmiUpdate;
	TMenuItem *wmiAbout;
	TMenuItem *MenuItem5;
	TMenuItem *wmiDebug;
	TMenuItem *wmiUpdateURL;
	TMenuItem *wmiCreateHtml;
	TMenuItem *wmiBuildIndex;
	TLabel *Label22;
	TLabel *Label23;
	TCheckBox *cbSearchThisSutra;
	TMenuItem *wmiHelp;
	TMenuItem *wmiOnlineDoc;
	TMenuItem *wmiOnlineFAQ;
	TMenuItem *mmiHelp;
	TMenuItem *mmiOnlineDoc;
	TMenuItem *mmiOnlineFAQ;
	TLabel *lbFindSutraCount;
	TButton *btBoolean;
	TPopupMenu *pmBoolean;
	TMenuItem *miNear;
	TMenuItem *miBefore;
	TMenuItem *miAnd;
	TMenuItem *miOr;
	TMenuItem *miExclude;
	TStringColumn *StringColumn8;
	TStringColumn *StringColumn16;
	TCornerButton *btCopy;
	TCornerButton *btCiteCopy;

	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btOpenBookcaseClick(TObject *Sender);
	void __fastcall btFindSutraClick(TObject *Sender);
	void __fastcall btGoSutraClick(TObject *Sender);
	void __fastcall btGoBookClick(TObject *Sender);
	void __fastcall sgFindSutraCellDblClick(TColumn * const Column, const int Row);
	void __fastcall btOptionClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btTextSearchClick(TObject *Sender);
	void __fastcall sgTextSearchCellDblClick(TColumn * const Column, const int Row);
	void __fastcall cbSearchRangeChange(TObject *Sender);
	void __fastcall btGoByKeywordClick(TObject *Sender);
	void __fastcall btOpenBuleiNavClick(TObject *Sender);
	void __fastcall btOpenBookNavClick(TObject *Sender);
	void __fastcall btPrevJuanClick(TObject *Sender);
	void __fastcall btNextJuanClick(TObject *Sender);
	void __fastcall mmiAboutClick(TObject *Sender);
	void __fastcall mmiUpdateClick(TObject *Sender);
	void __fastcall btNavWidthSwitchClick(TObject *Sender);
	void __fastcall btMuluWidthSwitchClick(TObject *Sender);
	void __fastcall fanNavWidthFinish(TObject *Sender);
	void __fastcall fanMuluWidthFinish(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall rbFontSmallChange(TObject *Sender);
	void __fastcall btOpenSimpleNavClick(TObject *Sender);
	void __fastcall wmiUpdateURLClick(TObject *Sender);
	void __fastcall wmiCreateHtmlClick(TObject *Sender);
	void __fastcall wmiBuildIndexClick(TObject *Sender);
	void __fastcall edFindSutra_VolFromEnter(TObject *Sender);
	void __fastcall edGoSutra_SutraNumEnter(TObject *Sender);
	void __fastcall edGoBook_VolEnter(TObject *Sender);
	void __fastcall edGoByKeywordEnter(TObject *Sender);
	void __fastcall edTextSearchEnter(TObject *Sender);
	void __fastcall cbSearchThisSutraChange(TObject *Sender);
	void __fastcall wmiOnlineDocClick(TObject *Sender);
	void __fastcall wmiOnlineFAQClick(TObject *Sender);
	void __fastcall mmiOnlineDocClick(TObject *Sender);
	void __fastcall mmiOnlineFAQClick(TObject *Sender);
	void __fastcall miNearClick(TObject *Sender);
	void __fastcall btBooleanClick(TObject *Sender);
	void __fastcall miOrClick(TObject *Sender);
	void __fastcall miExcludeClick(TObject *Sender);
	void __fastcall miAndClick(TObject *Sender);
	void __fastcall miBeforeClick(TObject *Sender);
	void __fastcall btCopyClick(TObject *Sender);
	void __fastcall btCiteCopyClick(TObject *Sender);



private: // User declarations

	String __fastcall GetTodayString();   // ���o������, �榡 19991231
	void __fastcall SetPermissions(int iIE); // �]�w TWebBrowser �� IE ����
	void __fastcall NavTreeItemClick(TObject *Sender); // NavTree Item �I�G�U���@��

public: // User declarations

    String Version;     	// ����
	String ProgramTitle;    // �{���W��
	bool IsDebug;           // debug �ܼ�
    String DebugString;     // debug �f�O

    int SelectedBook;   // �ثe�襤����, -1 ����٨S��

	String MyFullPath;
	String MyTempPath;  // �s��Ȯ��ɪ��ؿ�
	String MyHomePath;  // �ӤH�ؿ�
	String MySettingPath;  // �p�H�ؿ�, �n��]�w��
	String SettingFile;    // �]�w��

	CSetting * Setting; // �]�w��
	CBookcase * Bookcase; // ���d
	CNavTree * NavTree; // ������� (�Ȯɪ�, ���|��b Serial ���� ???)
	CNavTree * MuluTree; // ��g������� (�Ȯɪ�, ���|��b Serial ���� ???)

    TmyMonster * SearchEngine;   // �����˯�����

	TStringList * SearchWordList;	// �s��C�@���˯�����, �����|�Ψ�
	String SearchSentence;	// �j�M�r��

	int SpineID;    // �ثe�}�Ҫ��ɮ�, �ΨӳB�z�W�@���M�U�@���Ϊ�

	int NavWidth;   // �ؿ��Ϫ��e��
	int MuluWidth;  // �ѥذϪ��e��

	void __fastcall InitialPath(); // 	���|��ȳ]�w

	bool __fastcall IsSelectedBook(); // �O�_����ܮM�ѤF?
	// �}�ҫ��w�����d
	void __fastcall OpenBookcase(int iId);
	// �}��CBETA���d
	void __fastcall OpenCBETABook();

	// �N�ɮ׸��J������
	void __fastcall LoadNavTree(String sFile);
	// �N�ɮ׸��J�ؿ���
	void __fastcall LoadMuluTree(String sFile);

	// ���J XML �óB�z������
	void __fastcall ShowCBXML(String sFile, bool bShowHighlight = false, TmyMonster * SearchEngine = 0);

	// �ˬd���S����s�{��
	void __fastcall CheckUpdate(bool bShowNoUpdate=false);

    // ��ܥ����˯�����
	void __fastcall SetSearchEngine();

	// �N�Ҧ��� Default ����
	void __fastcall CancelAllDefault();

	// ��l���
	void __fastcall InitialData();

	// �ۭq���j�M�C�� OnKeyDown
	void __fastcall sgTextSearchKeyDown(TObject *Sender, System::Word &Key, System::WideChar &KeyChar, System::Classes::TShiftState Shift);
	// �ۭq���ؿ��C�� OnKeyDown
	void __fastcall sgFindSutraKeyDown(TObject *Sender, System::Word &Key, System::WideChar &KeyChar, System::Classes::TShiftState Shift);

	__fastcall TfmMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
// ---------------------------------------------------------------------------
#endif
