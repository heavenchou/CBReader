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

	// 測試物件
	Sample = new CSample(Titles, Results);
	ListBoxAddHead("測試示範");
	Sample->RunAllTest();
	ListBoxAddHead("測試示範2");
	Sample->RunAllTest();
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
void __fastcall TfmMain::btRunAllTestClick(TObject *Sender)
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
