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
	LogTest(u"��� CBETA �ؿ�", BookcaseFindDir());
	LogTest(u"���լO�_��� 2 ����", BookcaseCount());
	LogTest(u"����Ū���Ĥ@���� Title", ReadSeriesTitle());
	LogTest(u"����Ū���ĤG���� Nav ��m", ReadSeriesNav());
	LogTest(u"���o�зǥU�� T,1", ChkNormalVol("T","1","T01"));
	LogTest(u"���o�зǥU�� T,03", ChkNormalVol("T","03","T03"));
	LogTest(u"���o�зǥU�� C,5", ChkNormalVol("C","5","C005"));
	LogTest(u"���o�зǥU�� C,00003", ChkNormalVol("C","00003","C003"));
	LogTest(u"���o�зǸg�� 1", ChkNormalSutra("1","0001"));
	LogTest(u"���o�зǸg�� 55b", ChkNormalSutra("55b","0055b"));
	LogTest(u"���o�зǸg�� 01234", ChkNormalSutra("01234","1234"));
	LogTest(u"���o�зǸg�� 0055", ChkNormalSutra("0055","0055"));
	LogTest(u"���o�зǸg�� 0056A", ChkNormalSutra("0056A","0056A"));
	LogTest(u"���o�зǸg�� 002345b", ChkNormalSutra("002345b","2345b"));
	LogTest(u"���o�зǭ��X 32", ChkNormalPage("32","0032"));
	LogTest(u"���o�зǭ��X b30", ChkNormalPage("b30","b030"));
	LogTest(u"���o�зǭ��X 0456", ChkNormalPage("0456","0456"));
	LogTest(u"���o�зǭ��X 00123", ChkNormalPage("00123","0123"));
	LogTest(u"���o�зǭ��X c00045", ChkNormalPage("c00045","c045"));
	LogTest(u"���o�зǦ� 5", ChkNormalLine("5","05"));
	LogTest(u"���o�зǦ� 12", ChkNormalLine("12","12"));
	LogTest(u"���o�зǦ� 0032", ChkNormalLine("0032","32"));
	LogTest(u"���եѸg���M���ɮ� T,3", FindXMLBySutraJuan(u"T",u"3","","XML/T/T01/T01n0003_001.xml"));
	LogTest(u"���եѸg���M���ɮ� T,99,5", FindXMLBySutraJuan(u"T",u"99","5","XML/T/T02/T02n0099_005.xml"));
	LogTest(u"���եѸg���M���ɮ� T,128", FindXMLBySutraJuan(u"T",u"128","","XML/T/T02/T02n0128a_001.xml"));
	LogTest(u"���եѸg���M���ɮ� T,128b,1", FindXMLBySutraJuan(u"T",u"128b","1","XML/T/T02/T02n0128b_001.xml"));
	LogTest(u"���եѸg���M���ɮ� T,150,1", FindXMLBySutraJuan(u"T",u"150","1","XML/T/T02/T02n0150A_001.xml"));
	LogTest(u"���եѸg���M���ɮ� T,150b,1", FindXMLBySutraJuan(u"T",u"150b","1","XML/T/T02/T02n0150B_001.xml"));
	LogTest(u"���եѥU�����M���ɮ� T,1", FindXMLByVolPageFieldLine(u"T",u"1","","","","XML/T/T01/T01n0001_001.xml"));
	LogTest(u"���եѥU�����M���ɮ� T,1,156,c,8", FindXMLByVolPageFieldLine(u"T",u"1","156","c","8","XML/T/T01/T01n0003_001.xml"));
	LogTest(u"���եѥU�����M���ɮ� T,1,156,3,9", FindXMLByVolPageFieldLine(u"T",u"1","156","3","9","XML/T/T01/T01n0003_002.xml"));
	LogTest(u"���եѥU�����M���ɮ� T,002,00837,0c,007", FindXMLByVolPageFieldLine(u"T",u"002","00837","0c","007","XML/T/T02/T02n0128a_001.xml"));
	LogTest(u"���եѥU�����M���ɮ� T,2,883,,9", FindXMLByVolPageFieldLine(u"T",u"2","883","","9","XML/T/T02/T02n0150B_001.xml"));
	LogTest(u"���եѥU�����M���ɮ� T,2,999,,99", FindXMLByVolPageFieldLine(u"T",u"2","999","","99","XML/T/T02/T02n0151_001.xml"));
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

	return (s.Pos("CBETA")>0);
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
	return (s == u"CBETA�~��q�l��嶰��");
}

//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ReadSeriesNav()
{
	Series = (CSeries *) Bookcase->Books->Items[1];
	String s = Series->NavFile;
	return (s == u"nav.xhtml");
}
//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::FindXMLBySutraJuan(String sBookID, String sSutra, String sJuan, String sFile)
{
	Series = Bookcase->CBETA;
	String s = Series->CBGetFileNameBySutraNumJuan(sBookID, sSutra, sJuan);
	return (s == sFile);
}
//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::FindXMLByVolPageFieldLine(String sBookID, String sVol, String sPage, String sField, String sLine, String sFile)
{
	Series = Bookcase->CBETA;
	String s = Series->CBGetFileNameByVolPageFieldLine(sBookID, sVol, sPage, sField, sLine);
	return (s == sFile);
}
//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ChkNormalVol(String sBookID, String sVol, String sVolResult)
{
	Series = Bookcase->CBETA;
	String s = Series->BookData->GetFullVolString(sBookID, sVol);
	return (s == sVolResult);
}
//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ChkNormalSutra(String sSutra, String sResult)
{
	Series = Bookcase->CBETA;
	String s = Series->Spine->CBGetSutraNumFormat(sSutra);
	return (s == sResult);
}
//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ChkNormalPage(String sPage, String sResult)
{
	Series = Bookcase->CBETA;
	String s = Series->JuanLine->GetNormalPage(sPage);
	return (s == sResult);
}
//---------------------------------------------------------------------------
bool __fastcall CBookcaseTest::ChkNormalLine(String sLine, String sResult)
{
	Series = Bookcase->CBETA;
	String s = Series->JuanLine->GetNormalLine(sLine);
	return (s == sResult);
}
//---------------------------------------------------------------------------
