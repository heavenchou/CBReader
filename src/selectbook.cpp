//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "selectbook.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmSelectBook *fmSelectBook;
//---------------------------------------------------------------------------
__fastcall TfmSelectBook::TfmSelectBook(TComponent* Owner)
	: TForm(Owner)
{
	SelectedBook = -1;   // �ثe�襤���ѥN�X, -1 ����٨S����

	// ��l���d
	Initial(fmMain->Bookcase);
}
//---------------------------------------------------------------------------
// ��l���d
void __fastcall TfmSelectBook::Initial(CBookcase * Bookcase)
{
	for(int i=0; i<Bookcase->Books->Count; i++)
	{
		CSeries * Series = (CSeries * )Bookcase->Books->Items[i];
		String sTitle = Series->Title;
		String sCreator = Series->Creator;
		if(sCreator != "") sTitle = sTitle + u"�@�i" + sCreator + u"�j";

		TListBoxItem * Item = new TListBoxItem(NULL);
		lbBookcase->AddObject((TFmxObject *)Item);
		Item->StyleLookup = "CustomItem";
		Item->Text = sTitle;
		Item->Font->Size = 20;
		Item->Height = 30;
		Item->Tag = i;
		Item->StyledSettings = Item->StyledSettings >> TStyledSetting::Size;
		Item->OnDblClick = ListBoxItem1Click;
	}
}
//---------------------------------------------------------------------------
// �襤�Y�@����
void __fastcall TfmSelectBook::ListBoxItem1Click(TObject *Sender)
{
	TListBoxItem * Item = (TListBoxItem *) Sender;
	SelectedBook = Item->Tag;
    Close();
}
//---------------------------------------------------------------------------


