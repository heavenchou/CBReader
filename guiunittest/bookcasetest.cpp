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
// �����������
void __fastcall CBookcaseTest::RunAllTest()
{
	LogTest(u"��� Agama �ؿ�", BookcaseFindDir());
	LogTest(u"���լO�_��� 2 ����", BookcaseCount());
	LogTest(u"����Ū���Ĥ@���� Title", ReadSeriesTitle());
	LogTest(u"����Ū���ĤG���� Nav ��m", ReadSeriesNav());
}
//---------------------------------------------------------------------------
// �x�s���յ��G
void __fastcall CBookcaseTest::LogTest(String sTitle, bool bResult)
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
	return (s == u"���t�g");
}

//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ReadSeriesNav()
{
	Series = (CSeries *) Bookcase->Books->Items[1];
	String s = Series->NavFile;
	return (s == u"ys_nav.xhtml");
}

//---------------------------------------------------------------------------

