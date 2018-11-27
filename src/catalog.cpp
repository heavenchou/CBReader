// ---------------------------------------------------------------------------
#pragma hdrstop
#include "catalog.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// �غc��
__fastcall CCatalog::CCatalog()
{
	ID = new TStringList();         // �øg�N�X
	Bulei = new TStringList();      // ����
	Part = new TStringList();       // ���O
	Vol = new TStringList();        // �U��
	SutraNum = new TStringList();   // �g��
	JuanNum = new TStringList();    // ����
	SutraName = new TStringList();  // �g�W
	Byline = new TStringList();     // �@Ķ��

	Field.push_back(ID);
	Field.push_back(Bulei);
	Field.push_back(Part);
	Field.push_back(Vol);
	Field.push_back(SutraNum);
	Field.push_back(JuanNum);
	Field.push_back(SutraName);
	Field.push_back(Byline);
}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CCatalog::~CCatalog()
{
	if(ID) delete ID;
	if(Bulei) delete Bulei;
	if(Part) delete Part;
	if(Vol) delete Vol;
	if(SutraNum) delete SutraNum;
	if(JuanNum) delete JuanNum;
	if(SutraName) delete SutraName;
	if(Byline) delete Byline;

}
// ---------------------------------------------------------------------------
// ���J�ؿ����
void __fastcall CCatalog::LoadCatalog(String sFile)
{
	TStringList * slCatalog = new TStringList();

	if(!TFile::Exists(sFile))
	{
		TDialogService::ShowMessage(u"�ؿ���󤣦s�b : " + sFile);
		return;
	}

	slCatalog->LoadFromFile(sFile,TEncoding::UTF8);

	for(int i=0; i<slCatalog->Count; i++)
	{
		TStringDynArray slItems = TRegEx::Split(slCatalog->Strings[i], "\\s*,\\s*");

		if(slItems.get_length() == 8)
		{
			for(int i =0; i<8; i++)
			{
				Field[i]->Add(slItems[i]);
            }
		}
    }
}
// ---------------------------------------------------------------------------
// ���o Catalog ���s��, �Ǧ^ -1 ��ܧ䤣��
/*
int __fastcall CCatalog::FindIndexBySutraNum(String sBook, String sSutraNum)
{
	for(int i=0; i<ID->Count; i++)
	{
		if(ID->Strings[i] == sBook && SutraNum->Strings[i] == sSutraNum)
			return i;
	}
	return -1;
}
*/
// ---------------------------------------------------------------------------
// ���o Catalog ���s��, �Ǧ^ -1 ��ܧ䤣��
int __fastcall CCatalog::FindIndexBySutraNum(String sBook, String sVol, String sSutraNum)
{
	return FindIndexBySutraNum(sBook, sVol.ToInt(), sSutraNum);
}
// ---------------------------------------------------------------------------
// ���o Catalog ���s��, �Ǧ^ -1 ��ܧ䤣��
int __fastcall CCatalog::FindIndexBySutraNum(String sBook, int iVol, String sSutraNum)
{
	for(int i=0; i<ID->Count; i++)
	{
		if(ID->Strings[i] == sBook && SutraNum->Strings[i] == sSutraNum)
			if(Vol->Strings[i].ToInt() == iVol && iVol != 0)
				return i;
	}
	return -1;
}
// ---------------------------------------------------------------------------

