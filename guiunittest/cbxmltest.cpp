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

	CBXML = new CCBXML("T01n0003_001.xml");
}
//---------------------------------------------------------------------------
// �����������
void __fastcall CCBXMLTest::RunAllTest()
{
	LogTest(u"�x�sHTML (always true)", CBXMLSaveToHTML());
}
//---------------------------------------------------------------------------
// �x�s���յ��G
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
	CBXML->SaveToHTML("d:/Data/c/_CppBuilder/CBReader/guiunittest/Win32/Debug/T01n0003_001.htm");
	return true;
}