//---------------------------------------------------------------------------

//#include <fmx.h>
#pragma hdrstop

#include "sample.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall CSample::CSample(TStringList * slTitle, TStringList * slResult)
{
	Titles = slTitle;
	Results = slResult;
}
//---------------------------------------------------------------------------
// 執行全部測試
void __fastcall CSample::RunAllTest()
{
	LogTest("測試一", Test1());
	LogTest("測試二", Test2());
}
//---------------------------------------------------------------------------
// 儲存測試結果
void __fastcall CSample::LogTest(String sTitle, bool bResult)
{
	String s = "0";
	if(bResult) s = "1";
	Titles->Add(sTitle);
	Results->Add(s);
}
//---------------------------------------------------------------------------
// 每一個測試項目
//---------------------------------------------------------------------------
bool __fastcall CSample::Test1()
{
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall CSample::Test2()
{
	return false;
}
//---------------------------------------------------------------------------

