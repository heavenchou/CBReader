// ---------------------------------------------------------------------------

#pragma hdrstop

#include "bookcase.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CBookcase::CBookcase() // �غc��
{
	BookcaseDir = ""; // ���d���ؿ�
	Books = new TList;
}
// ---------------------------------------------------------------------------
__fastcall CBookcase::~CBookcase(void)
{
	// �v�@�Ѻc Books
	for(int i=0; i < Books->Count; i++)
	{
		CSeries * s = (CSeries *) Books->Items[i];
		if(s) delete s;
    }
	if(Books) delete Books;
}
// ---------------------------------------------------------------------------
// Books ���ƶq
int __fastcall CBookcase::Count()
{
	return Books->Count;
}

// ---------------------------------------------------------------------------
// ���J�Ҧ������d
void __fastcall CBookcase::LoadBookcase(String sDir)
{
	// �ˬd���d�ؿ��O�_�s�b
	if(!TDirectory::Exists(sDir))
	{
		ShowMessage(u"���d�ؿ����s�b : " + sDir);
		return;
	}
	BookcaseDir = sDir; 	// ���d���ؿ�

	TStringDynArray list;
	list = TDirectory::GetDirectories(sDir, "*", TSearchOption::soTopDirectoryOnly);
	for(int i=0; i<list.Length; i++)
	{
		// ���իإ߮M�Ѫ���
		CSeries * s = new CSeries(list[i]);
		if(s->NavFile != "")
			Books->Add(s);   // �ܤ֭n��������� NavFile
		else
			delete s;
	}
}
// ---------------------------------------------------------------------------

