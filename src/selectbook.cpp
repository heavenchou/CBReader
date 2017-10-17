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
	SelectedBook = -1;   // 目前選中的書代碼, -1 表示還沒有選

	// 初始書櫃
	Initial(fmMain->Bookcase);
}
//---------------------------------------------------------------------------
// 初始書櫃
void __fastcall TfmSelectBook::Initial(CBookcase * Bookcase)
{
	for(int i=0; i<Bookcase->Books->Count; i++)
	{
		CSeries * Series = (CSeries * )Bookcase->Books->Items[i];
		String sTitle = Series->Title;
		String sCreator = Series->Creator;
		if(sCreator != "") sTitle = sTitle + u"　【" + sCreator + u"】";

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
// 選中某一本書
void __fastcall TfmSelectBook::ListBoxItem1Click(TObject *Sender)
{
	TListBoxItem * Item = (TListBoxItem *) Sender;
	SelectedBook = Item->Tag;
    Close();
}
//---------------------------------------------------------------------------


