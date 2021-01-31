//---------------------------------------------------------------------------

#pragma hdrstop

#include "bookdata.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// �غc��
__fastcall CBookData::CBookData()
{
	ID = new TStringList();            // �øg�N�X
	VolCount = new TStringList();      // �U�ƪ����
	VerName = new TStringList();       // �����W��, �Ҧp�j���ìO�i�j�j
	BookName = new TStringList();      // ����W��
	BookEngName = new TStringList();   // �^��W��
}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CBookData::~CBookData()
{
	if(ID) delete ID;
	if(VolCount) delete VolCount;
	if(VerName) delete VerName;
	if(BookName) delete BookName;
	if(BookEngName) delete BookEngName;

	ID = 0;            // �øg�N�X
	VolCount = 0;      // �U�ƪ����
	VerName = 0;       // �����W��, �Ҧp�j���ìO�i�j�j
	BookName = 0;      // ����W��
	BookEngName = 0;   // �^��W��

}
// ---------------------------------------------------------------------------
// ���J�ؿ����
void __fastcall CBookData::LoadBookDataFile(String sFile)
{
	TStringList * slBookData = new TStringList();

	if(!TFile::Exists(sFile))
	{
		TDialogService::ShowMessage(u"BookData ��󤣦s�b : " + sFile);
		return;
	}

	slBookData->LoadFromFile(sFile);

	for(int i=0; i<slBookData->Count; i++)
	{
		TStringDynArray slItems = TRegEx::Split(slBookData->Strings[i], u"\\s*,\\s*");

		if(slItems.get_length() == 5)
		{
			ID->Add(slItems[0]);
			VolCount->Add(slItems[1]);
			VerName->Add(slItems[2]);
			BookName->Add(slItems[3]);
			BookEngName->Add(slItems[4]);
		}
    }
}
// ---------------------------------------------------------------------------
// �ǤJ T, �Ǧ^��b TStringList �� Index , �S���h�Ǧ^ -1
int __fastcall CBookData::GetBookIndex(String sBook)
{
	for(int i=0; i<ID->Count; i++)
	{
		if(ID->Strings[i] == sBook)
			return i;
	}
	return -1;
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
	int i = GetBookIndex(sBook);
	if(i >= 0)
		return VolCount->Strings[i].ToInt();
	else
		return 0; // �䤣��Ǧ^ 0
}
// ---------------------------------------------------------------------------
// �ǤJ T, 1 , �Ǧ^ "01" �o�ؼзǪ��U��
String __fastcall CBookData::GetNormalVolNumString(String sBook, String sVol)
{
	int iVol = sVol.ToIntDef(0);
	//if(iVol == 0) return u"";
	int iVolLen = GetVolLen(sBook); // ���o���w�øg���U�Ʀ��, �Ҧp�j���ìO 2 ���
	int iMyLen = sVol.Length();

	if(iMyLen < iVolLen)
	{
		sVol = String().StringOfChar(u'0',iVolLen-iMyLen) + sVol;
	}
	else if(iMyLen > iVolLen)
	{
		sVol.Delete0(0,iVolLen-iMyLen);
    }

	return sVol;
}
// ---------------------------------------------------------------------------
// �ǤJ T, �Ǧ^ "�i�j�j" �����W�� , �䤣��Ǧ^�Ŧr��
String __fastcall CBookData::GetVerName(String sBook)
{
	int i = GetBookIndex(sBook);
	if(i >= 0)
		return VerName->Strings[i];
	else
		return u""; // �䤣��Ǧ^�Ŧr��
}
// ---------------------------------------------------------------------------
// �ǤJ T, �Ǧ^ "�j���s��j�øg" �����W�� , �䤣��Ǧ^�Ŧr��
String __fastcall CBookData::GetBookName(String sBook)
{
	int i = GetBookIndex(sBook);
	if(i >= 0)
		return BookName->Strings[i];
	else
		return u""; // �䤣��Ǧ^�Ŧr��
}
// ---------------------------------------------------------------------------
// �� bookdata ���X map , �ǤJ "T" , �Ǧ^ "�i�j�j"
// �o�O�n�ǵ������˯��Ϊ�
void __fastcall CBookData::BookToVerNameMap(map<String, String> & mpBookToVerName)
{
	for(int i=0; i<ID->Count; i++)
	{
		mpBookToVerName[ID->Strings[i]] = VerName->Strings[i];
    }
}
// ---------------------------------------------------------------------------
