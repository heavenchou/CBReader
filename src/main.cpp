// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
#include "selectbook.h"

#ifdef _Windows
#include <System.Win.Registry.hpp>
#endif

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

TfmMain *fmMain;

// ---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner) : TForm(Owner)
{
	// ���o�]�w�ɨ�Ū���Ҧ��]�w

	// ���o Bookcase �Ҧ���ư�

#ifdef _Windows
	//SetPermissions(); // �N IE �]�w�� IE 11 (�p�G�S IE 11 ���p��?)
#endif

#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012";
	//MyFullPath = GetCurrentDir();
#endif

	MyFullPath += "/";
	SettingFile = "cbreader.ini";
	Setting = new CSetting();

	SelectedBook = -1;   // �ثe�襤����, -1 ����٨S��

	// ���J���d

	Bookcase = new CBookcase();
	Bookcase->LoadBookcase(MyFullPath + Setting->BookcaseDir);

	// �b���d����O��
	int iBookcaseCount = Bookcase->Count();
	if(iBookcaseCount == 0)
	{
		ShowMessage(u"���d���@���ѳ��S��");
    }
	else if(iBookcaseCount == 1)
	{
		// �u���@���ѴN�����}�F
		OpenBookcase(0);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfmMain::FormDestroy(TObject *Sender)
{
	if(Setting) delete Setting;
	if(Bookcase) delete Bookcase;
	if(NavTree) delete NavTree;
}

// ---------------------------------------------------------------------------
// �N IE �]�w�� IE 11
// copy from
// http://docwiki.embarcadero.com/Libraries/Tokyo/en/FMX.WebBrowser.TWebBrowser
//
// �Ѧ� MSDN
// https://msdn.microsoft.com/en-us/library/ee330730%28v=vs.85%29.aspx#browser_emulation
void __fastcall TfmMain::SetPermissions()
{
#ifdef _Windows
	UnicodeString cHomePath = "SOFTWARE";
	UnicodeString cFeatureBrowserEmulation = "Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION\\";
	int cIE11 = 11001;
	UnicodeString sKey = ExtractFileName(ParamStr(0));
	TRegIniFile *Reg = new TRegIniFile(cHomePath);
	__try
	{
		TRegistry *reg1 = dynamic_cast<TRegistry*>(Reg);
		if (Reg->OpenKey(cFeatureBrowserEmulation,
			true) && !(reg1->KeyExists(sKey) && reg1->ReadInteger(sKey)
			== cIE11))
		{
			reg1->WriteInteger(sKey, cIE11);
		}
	}
	__finally
	{
		Reg->Free();
	}
#endif
}
//---------------------------------------------------------------------------
// �}�ҫ��w�����d
void __fastcall TfmMain::OpenBookcase(int iID)
{
	if(iID == SelectedBook) return;	// �P�@��, ���n���}
	if(iID == -1) return;   	// �S���

	SelectedBook = iID;

	// ���J�O�Ѫ��_�l�ؿ�
	if(NavTree) delete NavTree;
	CSeries * s = (CSeries *) Bookcase->Books->Items[iID];
	NavTree = new CNavTree(s->Dir + s->NavFile);
	NavTree->SaveToTreeView(tvNavTree, NavTreeItemClick);
}

//---------------------------------------------------------------------------
// NavTree Item �I�G�U���@��
// Item->TagString �x�s URL
// Item->Tag �x�s Type
// Item->TagObject �x�s SNavItem
void __fastcall TfmMain::NavTreeItemClick(TObject *Sender)
{
	// Item
	TTreeViewItem * tvItem = (TTreeViewItem *) Sender;
	String sURL = tvItem->TagString;
	CSeries * sSeries = (CSeries *)Bookcase->Books->Items[SelectedBook];

	if(sURL == "")  // �S�� URL
	{
		// �p�G���l�h, �N�����i�}�γ��X���A
		if(tvItem->ChildrenCount > 0)
		{
			if(tvItem->IsExpanded)
				tvItem->Collapse();
			else
				tvItem->Expand();
        }
		return;
	}

	int iType = tvItem->Tag;

	// �@��s��
	if(iType == nit_NormalLink)
	{
		if(sURL.SubString(1,4) == "http")
			WebBrowser->URL = sURL;
		else
			WebBrowser->URL = "file://" + sSeries->Dir + sURL;
		WebBrowser->Navigate();
	}
	// �ؿ��s��
	else if(iType == nit_NavLink)
	{
		if(NavTree) delete NavTree;     // �o�������Ӫ���� ???
		NavTree = new CNavTree(sSeries->Dir + sURL);
		NavTree->SaveToTreeView(tvNavTree, NavTreeItemClick);
	}
	// CBETA �g��
	else if(iType == nit_CBLink)
	{
		ShowCBXML(sURL);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::CornerButton1Click(TObject *Sender)
{
	fmSelectBook->ShowModal();
	OpenBookcase(fmSelectBook->SelectedBook);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btSetBookcasePathClick(TObject *Sender)
{
    MyFullPath = edBookcasePath->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::CheckBox1Change(TObject *Sender)
{
	Setting->CBXMLOption->ShowLineFormat = CheckBox1->IsChecked;
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::CornerButton2Click(TObject *Sender)
{
	WebBrowser->GoBack();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::CornerButton3Click(TObject *Sender)
{
	WebBrowser->GoForward();
}
//---------------------------------------------------------------------------
// �O�_����ܮM�ѤF?
bool __fastcall TfmMain::IsSelectedBook()
{
	if(SelectedBook == -1) return false;
	else return true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btFindSutraClick(TObject *Sender)
{
	String sSutraName = edFindSutraName->Text;
	String sByline = edFindSutraByline->Text;

	// �v�@�j�M�ؿ�
	if(IsSelectedBook())
	{
		CSeries * Series = (CSeries *) Bookcase->Books->Items[SelectedBook];
		CCatalog * Catalog = Series->Catalog;
		int iCount = Catalog->ID->Count;
		// �v�@�ˬd
		sgFindSutra->BeginUpdate();
		int iGridIndex = 0;
		sgFindSutra->RowCount = 10;
		for(int i=0; i<iCount; i++)
		{
			bool bFound = true;

			// ��g�W
			if(!sSutraName.IsEmpty())
				if(Catalog->SutraName->Strings[i].Pos(sSutraName) <= 0)
					continue;
			// ��Ķ��
			if(!sByline.IsEmpty())
				if(Catalog->Byline->Strings[i].Pos(sByline) <= 0)
					continue;

			// ���F

			sgFindSutra->Cells[0][iGridIndex]=Catalog->SutraName->Strings[i];
			sgFindSutra->Cells[1][iGridIndex]=Catalog->Byline->Strings[i];
			iGridIndex++;

			if(iGridIndex >= sgFindSutra->RowCount)
				sgFindSutra->RowCount += 10;
		}
		sgFindSutra->RowCount = iGridIndex;
        sgFindSutra->EndUpdate();
	}
}
//---------------------------------------------------------------------------
// �Ѹg�������e�{�g��
void __fastcall TfmMain::btGoSutraClick(TObject *Sender)
{
	String sBook = "T";
	String sSutraNum = edGoSutra_SutraNum->Text;
	String sJuan = edGoSutra_Juan->Text;
	String sPage = edGoSutra_Page->Text;
	String sField = edGoSutra_Field->Text;
	String sLine = edGoSutra_Line->Text;

	CSeries * csCBETA = Bookcase->CBETA;

	String sFile = csCBETA->CBGetFileNameBySutraNumJuan(sBook, sSutraNum, sJuan);
	ShowCBXML(sFile);

}
//---------------------------------------------------------------------------
// ���J XML �óB�z������
void __fastcall TfmMain::ShowCBXML(String sFile)
{
	if(sFile == "")
	{
		ShowMessage("�S����쥿�T�ɮ�");
        return;
    }
	sFile = Bookcase->CBETA->Dir + sFile;
	CCBXML * CBXML = new CCBXML(sFile, Setting->CBXMLOption);

	// ������, �]�� mac os ���ͥX�Ӫ��ɦW�O /var/tmp/xxxxx
	// windows �O xxxxxx
	// �ҥH����٬O�ۤv�g�a
	//char cOutFile[14];
	//std::tmpnam(cOutFile);

	String sOutFile = sFile + ".htm";   // ???? ��X���ɦW�Ȯɴ�X��
	CBXML->SaveToHTML(sOutFile);

	WebBrowser->URL = "file://" + sOutFile;
	WebBrowser->Navigate();
}
//---------------------------------------------------------------------------
// �ѥU�����e�{�g��
void __fastcall TfmMain::btGoBookClick(TObject *Sender)
{
	String sBook = "T";
	String sVol = edGoBook_Vol->Text;
	String sPage = edGoBook_Page->Text;
	String sField = edGoBook_Field->Text;
	String sLine = edGoBook_Line->Text;

	CSeries * csCBETA = Bookcase->CBETA;

	// �ǤJ T, 1 , �Ǧ^ "T01" �o�ؼзǪ��U��
	sVol = csCBETA->BookData->GetFullVolString(sBook, sVol);

	String sFile = csCBETA->CBGetFileNameByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	ShowCBXML(sFile);
}
//---------------------------------------------------------------------------

