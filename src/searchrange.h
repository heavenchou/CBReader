//---------------------------------------------------------------------------

#ifndef searchrangeH
#define searchrangeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.TreeView.hpp>
//---------------------------------------------------------------------------
class TfmSearchRange : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSplitter *Splitter1;
	TListBox *lbSearchRangeList;
	TListBoxItem *ListBoxItem1;
	TListBoxItem *ListBoxItem2;
	TPanel *Panel2;
	TTabControl *TabControl1;
	TTabItem *TabItem1;
	TTreeView *tvBulei;
	TTreeViewItem *triBulei0;
	TTreeViewItem *triBulei1;
	TTabItem *TabItem2;
	TPanel *Panel3;
	TTreeViewItem *triBulei2;
	TTreeView *tvBook;
	TTreeViewItem *tviT;
	TTreeViewItem *tviX;
	TTreeViewItem *tviY;
	TButton *btOK;
	TButton *btCancle;
	void __fastcall ListBoxItem1Click(TObject *Sender);
	void __fastcall ListBoxItem2Click(TObject *Sender);
	void __fastcall btOKClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	// 處理部類
	void __fastcall BuleiSelect();
	// 處理原書
	void __fastcall BookSelect();

	__fastcall TfmSearchRange(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmSearchRange *fmSearchRange;
//---------------------------------------------------------------------------
#endif
