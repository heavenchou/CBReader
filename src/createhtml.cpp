//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "createhtml.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmCreateHtml *fmCreateHtml;
//---------------------------------------------------------------------------
__fastcall TfmCreateHtml::TfmCreateHtml(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmCreateHtml::btStartClick(TObject *Sender)
{
	Stop = false;
	// 如果找到第一筆, 而且有勾選 "繼續" , 就持續下去
	bool bGoing = false;

	String sTempPath = fmMain->MyTempPath + u"Debug\\";
	if(!TDirectory::Exists(sTempPath))
	{
		TDirectory::CreateDirectoryW(sTempPath);
	}
	edTempPath->Text = sTempPath;

	int iSpineCount = fmMain->Bookcase->CBETA->Spine->Files->Count;
	for(int i=0; i<iSpineCount; i++)
	{
		// 該忽略還是要忽略才對
		if(edNoBook->Text != u"")
		{
			if(edNoBook->Text == fmMain->Bookcase->CBETA->Spine->BookID->Strings[i])
				continue;
		}

		if(!bGoing) // if true , 找到第一筆就不用比對, 一直做下去
		{
			if(edBook->Text != u"")
			{
				if(edBook->Text != fmMain->Bookcase->CBETA->Spine->BookID->Strings[i])
					continue;
			}
			if(edVolNum->Text != u"")
			{
				if(edVolNum->Text != fmMain->Bookcase->CBETA->Spine->VolNum->Strings[i].ToInt())
					continue;
			}
		}
		String sFile = fmMain->Bookcase->CBETA->Spine->Files->Strings[i];

		CreateThisFile(sFile);
		lbFileName->Text = sFile;

		if(cbContinue->IsChecked) bGoing = true; // 找到第一筆就繼續

		Application->ProcessMessages();
		if(Stop) return;
	}

    TDialogService::ShowMessage("OK");
}
//---------------------------------------------------------------------------

void __fastcall TfmCreateHtml::CreateThisFile(String sFile)
{
	if(sFile == "")
	{
		TDialogService::ShowMessage(u"沒有找到正確檔案");
        return;
	}

	String sXMLFile = fmMain->Bookcase->CBETA->Dir + sFile;
	String sJSFile = fmMain->Bookcase->CBETA->Dir + fmMain->Bookcase->CBETA->JSFile;
	//CCBXML * CBXML = new CCBXML(sXMLFile, sLink, Setting, sJSFile, bShowHighlight, seSearchEngine);
	CCBXML * CBXML = new CCBXML(sXMLFile, "", fmMain->Setting, sJSFile, false, 0);

	String sOutFile = sFile + u".htm";

	sOutFile = StringReplace(sOutFile, "/", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = StringReplace(sOutFile, "\\", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = edTempPath->Text + sOutFile;

	CBXML->SaveToHTML(sOutFile);

    delete CBXML;

}
//---------------------------------------------------------------------------
void __fastcall TfmCreateHtml::btStopClick(TObject *Sender)
{
	Stop = true;
}
//---------------------------------------------------------------------------
