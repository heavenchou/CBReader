//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "bookcasetest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall CBookcaseTest::CBookcaseTest(TStringList * slTitle, TStringList * slResult)
{
	Titles = slTitle;
	Results = slResult;

#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012/gui-test";
#endif

	MyFullPath += "/";
	Bookcase = new CBookcase();
	Bookcase->LoadBookcase(MyFullPath + "Bookcase/");
}
//---------------------------------------------------------------------------
// 執行全部測試
void __fastcall CBookcaseTest::RunAllTest()
{
	LogTest(u"找到 Agama 目錄", BookcaseFindDir());
	LogTest(u"測試是否找到 2 本書", BookcaseCount());
	LogTest(u"測試讀取第一本書 Title", ReadSeriesTitle());
	LogTest(u"測試讀取第二本書 Nav 位置", ReadSeriesNav());
}
//---------------------------------------------------------------------------
// 儲存測試結果
void __fastcall CBookcaseTest::LogTest(String sTitle, bool bResult)
{
	String s = "0";
	if(bResult) s = "1";
	Titles->Add(sTitle);
	Results->Add(s);
}
//---------------------------------------------------------------------------
// 每一個測試項目
//---------------------------------------------------------------------------
// NavTree 讀取四種資料
bool __fastcall CBookcaseTest::BookcaseFindDir()
{
	Series = (CSeries *) Bookcase->Books->Items[0];
	String s = Series->Dir;

	return (s.Pos("Agama")>0);
}
//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::BookcaseCount()
{
	return (Bookcase->Count() == 2);
}

//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ReadSeriesTitle()
{
	Series = (CSeries *) Bookcase->Books->Items[0];
	String s = Series->Title;
	return (s == u"阿含經");
}

//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ReadSeriesNav()
{
	Series = (CSeries *) Bookcase->Books->Items[1];
	String s = Series->NavFile;
	return (s == u"ys_nav.xhtml");
}

//---------------------------------------------------------------------------

