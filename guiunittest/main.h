//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>

#include <FMX.Controls.Presentation.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.TreeView.hpp>
#include "sample.h"
#include "navtreetest.h"
#include "cbxmltest.h"
#include "bookcasetest.h"
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TCornerButton *btShowTestResult;
	TListBox *lbResult;
	TTreeView *TreeView;
	void __fastcall btShowTestResultClick(TObject *Sender);
private:	// User declarations
	// 儲存結果的字串列表
	TStringList * Titles;   // 說明
	TStringList * Results;  // 結果 0 : 錯誤, 1: 成功, ...

	String TestString;  // 測試用字串
public:		// User declarations

	// 要測試的物件
	CSample * Sample;
	CNavTreeTest * NavTreeTest;
	CCBXMLTest * CBXMLTest;
    CBookcaseTest * BookcaseTest;

    // 在測試結果上加標題, 以便區分
	void __fastcall ListBoxAddHead(String sHead);


	// Nav 樹狀目錄 Item Click 測試功能
	void __fastcall NavTreeItemClick(TObject *Sender);
	// Nav 樹狀目錄測試
	void __fastcall TreeViewRunAllTest(void);
	// 儲存測試結果
	void __fastcall LogTest(String sTitle, bool bResult);
	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
