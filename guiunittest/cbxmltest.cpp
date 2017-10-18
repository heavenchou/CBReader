//---------------------------------------------------------------------------

#pragma hdrstop

#include "cbxmltest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall CCBXMLTest::CCBXMLTest(TStringList * slTitle, TStringList * slResult)
{
	Titles = slTitle;
	Results = slResult;

#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012/gui-test";
#endif

	MyFullPath += "/";

	CBXMLOption = new CCBXMLOption(); // 經文呈現設定檔

	CBXML = new CCBXML(MyFullPath + "Bookcase/Agama/T01n0003_001.xml", CBXMLOption);
}
//---------------------------------------------------------------------------
// 執行全部測試
void __fastcall CCBXMLTest::RunAllTest()
{
	LogTest(u"儲存 HTML", CBXMLSaveToHTML());
}
//---------------------------------------------------------------------------
// 儲存測試結果
void __fastcall CCBXMLTest::LogTest(String sTitle, bool bResult)
{
	String s = "0";
	if(bResult) s = "1";
	Titles->Add(sTitle);
	Results->Add(s);
}
//---------------------------------------------------------------------------
bool __fastcall CCBXMLTest::CBXMLSaveToHTML()
{
	String sOUT = MyFullPath + "Bookcase/Agama/T01n0003_001.htm";

	if(TFile::Exists(sOUT))
	{
		TFile::Delete(sOUT);
	}

	CBXML->SaveToHTML(sOUT);

	if(TFile::Exists(sOUT))
		return true;
	else
		return false;
}