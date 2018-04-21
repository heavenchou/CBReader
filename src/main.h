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
// ---------------------------------------------------------------------------

class TfmMain : public TForm
{
__published: // IDE-managed Components
	TMainMenu *MainMenu1;
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
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
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
	TLabel *Label35;
	TLabel *Label33;
	TLabel *Label34;
	TLabel *Label16;
	TComboBox *cbFindSutra_BookId;
	TLabel *Label17;
	TLabel *Label18;
	TStringColumn *StringColumn8;
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
	TStringColumn *StringColumn16;
	TButton *btTextSearch;
	TLabel *lbSearchMsg;
	TStringColumn *StringColumn17;
	TCheckBox *cbSearchRange;
	TCornerButton *btBuildIndex;
	TStyleBook *sbMid;
	TPanel *Panel8;
	TButton *btOpenBuleiNav;
	TButton *btOpenBookNav;
	TCornerButton *btPrevJuan;
	TCornerButton *btNextJuan;
	TMenuItem *MenuItem1;
	TMenuItem *MenuItem2;
	TMenuItem *MenuItem4;
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
	TStringColumn *StringColumn18;
	TLabel *Label21;
	TRadioButton *rbFontSmall;
	TRadioButton *rbFontMid;
	TRadioButton *rbFontBig;
	TStyleBook *sbBig;
	TStyleBook *sbSmall;

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
	void __fastcall btBuildIndexClick(TObject *Sender);
	void __fastcall btOpenBuleiNavClick(TObject *Sender);
	void __fastcall btOpenBookNavClick(TObject *Sender);
	void __fastcall btPrevJuanClick(TObject *Sender);
	void __fastcall btNextJuanClick(TObject *Sender);
	void __fastcall MenuItem1Click(TObject *Sender);
	void __fastcall MenuItem4Click(TObject *Sender);
	void __fastcall btNavWidthSwitchClick(TObject *Sender);
	void __fastcall btMuluWidthSwitchClick(TObject *Sender);
	void __fastcall fanNavWidthFinish(TObject *Sender);
	void __fastcall fanMuluWidthFinish(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall rbFontSmallChange(TObject *Sender);



private: // User declarations

	void __fastcall SetPermissions(int iIE); // 設定 TWebBrowser 的 IE 版本
	void __fastcall NavTreeItemClick(TObject *Sender); // NavTree Item 點二下的作用

public: // User declarations

    String ProgramTitle;    // 程式名稱

    int SelectedBook;   // 目前選中的書, -1 表示還沒選
	String SettingFile;
	String MyFullPath;
	String MyTempPath;  // 存放暫時檔的目錄
    String MyHomePath;  // 私人目錄, 要放設定檔

	CSetting * Setting; // 設定檔
	CBookcase * Bookcase; // 書櫃
	CNavTree * NavTree; // 導覽文件 (暫時的, 日後會放在 Serial 物件中 ???)
	CNavTree * MuluTree; // 單經導覽文件 (暫時的, 日後會放在 Serial 物件中 ???)

	TStringList * SearchWordList;	// 存放每一個檢索的詞, 日後塗色會用到
	String SearchSentence;	// 搜尋字串

	int SpineID;    // 目前開啟的檔案, 用來處理上一卷和下一卷用的

	int NavWidth;   // 目錄區的寬度
	int MuluWidth;  // 書目區的寬度

	void __fastcall InitialPath(); // 	路徑初值設定

	bool __fastcall IsSelectedBook(); // 是否有選擇套書了?
	// 開啟指定的書櫃
	void __fastcall OpenBookcase(int iId);
	// 開啟CBETA書櫃
	void __fastcall OpenCBETABook();

	// 將檔案載入導覽樹
	void __fastcall LoadNavTree(String sFile);
	// 將檔案載入目錄樹
	void __fastcall LoadMuluTree(String sFile);

	// 載入 XML 並處理成網頁
	void __fastcall ShowCBXML(String sFile, bool bShowHighlight = false, TmyMonster * SearchEngine = 0);

	// 檢查有沒有更新程式
	void __fastcall CheckUpdate(String sPara);

	// 初始資料
	void __fastcall InitialData();
	__fastcall TfmMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
// ---------------------------------------------------------------------------
#endif
