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
	tcSelect->TabIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmSearchRange::ListBoxItem2Click(TObject *Sender)
{
    tcSelect->TabIndex = 1;
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
	//fmMain->SearchEngine->BuildFileList->NoneSearch();
	fmMain->Bookcase->CBETA->SearchEngine_CB->BuildFileList->NoneSearch();
	fmMain->Bookcase->CBETA->SearchEngine_orig->BuildFileList->NoneSearch();
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
				if(j==4620)
				{
					int a=10;
                    int b=a+1;
                }
				if(Catalog->Bulei->Strings[j] == sBuleiName)
				{
					// ���g�n�˯�
					//fmMain->SearchEngine->BuildFileList->SearchThisSutra(Catalog->ID->Strings[j],Catalog->SutraNum->Strings[j]);
					fmMain->Bookcase->CBETA->SearchEngine_CB->BuildFileList->SearchThisSutra(Catalog->ID->Strings[j],Catalog->SutraNum->Strings[j]);
					fmMain->Bookcase->CBETA->SearchEngine_orig->BuildFileList->SearchThisSutra(Catalog->ID->Strings[j],Catalog->SutraNum->Strings[j]);
                }
			}
		}
	}
}
//---------------------------------------------------------------------------
// �B�z���
void __fastcall TfmSearchRange::BookSelect()
{
	//fmMain->SearchEngine->BuildFileList->NoneSearch();
	fmMain->Bookcase->CBETA->SearchEngine_CB->BuildFileList->NoneSearch();
	fmMain->Bookcase->CBETA->SearchEngine_orig->BuildFileList->NoneSearch();
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
			//fmMain->SearchEngine->BuildFileList->SearchThisBoook(sBookId);
			fmMain->Bookcase->CBETA->SearchEngine_CB->BuildFileList->SearchThisBoook(sBookId);
			fmMain->Bookcase->CBETA->SearchEngine_orig->BuildFileList->SearchThisBoook(sBookId);
		}

	}
}
//---------------------------------------------------------------------------
// �������
void __fastcall TfmSearchRange::btCheckAllClick(TObject *Sender)
{
	if(lbSearchRangeList->ItemIndex == 0)
	{
		// �B�z����
		TreeViewCheckAll(tvBulei, true);
	}
	else
	{
		// �B�z���
		TreeViewCheckAll(tvBook, true);
	}
}
//---------------------------------------------------------------------------
// ���������
void __fastcall TfmSearchRange::btUnCheckAllClick(TObject *Sender)
{
 	if(lbSearchRangeList->ItemIndex == 0)
	{
		// �B�z����
		TreeViewCheckAll(tvBulei, false);
	}
	else
	{
		// �B�z���
		TreeViewCheckAll(tvBook, false);
	}
}
//---------------------------------------------------------------------------
// �Y Treeview ����
void __fastcall TfmSearchRange::TreeViewCheckAll(TTreeView * tvTreeView, bool bChecked)
{
	for(int i=0; i<tvTreeView->Count; i++)
	{
		tvTreeView->Items[i]->IsChecked = bChecked;
	}
}
//---------------------------------------------------------------------------


void __fastcall TfmSearchRange::FormShow(TObject *Sender)
{
	tcSelect->TabIndex = 0;
    lbSearchRangeList->ItemIndex = 0;
}
//---------------------------------------------------------------------------

