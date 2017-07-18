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
	// 取得設定檔並讀取

	MyFullPath = ExtractFilePath(ParamStr(0));
	SettingFile = "cbreader.ini";
	Setting = new CSetting;

	// 取得 Bookcase 的目錄

	Bookcase = new CBookcase();
	//Bookcase->LoadBooks(Setting->BookcaseDir);

	// 在書櫃選擇叢書

	// 載入叢書的起始目錄
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormDestroy(TObject *Sender)
{
	delete Setting;
	delete Bookcase;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::CornerButton1Click(TObject *Sender)
{
	WebBrowser1->URL = "file://" + MyFullPath + "Bookcase/Agama/T02n0099_001.htm";
	WebBrowser1->Navigate();
}
//---------------------------------------------------------------------------

