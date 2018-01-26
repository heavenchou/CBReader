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
	SearchSentence = "";    // �j�M�r��
	SearchWordList = new TStringList;	    // �s��C�@���˯�����, �����|�Ψ�

	// ���o�]�w�ɨ�Ū���Ҧ��]�w

	// ���o Bookcase �Ҧ���ư�

#ifdef _Windows
	SetPermissions(11001); // �N IE �]�w�� IE 11 (�p�G�S IE 11 ���p��?)
#endif

#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	// MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012";
	// MyFullPath = GetCurrentDir();
	//MyFullPath = StringReplace(MyFullPath, "/CBReader.app/Contents/MacOS", "", TReplaceFlags() << rfReplaceAll);

	MyFullPath = System::Ioutils::TPath::GetHomePath();
	MyFullPath += "/Desktop";

#endif

	MyFullPath += "/";
	MyTempPath = MyFullPath + "_Temp_/";
	if(!TDirectory::Exists(MyTempPath))
	{
		TDirectory::CreateDirectory(MyTempPath);
	}

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
	if(SearchWordList) delete SearchWordList;
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
void __fastcall TfmMain::SetPermissions(int iIE)
{
#ifdef _Windows
	UnicodeString cHomePath = "SOFTWARE";
	UnicodeString cFeatureBrowserEmulation = "Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION\\";
	//int cIE11 = 11001;
	UnicodeString sKey = ExtractFileName(ParamStr(0));
	TRegIniFile *Reg = new TRegIniFile(cHomePath);
	__try
	{
		TRegistry *reg1 = dynamic_cast<TRegistry*>(Reg);
		if (Reg->OpenKey(cFeatureBrowserEmulation,
			true) && !(reg1->KeyExists(sKey) && reg1->ReadInteger(sKey)
			== iIE))
		{
			reg1->WriteInteger(sKey, iIE);
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
	String sVolFrom = edFindSutra_VolFrom->Text;
	String sVolTo = edFindSutra_VolTo->Text;
	String sSutraFrom = edFindSutra_SutraFrom->Text;
	String sSutraTo = edFindSutra_SutraTo->Text;
	String sSutraName = edFindSutra_SutraName->Text;
	String sByline = edFindSutra_Byline->Text;

	// ���� CBETA �� ???
	// �v�@�j�M�ؿ�
	//if(IsSelectedBook())
	{
		// ���� CBETA �� ???
		// CSeries * Series = (CSeries *) Bookcase->Books->Items[SelectedBook];
		CSeries * Series = Bookcase->CBETA;

		CCatalog * Catalog = Series->Catalog;
		int iCount = Catalog->ID->Count;
		// �v�@�ˬd
		sgFindSutra->BeginUpdate();
		int iGridIndex = 0;
		sgFindSutra->RowCount = 10;
		for(int i=0; i<iCount; i++)
		{
			bool bFound = true;

			// ��U��
			if(!sVolFrom.IsEmpty())
				if(Catalog->Vol->Strings[i].ToInt() < sVolFrom.ToInt())
					continue;
			// ��U��
			if(!sVolTo.IsEmpty())
				if(Catalog->Vol->Strings[i].ToInt() > sVolTo.ToInt())
					continue;
			// ��g��
			if(!sSutraFrom.IsEmpty())
			{
				// �g���зǤ�
				sSutraFrom = Series->Spine->CBGetSutraNumFormat(sSutraFrom);
				String sSutra = Series->Spine->CBGetSutraNumFormat(Catalog->SutraNum->Strings[i]);
				sSutraFrom = sSutraFrom.LowerCase();
				sSutra = sSutra.LowerCase();
				if(sSutra < sSutraFrom)
					continue;
			}
			// ��g��
			if(!sSutraTo.IsEmpty())
			{
				// �g���зǤ�
				sSutraTo = Series->Spine->CBGetSutraNumFormat(sSutraTo);
				String sSutra = Series->Spine->CBGetSutraNumFormat(Catalog->SutraNum->Strings[i]);
				sSutraTo = sSutraTo.LowerCase();
				sSutra = sSutra.LowerCase();
				if(sSutraTo.Length() == 4 && sSutra.Length() == 5)
				{
					sSutraTo += "z";
                }
				if(sSutra > sSutraTo)
					continue;
			}
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
			sgFindSutra->Cells[1][iGridIndex]=Catalog->ID->Strings[i];
			sgFindSutra->Cells[2][iGridIndex]=Catalog->Vol->Strings[i];
			sgFindSutra->Cells[3][iGridIndex]=Catalog->Part->Strings[i];
			sgFindSutra->Cells[4][iGridIndex]=Catalog->SutraNum->Strings[i];
			sgFindSutra->Cells[5][iGridIndex]=Catalog->JuanNum->Strings[i];
			sgFindSutra->Cells[6][iGridIndex]=Catalog->Byline->Strings[i];
			sgFindSutra->Cells[7][iGridIndex]=i;
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
void __fastcall TfmMain::ShowCBXML(String sFile, bool bShowHighlight, TmyMonster * seSearchEngine)
{
	if(sFile == "")
	{
		ShowMessage("�S����쥿�T�ɮ�");
        return;
    }
	String sXMLFile = Bookcase->CBETA->Dir + sFile;
	String sJSFile = Bookcase->CBETA->Dir + Bookcase->CBETA->JSFile;
	CCBXML * CBXML = new CCBXML(sXMLFile, Setting, sJSFile, bShowHighlight, seSearchEngine);

	// ������, �]�� mac os ���ͥX�Ӫ��ɦW�O /var/tmp/xxxxx
	// windows �O xxxxxx
	// �ҥH����٬O�ۤv�g�a
	//char cOutFile[14];
	//std::tmpnam(cOutFile);


	String sOutFile = sFile + ".htm";
	sOutFile = StringReplace(sOutFile, "/", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = StringReplace(sOutFile, "\\", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = MyTempPath + sOutFile;

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

	String sFile = csCBETA->CBGetFileNameByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	ShowCBXML(sFile);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::sgFindSutraCellDblClick(TColumn * const Column, const int Row)
{
	int iIndex = sgFindSutra->Cells[7][Row].ToInt();

	CCatalog * cbCatalog = Bookcase->CBETA->Catalog;
	String sBookID = cbCatalog->ID->Strings[iIndex];
	String sSutra = cbCatalog->SutraNum->Strings[iIndex];

	String sFile = Bookcase->CBETA->CBGetFileNameBySutraNumJuan(sBookID, sSutra);
	ShowCBXML(sFile);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btOptionClick(TObject *Sender)
{
    fmOption->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
#ifdef _Windows
	SetPermissions(7000); // �]�w�� IE 7
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btTextSearchClick(TObject *Sender)
{
	SearchSentence = edTextSearch->Text;

	// �h���Y�����U�Φr��
	SearchSentence = CMyStrUtil::Trim(SearchSentence, u'?');

	if(SearchSentence == "") return;    // �S��J

	//RememberWord(cbSearchWord);		// �N�d�ߪ��r�s�_��
	//UpdateSearchHistory = true;

	//SearchSentenceOrig = cbSearchWord->Text;        // �̭�l���˯��y�r, �i��]�t unicode


	// ���ܷƹ�
	TCursor csOldCursor = Cursor;
	Cursor = crHourGlass;

	clock_t t1 = clock();
	bool bHasRange = false;     // ���d��N�n�]�w ????

	TmyMonster * SearchEngine = Bookcase->CBETA->SearchEngine;
	CCatalog * Catalog = Bookcase->CBETA->Catalog;
	CSpine * Spine = Bookcase->CBETA->Spine;
	bool bFindOK = SearchEngine->Find(SearchSentence,bHasRange);      // �b���o.........................................
	clock_t t2 = clock();

	int iFoundCount = SearchEngine->FileFound->Total;

    // �q�X���X�Ӫ��T��

	lbSearchMsg->Text = u"���" + String(iFoundCount) + u"���A�@��ɶ��G" + String(t2-t1);

    int iTotalSearchFileNum = 0;
    bool bShowAll = false;
    int iMaxSearchNum = 0;

    if(bFindOK)
    {
		SearchWordList->Clear();
		for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
			SearchWordList->Add(SearchEngine->SearchWordList->Strings[i]);	// �s�_�d�ߪ���

        // ���ˬd���S���W�L����

        for (int i=0; i<BuildFileList->FileCount; i++)
    	{
	    	if(SearchEngine->FileFound->Ints[i])
		    {
			    iTotalSearchFileNum++;
            }
        }

        // �N���G��J list �C��
	}

	sgTextSearch->BeginUpdate();
	int iGridIndex = 0;
	sgTextSearch->RowCount = 10;

	for (int i=0; i<BuildFileList->FileCount; i++)
	{
		// ���F
        if(SearchEngine->FileFound->Ints[i])
		{
			String sSutraNum  = BuildFileList->SutraNum[i];		// ���o�g��
			String sBook = BuildFileList->Book[i];

			// �o�̥i���� T220 �� 600 ��, �o�Ǧ^ T05 �Ӥ��O T07
			// ���ݧ�i�B�z ????
			int iCatalogIndex = Catalog->FindIndexBySutraNum(sBook,sSutraNum);	// ���o TripitakaMenu ���s��

			// ���F

			sgTextSearch->Cells[0][iGridIndex]=SearchEngine->FileFound->Ints[i];
			sgTextSearch->Cells[1][iGridIndex]=Catalog->SutraName->Strings[iCatalogIndex];
			sgTextSearch->Cells[2][iGridIndex]=Catalog->ID->Strings[iCatalogIndex];
			sgTextSearch->Cells[3][iGridIndex]=Spine->VolNum->Strings[i];
			sgTextSearch->Cells[4][iGridIndex]=Catalog->Part->Strings[iCatalogIndex];
			sgTextSearch->Cells[5][iGridIndex]=Catalog->SutraNum->Strings[iCatalogIndex];
			sgTextSearch->Cells[6][iGridIndex]=BuildFileList->JuanNum[i];
			sgTextSearch->Cells[7][iGridIndex]=Catalog->Byline->Strings[iCatalogIndex];
			sgTextSearch->Cells[8][iGridIndex]=i;
			iGridIndex++;

			if(iGridIndex >= sgTextSearch->RowCount)
				sgTextSearch->RowCount += 10;
		}
    }

	sgTextSearch->RowCount = iGridIndex;
	sgTextSearch->EndUpdate();

	// �٭�ƹ�
	Cursor=csOldCursor;

    if(bFindOK)
    {
		if(sgTextSearch->RowCount == 0)
	    {
			ShowMessage(u"�䤣�������");
    	}
    }
    else
	{
		ShowMessage(u"�d�ߦr��y�k�����D�A�ЦA�ˬd�ݬݡC");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::sgTextSearchCellDblClick(TColumn * const Column, const int Row)

{
	int iIndex = sgTextSearch->Cells[8][Row].ToInt();

	String sFile = Bookcase->CBETA->Spine->Files->Strings[iIndex];
    // �n���
	ShowCBXML(sFile, true, Bookcase->CBETA->SearchEngine);
}
//---------------------------------------------------------------------------

