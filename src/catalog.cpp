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
		ShowMessage(u"�䤣�� " + sFile);
		return;
	}

	slCatalog->LoadFromFile(sFile);

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

