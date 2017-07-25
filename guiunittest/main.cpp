//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
	: TForm(Owner)
{
	// 儲存結果的字串列表
	Titles = new TStringList();
	Results = new TStringList();
	TestString = "";

	// 測試物件
	ListBoxAddHead(u"測試示範");
	Sample = new CSample(Titles, Results);
	Sample->RunAllTest();

	ListBoxAddHead(u"NavTree 測試");
	NavTreeTest = new CNavTreeTest(Titles, Results);
	NavTreeTest->RunAllTest();

	ListBoxAddHead(u"TreeView 樹狀目錄測試");
	// 把樹狀目錄呈現, 並且測試 click 的結果
	NavTreeTest->NavTree->SaveToTreeView(TreeView, NavTreeItemClick);
	TreeViewRunAllTest();	// Nav 樹狀目錄測試

}
//---------------------------------------------------------------------------
// 在測試結果上加標題, 以便區分
void __fastcall TfmMain::ListBoxAddHead(String sHead)
{
	Titles->Add(sHead);
	Results->Add("H");
}
//---------------------------------------------------------------------------
// 列出結果
void __fastcall TfmMain::btShowTestResultClick(TObject *Sender)
{
	for (int i = 0; i < Titles->Count; i++)
	{
		if(Results->Strings[i] == "H")
		{
			// 這是一個標題
			TListBoxGroupHeader * lbHead = new TListBoxGroupHeader(lbResult);
			lbHead->Text = Titles->Strings[i];
			lbResult->AddObject(lbHead);
		}
		else
		{
			lbResult->Items->Add(Titles->Strings[i]);
			if(Results->Strings[i] == "1")
			{
				// 測試正確則打勾
				lbResult->ListItems[i]->IsChecked = true;
			}
			else
			{
				// 失敗則塗紅色
				lbResult->ListItems[i]->StyledSettings = lbResult->ListItems[i]->StyledSettings >> TStyledSetting::FontColor;
				lbResult->ListItems[i]->TextSettings->FontColor = 0xffff0000;
			}
		}
	}
}
//---------------------------------------------------------------------------
// Nav 樹狀目錄 Item Click 測試功能
void __fastcall TfmMain::NavTreeItemClick(TObject *Sender)
{
	TTreeViewItem * nItem = (TTreeViewItem *)Sender;

	TestString += nItem->Tag;
	TestString += ",";
	TestString += nItem->Text;
	TestString += ",";
	TestString += nItem->TagString;
}
//---------------------------------------------------------------------------
// Nav 樹狀目錄測試
void __fastcall TfmMain::TreeViewRunAllTest(void)
{
	bool bResult = false;
	// Click 會在 TestString 傳回結果
	NavTreeItemClick(TreeView->Items[0]->Items[0]->Items[0]);
	if(TestString == u"3,CBETA經文,T01n0001_001#p0001a01") bResult = true;

	LogTest(u"讀取樹狀目錄資料", bResult);
}
//---------------------------------------------------------------------------
// 儲存測試結果
void __fastcall TfmMain::LogTest(String sTitle, bool bResult)
{
	String s = "0";
	if(bResult) s = "1";
	Titles->Add(sTitle);
	Results->Add(s);
}
//---------------------------------------------------------------------------


