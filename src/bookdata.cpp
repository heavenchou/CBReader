//---------------------------------------------------------------------------

#pragma hdrstop

#include "bookdata.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// �غc��
__fastcall CBookData::CBookData()
{
	ID = new TStringList();         // �øg�N�X
	VolCount = new TStringList();       // �U�ƪ����
	BookName = new TStringList();      // ����W��
	BookEngName = new TStringList();        // �^��W��

}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CBookData::~CBookData()
{
	if(ID) delete ID;
	if(VolCount) delete VolCount;
	if(BookName) delete BookName;
	if(BookEngName) delete BookEngName;

}
// ---------------------------------------------------------------------------
// ���J�ؿ����
void __fastcall CBookData::LoadBookDataFile(String sFile)
{
	TStringList * slBookData = new TStringList();

	if(!TFile::Exists(sFile))
	{
		ShowMessage(u"BookData ��󤣦s�b : " + sFile);
		return;
	}

	slBookData->LoadFromFile(sFile);

	for(int i=0; i<slBookData->Count; i++)
	{
		TStringDynArray slItems = TRegEx::Split(slBookData->Strings[i], "\\s*,\\s*");

		if(slItems.get_length() == 4)
		{
			ID->Add(slItems[0]);
			VolCount->Add(slItems[1]);
			BookName->Add(slItems[2]);
			BookEngName->Add(slItems[3]);
		}
    }
}
// ---------------------------------------------------------------------------
// �ǤJ T, 1 , �Ǧ^ "T01" �o�ؼзǪ��U��
String __fastcall CBookData::GetFullVolString(String sBook, String sVol)
{
	sVol = GetNormalVolNumString(sBook, sVol);
	sVol = sBook + sVol;
	return sVol;
}
// ---------------------------------------------------------------------------
// ���o���w�øg���U�Ʀ��, �Ҧp�j���ìO 2 ���
int __fastcall CBookData::GetVolLen(String sBook)
{
	for(int i=0; i<ID->Count; i++)
	{
		if(ID->Strings[i] == sBook)
		{
			return VolCount->Strings[i].ToInt();
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
// �ǤJ T, 1 , �Ǧ^ "01" �o�ؼзǪ��U��
String __fastcall CBookData::GetNormalVolNumString(String sBook, String sVol)
{
	int iVol = sVol.ToIntDef(0);
	if(iVol == 0) return "";
	int iVolLen = GetVolLen(sBook); // ���o���w�øg���U�Ʀ��, �Ҧp�j���ìO 2 ���
	int iMyLen = sVol.Length();

	if(iMyLen < iVolLen)
	{
		sVol = String().StringOfChar(L'0',iVolLen-iMyLen) + sVol;
	}
	else if(iMyLen > iVolLen)
	{
		String::iterator it = sVol.end();
        sVol = String(it-iVolLen,iVolLen);
    }

	return sVol;
}
// ---------------------------------------------------------------------------

