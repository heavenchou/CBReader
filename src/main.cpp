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

#ifdef _Windows
	SetPermissions();   // 將 IE 設定到 IE 11 (如果沒 IE 11 的如何?)
#endif
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
	//WebBrowser1->URL = "https://www.w3schools.com/html/tryit.asp?filename=tryhtml5_video";
	WebBrowser1->Navigate();
}
//---------------------------------------------------------------------------
// 將 IE 設定為 IE 11
// copy from
// http://docwiki.embarcadero.com/Libraries/Tokyo/en/FMX.WebBrowser.TWebBrowser
//
// 參考 MSDN
// https://msdn.microsoft.com/en-us/library/ee330730%28v=vs.85%29.aspx#browser_emulation

void __fastcall TfmMain::SetPermissions() {
#ifdef _Windows
	UnicodeString cHomePath = "SOFTWARE";
	UnicodeString cFeatureBrowserEmulation = "Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION\\";
	int cIE11 = 11001;
	UnicodeString sKey = ExtractFileName(ParamStr(0));
	TRegIniFile *Reg = new TRegIniFile(cHomePath);
	__try {
		TRegistry *reg1 = dynamic_cast<TRegistry*>(Reg);
		if (Reg->OpenKey(cFeatureBrowserEmulation,
			true) && !(reg1->KeyExists(sKey) && reg1->ReadInteger(sKey)
			== cIE11)) {
			reg1->WriteInteger(sKey, cIE11);
		}
	}
	__finally {
		Reg->Free();
	}
#endif
}
//---------------------------------------------------------------------------

