//---------------------------------------------------------------------------

//#include <fmx.h>
#pragma hdrstop

#include "navtreetest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall CNavTreeTest::CNavTreeTest(TStringList * slTitle, TStringList * slResult)
{
	Titles = slTitle;
	Results = slResult;

#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012/gui-test";
#endif

	MyFullPath += "/";
	NavTree = new CNavTree(MyFullPath + "Bookcase/Test/nav.xhtml");
}
//---------------------------------------------------------------------------
// 執行全部測試
void __fastcall CNavTreeTest::RunAllTest()
{
	LogTest(u"讀取純標題", NavTreeReadTitle());
	LogTest(u"讀取一般連結", NavTreeReadLink());
	LogTest(u"讀取目錄連結", NavTreeReadNavLink());
	LogTest(u"讀取CB經文連結", NavTreeReadCBLink());
}
//---------------------------------------------------------------------------
// 儲存測試結果
void __fastcall CNavTreeTest::LogTest(String sTitle, bool bResult)
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
bool __fastcall CNavTreeTest::NavTreeReadTitle()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[0];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	return (s == u"0,0,佛教資料,");
}
//---------------------------------------------------------------------------
bool __fastcall CNavTreeTest::NavTreeReadLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[2];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s 應該是 "2,3,CBETA經文,T01n0001_001#p0001a01"
	return (s == u"2,1,Unicode Test,Book/unicodetest.htm");
}
//---------------------------------------------------------------------------
bool __fastcall CNavTreeTest::NavTreeReadNavLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[9];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s 應該是 "2,3,連結大正藏導覽文件,Taisho.xml"
	return (s == u"1,2,第二個目錄,nav2.xhtml");
}
//---------------------------------------------------------------------------
bool __fastcall CNavTreeTest::NavTreeReadCBLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[11];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s 應該是 "2,3,CBETA經文,T01n0001_001#p0001a01"
	return (s == u"1,3,毘婆尸佛經,Book/T01n0003_001.xml");
}
//---------------------------------------------------------------------------

