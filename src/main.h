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
#include "navtree.h"
#include "cbxml.h"
#include <FMX.ActnList.hpp>
#include <FMX.StdActns.hpp>
#include <System.Actions.hpp>
#include <FMX.Edit.hpp>
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
	TTreeView *tvNavTree;
	TSplitter *Splitter1;
	TEdit *edBookcasePath;
	TCornerButton *btSetBookcasePath;
	TCheckBox *CheckBox1;
	TCornerButton *CornerButton2;
	TCornerButton *CornerButton3;

	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall CornerButton1Click(TObject *Sender);
	void __fastcall btSetBookcasePathClick(TObject *Sender);
	void __fastcall CheckBox1Change(TObject *Sender);
	void __fastcall CornerButton2Click(TObject *Sender);
	void __fastcall CornerButton3Click(TObject *Sender);

private: // User declarations

	void __fastcall SetPermissions(); // 設定 TWebBrowser 的 IE 版本
	void __fastcall NavTreeItemClick(TObject *Sender); // NavTree Item 點二下的作用

public: // User declarations

	String SettingFile;
	String MyFullPath;

	CSetting * Setting; // 設定檔
	CBookcase * Bookcase; // 書櫃
	CNavTree * NavTree; // 導覽文件 (暫時的, 日後會放在 Serial 物件中 ???)

	__fastcall TfmMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
// ---------------------------------------------------------------------------
#endif
