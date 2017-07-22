// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"

#ifdef _Windows
#include <System.Win.Registry.hpp>
#endif

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

TfmMain *fmMain;

// ---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner) : TForm(Owner)
{
	// ���o�]�w�ɨ�Ū��

#ifdef _Windows
	SetPermissions(); // �N IE �]�w�� IE 11 (�p�G�S IE 11 ���p��?)
#endif

#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012";
#endif

	MyFullPath += "/";
	SettingFile = "cbreader.ini";
	Setting = new CSetting;

	// ���o Bookcase ���ؿ�

	Bookcase = new CBookcase();
	// Bookcase->LoadBooks(Setting->BookcaseDir);

	// �b���d����O��

	// ���J�O�Ѫ��_�l�ؿ�
	NavTree = new CNavTree(MyFullPath + "nav.xhtml");
}

// ---------------------------------------------------------------------------
void __fastcall TfmMain::FormDestroy(TObject *Sender)
{
	delete Setting;
	delete Bookcase;
	delete NavTree;
}

// ---------------------------------------------------------------------------
void __fastcall TfmMain::CornerButton1Click(TObject *Sender)
{
	WebBrowser1->URL = "file://" + MyFullPath + "Bookcase/Agama/T02n0099_001.htm";
	// WebBrowser1->URL = "https://www.w3schools.com/html/tryit.asp?filename=tryhtml5_video";
	WebBrowser1->Navigate();
}

// ---------------------------------------------------------------------------
// �N IE �]�w�� IE 11
// copy from
// http://docwiki.embarcadero.com/Libraries/Tokyo/en/FMX.WebBrowser.TWebBrowser
//
// �Ѧ� MSDN
// https://msdn.microsoft.com/en-us/library/ee330730%28v=vs.85%29.aspx#browser_emulation
void __fastcall TfmMain::SetPermissions()
{
#ifdef _Windows
	UnicodeString cHomePath = "SOFTWARE";
	UnicodeString cFeatureBrowserEmulation = "Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION\\";
	int cIE11 = 11001;
	UnicodeString sKey = ExtractFileName(ParamStr(0));
	TRegIniFile *Reg = new TRegIniFile(cHomePath);
	__try
	{
		TRegistry *reg1 = dynamic_cast<TRegistry*>(Reg);
		if (Reg->OpenKey(cFeatureBrowserEmulation,
			true) && !(reg1->KeyExists(sKey) && reg1->ReadInteger(sKey)
			== cIE11))
		{
			reg1->WriteInteger(sKey, cIE11);
		}
	}
	__finally
	{
		Reg->Free();
	}
#endif
}
// ---------------------------------------------------------------------------
