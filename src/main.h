//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
using namespace std;

class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TPanel *Panel1;
	TTabControl *TabControl1;
	TPanel *Panel2;
	TTabItem *TabItem1;
	TWebBrowser *WebBrowser1;
	TToolBar *ToolBar1;
	TCornerButton *CornerButton1;
	TMenuItem *MenuItem3;
	TMenuItem *MenuItem4;
	TTreeView *TreeView1;
	TSplitter *Splitter1;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall CornerButton1Click(TObject *Sender);

private:	// User declarations

public:		// User declarations

	string SettingFile;
	String MyFullPath;

	CSetting * Setting;   // ³]©wÀÉ
	CBookcase * Bookcase; // ®ÑÂd

	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
