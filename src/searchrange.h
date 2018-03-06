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
	TPanel *Panel3;
	TButton *btOK;
	TButton *btCancle;
	TTabControl *TabControl1;
	TTabItem *TabItem1;
	TTreeView *tvBulei;
	TTreeViewItem *triBulei0;
	TTreeViewItem *triBulei1;
	TTreeViewItem *triBulei2;
	TTreeViewItem *TreeViewItem20;
	TTreeViewItem *TreeViewItem21;
	TTreeViewItem *TreeViewItem22;
	TTreeViewItem *TreeViewItem23;
	TTreeViewItem *TreeViewItem24;
	TTreeViewItem *TreeViewItem25;
	TTreeViewItem *TreeViewItem26;
	TTreeViewItem *TreeViewItem27;
	TTreeViewItem *TreeViewItem28;
	TTreeViewItem *TreeViewItem29;
	TTreeViewItem *TreeViewItem30;
	TTreeViewItem *TreeViewItem31;
	TTreeViewItem *TreeViewItem32;
	TTreeViewItem *TreeViewItem33;
	TTreeViewItem *TreeViewItem34;
	TTreeViewItem *TreeViewItem35;
	TTreeViewItem *TreeViewItem36;
	TTreeViewItem *TreeViewItem37;
	TTreeViewItem *TreeViewItem38;
	TTreeViewItem *TreeViewItem39;
	TTreeViewItem *TreeViewItem40;
	TTabItem *TabItem2;
	TTreeView *tvBook;
	TTreeViewItem *tviT;
	TTreeViewItem *tviX;
	TTreeViewItem *tviY;
	TTreeViewItem *TreeViewItem1;
	TTreeViewItem *TreeViewItem2;
	TTreeViewItem *TreeViewItem3;
	TTreeViewItem *TreeViewItem4;
	TTreeViewItem *TreeViewItem5;
	TTreeViewItem *TreeViewItem6;
	TTreeViewItem *TreeViewItem7;
	TTreeViewItem *TreeViewItem8;
	TTreeViewItem *TreeViewItem9;
	TTreeViewItem *TreeViewItem10;
	TTreeViewItem *TreeViewItem11;
	TTreeViewItem *TreeViewItem12;
	TTreeViewItem *TreeViewItem13;
	TTreeViewItem *TreeViewItem14;
	TTreeViewItem *TreeViewItem15;
	TTreeViewItem *TreeViewItem16;
	TTreeViewItem *TreeViewItem17;
	TTreeViewItem *TreeViewItem18;
	TTreeViewItem *TreeViewItem19;
	TButton *btCheckAll;
	TButton *btUnCheckAll;
	void __fastcall ListBoxItem1Click(TObject *Sender);
	void __fastcall ListBoxItem2Click(TObject *Sender);
	void __fastcall btOKClick(TObject *Sender);
	void __fastcall btCheckAllClick(TObject *Sender);
	void __fastcall btUnCheckAllClick(TObject *Sender);
private:	// User declarations

	void __fastcall TreeViewCheckAll(TTreeView * tvTreeView, bool bChecked); // 某 Treeview 全選

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
