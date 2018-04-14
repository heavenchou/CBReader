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
// �����������
void __fastcall CNavTreeTest::RunAllTest()
{
	LogTest(u"Ū���¼��D", NavTreeReadTitle());
	LogTest(u"Ū���@��s��", NavTreeReadLink());
	LogTest(u"Ū���ؿ��s��", NavTreeReadNavLink());
	LogTest(u"Ū��CB�g��s��", NavTreeReadCBLink());
}
//---------------------------------------------------------------------------
// �x�s���յ��G
void __fastcall CNavTreeTest::LogTest(String sTitle, bool bResult)
{
	String s = "0";
	if(bResult) s = "1";
	Titles->Add(sTitle);
	Results->Add(s);
}
//---------------------------------------------------------------------------
// �C�@�Ӵ��ն���
//---------------------------------------------------------------------------
// NavTree Ū���|�ظ��
bool __fastcall CNavTreeTest::NavTreeReadTitle()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[0];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	return (s == u"0,0,��и��,");
}
//---------------------------------------------------------------------------
bool __fastcall CNavTreeTest::NavTreeReadLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[2];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s ���ӬO "2,3,CBETA�g��,T01n0001_001#p0001a01"
	return (s == u"2,1,Unicode Test,Book/unicodetest.htm");
}
//---------------------------------------------------------------------------
bool __fastcall CNavTreeTest::NavTreeReadNavLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[9];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s ���ӬO "2,3,�s���j���þ������,Taisho.xml"
	return (s == u"1,2,�ĤG�ӥؿ�,nav2.xhtml");
}
//---------------------------------------------------------------------------
bool __fastcall CNavTreeTest::NavTreeReadCBLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[11];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s ���ӬO "2,3,CBETA�g��,T01n0001_001#p0001a01"
	return (s == u"1,3,�i�C�r��g,Book/T01n0003_001.xml");
}
//---------------------------------------------------------------------------

