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
	TestString = "";

	// ���ժ���
	ListBoxAddHead(u"���եܽd");
	Sample = new CSample(Titles, Results);
	Sample->RunAllTest();

	ListBoxAddHead(u"NavTree ����");
	NavTreeTest = new CNavTreeTest(Titles, Results);
	NavTreeTest->RunAllTest();

	ListBoxAddHead(u"TreeView �𪬥ؿ�����");
	// ��𪬥ؿ��e�{, �åB���� click �����G
	NavTreeTest->NavTree->SaveToTreeView(TreeView, NavTreeItemClick);
	TreeViewRunAllTest();	// Nav �𪬥ؿ�����

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
void __fastcall TfmMain::btShowTestResultClick(TObject *Sender)
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
// Nav �𪬥ؿ� Item Click ���ե\��
void __fastcall TfmMain::NavTreeItemClick(TObject *Sender)
{
	TTreeViewItem * nItem = (TTreeViewItem *)Sender;

	TestString += nItem->Tag;
	TestString += ",";
	TestString += nItem->Text;
	TestString += ",";
	TestString += nItem->TagString;
}
//---------------------------------------------------------------------------
// Nav �𪬥ؿ�����
void __fastcall TfmMain::TreeViewRunAllTest(void)
{
	bool bResult = false;
	// Click �|�b TestString �Ǧ^���G
	NavTreeItemClick(TreeView->Items[0]->Items[0]->Items[0]);
	if(TestString == u"3,CBETA�g��,T01n0001_001#p0001a01") bResult = true;

	LogTest(u"Ū���𪬥ؿ����", bResult);
}
//---------------------------------------------------------------------------
// �x�s���յ��G
void __fastcall TfmMain::LogTest(String sTitle, bool bResult)
{
	String s = "0";
	if(bResult) s = "1";
	Titles->Add(sTitle);
	Results->Add(s);
}
//---------------------------------------------------------------------------


