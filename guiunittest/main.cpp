//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
	: TForm(Owner)
{
	// �x�s���G���r��C��
	Titles = new TStringList();
	Results = new TStringList();

	// ���ժ���
	Sample = new CSample(Titles, Results);
	ListBoxAddHead("���եܽd");
	Sample->RunAllTest();
	ListBoxAddHead("���եܽd2");
	Sample->RunAllTest();
}
//---------------------------------------------------------------------------
// �b���յ��G�W�[���D, �H�K�Ϥ�
void __fastcall TfmMain::ListBoxAddHead(String sHead)
{
	Titles->Add(sHead);
	Results->Add("H");
}
//---------------------------------------------------------------------------
// �C�X���G
void __fastcall TfmMain::btRunAllTestClick(TObject *Sender)
{
	for (int i = 0; i < Titles->Count; i++)
	{
		if(Results->Strings[i] == "H")
		{
			// �o�O�@�Ӽ��D
			TListBoxGroupHeader * lbHead = new TListBoxGroupHeader(lbResult);
			lbHead->Text = Titles->Strings[i];
			lbResult->AddObject(lbHead);
		}
		else
		{
			lbResult->Items->Add(Titles->Strings[i]);
			if(Results->Strings[i] == "1")
			{
				// ���ե��T�h����
				lbResult->ListItems[i]->IsChecked = true;
			}
			else
			{
				// ���ѫh�����
				lbResult->ListItems[i]->StyledSettings = lbResult->ListItems[i]->StyledSettings >> TStyledSetting::FontColor;
				lbResult->ListItems[i]->TextSettings->FontColor = 0xffff0000;
			}
		}
	}
}
//---------------------------------------------------------------------------
