//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "searchrange.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmSearchRange *fmSearchRange;
//---------------------------------------------------------------------------
__fastcall TfmSearchRange::TfmSearchRange(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmSearchRange::ListBoxItem1Click(TObject *Sender)
{
	TabControl1->TabIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmSearchRange::ListBoxItem2Click(TObject *Sender)
{
    TabControl1->TabIndex = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfmSearchRange::btOKClick(TObject *Sender)
{
	if(lbSearchRangeList->ItemIndex == 0)
	{
		// �B�z����
		BuleiSelect();
	}
	else
	{
		// �B�z���
		BookSelect();
    }
}
//---------------------------------------------------------------------------
// �B�z����
void __fastcall TfmSearchRange::BuleiSelect()
{
	BuildFileList->NoneSearch();
	// �v�@�j�M��
	for(int i=0; i<tvBulei->Count; i++)
	{
		if(tvBulei->Items[i]->IsChecked)
		{
			String sBuleiName = tvBulei->Items[i]->Text;
			CSeries * Series = fmMain->Bookcase->CBETA;
			CCatalog * Catalog = Series->Catalog;

			// ����X�ӳ������g
			for(int j=0; j<Catalog->Bulei->Count; j++)
			{
				if(Catalog->Bulei->Strings[j] == sBuleiName)
				{
					// ���g�n�˯�
                    BuildFileList->SearchThisSutra(Catalog->ID->Strings[j],Catalog->SutraNum->Strings[j]);
                }
			}
		}
	}
}
//---------------------------------------------------------------------------
// �B�z���
void __fastcall TfmSearchRange::BookSelect()
{
    BuildFileList->NoneSearch();
	// �v�@�j�M��
	for(int i=0; i<tvBook->Count; i++)
	{
		if(tvBook->Items[i]->IsChecked)
		{
			String sName = tvBook->Items[i]->Text;
			// T �j����
			// ���X�e�����N�X
			int iPos = sName.Pos0(" ");
			String sBookId = sName.SubString0(0,iPos);
			BuildFileList->SearchThisBoook(sBookId);
		}

	}
}
//---------------------------------------------------------------------------

