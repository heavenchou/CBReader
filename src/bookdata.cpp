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
