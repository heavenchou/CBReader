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
#include "option.h"
#include "../../Monster/src/monster.h"
// ---------------------------------------------------------------------------

class TfmMain : public TForm
{
__published: // IDE-managed Components
	TMainMenu *MainMenu1;
	TPanel *Panel1;
	TTabControl *TabControl1;
	TPanel *Panel2;
	TTabItem *TabItem1;
	TWebBrowser *WebBrowser;
	TToolBar *ToolBar1;
	TCornerButton *CornerButton1;
	TMenuItem *MenuItem3;
	TMenuItem *MenuItem4;
	TSplitter *Splitter1;
	TCornerButton *CornerButton2;
	TCornerButton *CornerButton3;
	TTabControl *TabControl2;
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
	TComboBox *ComboBox1;
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
	TComboBox *ComboBox2;
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
	TLabel *�g��;
	TLabel *�g�W;
	TLabel *�@Ķ��;
	TLabel *Label16;
	TComboBox *ComboBox3;
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

	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall CornerButton1Click(TObject *Sender);
	void __fastcall CornerButton2Click(TObject *Sender);
	void __fastcall CornerButton3Click(TObject *Sender);
	void __fastcall btFindSutraClick(TObject *Sender);
	void __fastcall btGoSutraClick(TObject *Sender);
	void __fastcall btGoBookClick(TObject *Sender);
	void __fastcall sgFindSutraCellDblClick(TColumn * const Column, const int Row);
	void __fastcall btOptionClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btTextSearchClick(TObject *Sender);
	void __fastcall sgTextSearchCellDblClick(TColumn * const Column, const int Row);



private: // User declarations

	void __fastcall SetPermissions(int iIE); // �]�w TWebBrowser �� IE ����
	void __fastcall NavTreeItemClick(TObject *Sender); // NavTree Item �I�G�U���@��

public: // User declarations


    int SelectedBook;   // �ثe�襤����, -1 ����٨S��
	String SettingFile;
	String MyFullPath;
	String MyTempPath;  // �s��Ȯ��ɪ��ؿ�

	CSetting * Setting; // �]�w��
	CBookcase * Bookcase; // ���d
	CNavTree * NavTree; // ������� (�Ȯɪ�, ���|��b Serial ���� ???)

	TStringList * SearchWordList;	// �s��C�@���˯�����, �����|�Ψ�
    String SearchSentence;	// �j�M�r��


	bool __fastcall IsSelectedBook(); // �O�_����ܮM�ѤF?
	// �}�ҫ��w�����d
	void __fastcall OpenBookcase(int iId);

	// ���J XML �óB�z������
	void __fastcall ShowCBXML(String sFile, bool bShowHighlight = false, TmyMonster * SearchEngine = 0);

	__fastcall TfmMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
// ---------------------------------------------------------------------------
#endif
