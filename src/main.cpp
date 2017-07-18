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
	// ���o�]�w�ɨ�Ū��

	MyFullPath = ExtractFilePath(ParamStr(0));
	SettingFile = "cbreader.ini";
	Setting = new CSetting;

	// ���o Bookcase ���ؿ�

	Bookcase = new CBookcase();
	//Bookcase->LoadBooks(Setting->BookcaseDir);

	// �b���d����O��

	// ���J�O�Ѫ��_�l�ؿ�
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

