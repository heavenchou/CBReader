// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
#include "selectbook.h"
#include "searchrange.h"
#include "buildindex.h"
#include "logo.h"
#include "about.h"

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
	Application->Title = "CBReader";
	ProgramTitle = u"CBETA �q�l��� 2018";

	// �]�w�ؿ����
	InitialPath();

#ifdef _Windows
	SetPermissions(11001); // �N IE �]�w�� IE 11 (�p�G�S IE 11 ���p��?)
#endif

	SearchEngine = 0;   // �����˯�����
	SearchSentence = "";    // �j�M�r��
	SearchWordList = new TStringList;	    // �s��C�@���˯�����, �����|�Ψ�

	// ���o�]�w�ɨ�Ū���Ҧ��]�w
	Setting = new CSetting(SettingFile);

	// ��l�e�����]�w
	btOpenBookcase->Visible = false;
	btBuildIndex->Visible = false;
	tcMainFunc->TabIndex = 0;
	SelectedBook = -1;   // �ثe�襤����, -1 ����٨S��
    pnMulu->Width = 0;  // �ѥإ��Y��̤p
    MuluWidth = 200;    // ��l�ѥؼe��
}
// ---------------------------------------------------------------------------
void __fastcall TfmMain::FormDestroy(TObject *Sender)
{
	if(SearchWordList) delete SearchWordList;
	if(Setting) delete Setting;
	if(Bookcase) delete Bookcase;
	if(NavTree) delete NavTree;
	if(MuluTree) delete MuluTree;
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
// ---------------------------------------------------------------------------
// 	���|��ȳ]�w
void __fastcall TfmMain::InitialPath()
{
	// �{���D�ؿ�
#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	MyFullPath = System::Ioutils::TPath::GetHomePath();
#endif
	MyFullPath += u"/";

	// Temp �ؿ�

	MyTempPath = System::Ioutils::TPath::GetTempPath();
	MyTempPath = MyTempPath + u"CBReader/";

	if(!TDirectory::Exists(MyTempPath))
		TDirectory::CreateDirectory(MyTempPath);

	// �ϥΪ̭ӤH�ؿ�

	MyHomePath = System::Ioutils::TPath::GetHomePath();

	// �]�w�ɥؿ�

#ifdef _Windows
	MySettingPath = MyHomePath + u"/CBETA/";
#else
	MySettingPath = MyHomePath + u"/.CBETA/";
#endif
	if(!TDirectory::Exists(MySettingPath))
		TDirectory::CreateDirectory(MySettingPath);
	MySettingPath += u"CBReader2X/";
	if(!TDirectory::Exists(MySettingPath))
		TDirectory::CreateDirectory(MySettingPath);

	// �]�w��

	SettingFile = MySettingPath + u"cbreader.ini";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormShow(TObject *Sender)
{
	Cursor = crHourGlass;

	Application->ProcessMessages();
	fmLogo->Show();
	Application->ProcessMessages();
	InitialData();

	fmLogo->Close();
	Cursor = crDefault;
}
//---------------------------------------------------------------------------
// ��l���
void __fastcall TfmMain::InitialData()
{
	// ���o Bookcase �Ҧ���ư�
	// ���J���d

	Bookcase = new CBookcase();

	String sBookcasePath = u"";

	// Bookcase �ؿ��B�z��h
	// 1. �p�G�� Setting->BookcaseFullDir , �h�����Ĥ@�u��
	// 2. Windows �b�D�{���Ҧb�ؿ����U�h�� Bookcase �l�ؿ�
	// 3. Mac ���G���u������
	//    3.1 /User/xxx/Library/CBETA/Bookcase
	//    3.2 /Library/CBETA/Bookcase
	// 4. �H�W�Y���S��, �h�ѨϥΪ̴M��, ����s�b Setting->BookcaseFullDir

	if(Setting->BookcaseFullPath != u"")
	{
		sBookcasePath = Setting->BookcaseFullPath;
    }
	else
	{
#ifdef _Windows
		sBookcasePath = MyFullPath + Setting->BookcasePath;
#else
		// Mac �Ĥ@�u��
		sBookcasePath = MyHomePath + u"/Library/CBETA/" + Setting->BookcasePath;
		if(!TDirectory::Exists(sBookcasePath))
		{
			// Mac �ĤG�u��
			sBookcasePath = u"/Library/CBETA/" + Setting->BookcasePath;
		}
#endif
	}

	if(!TDirectory::Exists(sBookcasePath))
	{
		TDialogService::ShowMessage(u"�S�����z�� Bookcase ���d�ؿ��A�Ф�ʿ�ܥؿ��Ҧb��m�C");
		// �ϥΫ��w�ؿ� ???? �ӧאּ�ϥΪ̫��w�ؿ��~�n
		SelectDirectory(u"��� Bookcase �ؿ��Ҧb��m",MyFullPath,sBookcasePath);

		Setting->BookcaseFullPath = sBookcasePath;
		Setting->SaveToFile();
	}

	Bookcase->LoadBookcase(sBookcasePath);

	// �b���d����O��
	int iBookcaseCount = Bookcase->Count();
	if(iBookcaseCount == 0)
	{
		TDialogService::ShowMessage(u"���d���@���ѳ��S��");
	}
	// else if(iBookcaseCount == 1)
	else
	{
		// �u���@���ѴN�����}�F
		// OpenBookcase(0); // ???? �Ȯɨ���, �o�@���n�����}�� CBETA
		OpenCBETABook();    // ???? �����W��, �]���o�@���n�����}�� CBETA
	}

	MuluTree = 0;

    // �o�n���B�z, �ʤ@�U���e, ���M�����D�N�٥i�H����
	sgTextSearch->RowCount = 1;
	sgFindSutra->RowCount = 1;

	lbSearchMsg->Text = ""; // �M�ŷj�M�T��
	SpineID = -1;	// ��Ȫ�ܨS�}��
#ifdef _Windows
	String sToday = GetTodayString();
	if(sToday != Setting->LastUpdateChk)
		CheckUpdate(u"");   // �ˬd��s
#endif
	if(iBookcaseCount != 0)
	{
		WebBrowser->URL = "file://" + Bookcase->CBETA->Dir + u"help/index.htm";
		WebBrowser->Navigate();
	}
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
// �}��CBETA���d
void __fastcall TfmMain::OpenCBETABook()
{
	for(int i=0; i<Bookcase->Count(); i++)
	{
		if(Bookcase->Books->Items[i] == Bookcase->CBETA)
		{
			// ��� CBETA �F
			OpenBookcase(i);
			return;
		}
	}
	TDialogService::ShowMessage("�䤣�� CBETA ���");
    return;
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
	// ???? �o�����, ���n����
	// �]���q��g�ѥ��I���, ���ɨS���}�� SelectedBook
	if(SelectedBook <0) SelectedBook = 0;
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
		LoadNavTree(sSeries->Dir + sURL);
	}
	// CBETA �g��
	else if(iType == nit_CBLink)
	{
		ShowCBXML(sURL);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btOpenBookcaseClick(TObject *Sender)
{
	fmSelectBook->ShowModal();
	OpenBookcase(fmSelectBook->SelectedBook);
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
	String sBook = cbFindSutra_BookId->Selected->Text;
	if(cbFindSutra_BookId->ItemIndex == 0) sBook = u"";
	else
	{
		int iPos = sBook.Pos0(u" ");
		sBook.SetLength(iPos);
    }
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

			// ���øg
			if(!sBook.IsEmpty())
				if(Catalog->ID->Strings[i] != sBook)
					continue;

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
				if(Catalog->SutraName->Strings[i].Pos0(sSutraName) < 0)
					continue;
			// ��Ķ��
			if(!sByline.IsEmpty())
				if(Catalog->Byline->Strings[i].Pos0(sByline) < 0)
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

	String sBook = cbGoSutra_BookId->Items->Strings[cbGoSutra_BookId->ItemIndex];

	int iPos = sBook.Pos0(u" ");
	sBook.SetLength(iPos);

	String sSutraNum = edGoSutra_SutraNum->Text;
	String sJuan = edGoSutra_Juan->Text;
	String sPage = edGoSutra_Page->Text;
	String sField = edGoSutra_Field->Text;
	String sLine = edGoSutra_Line->Text;

	CSeries * csCBETA = Bookcase->CBETA;

	String sFile = csCBETA->CBGetFileNameBySutraNumJuan(sBook, sSutraNum, sJuan, sPage, sField, sLine);
	ShowCBXML(sFile);

}
//---------------------------------------------------------------------------
// ���J XML �óB�z������
void __fastcall TfmMain::ShowCBXML(String sFile, bool bShowHighlight, TmyMonster * seSearchEngine)
{
	if(sFile == "")
	{
		TDialogService::ShowMessage("�S����쥿�T�ɮ�");
        return;
	}

	// �p�G�ǨӪ��O XML/T/T01/T01n0001_001.xml#p0001a01
	// �h�n�� p0001a01 �����X��

	String sLink = "";
	int iPos = sFile.Pos0("#");
	if(iPos >= 0)
	{
		sLink = sFile.SubString0(iPos+1,sFile.Length()-iPos-1);
		sFile = sFile.SubString0(0,iPos);
    }

	String sXMLFile = Bookcase->CBETA->Dir + sFile;
	String sJSFile = Bookcase->CBETA->Dir + Bookcase->CBETA->JSFile;
	CCBXML * CBXML = new CCBXML(sXMLFile, sLink, Setting, sJSFile, bShowHighlight, seSearchEngine);

	// ��X spine id , -1 ��ܨS���
	SpineID = Bookcase->CBETA->Spine->Files->IndexOf(sFile);
#ifdef _Windows
	String sOutFile = sFile + u".htm";
#else
	String sOutFile = u"a.htm";
#endif
	sOutFile = StringReplace(sOutFile, "/", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = StringReplace(sOutFile, "\\", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = MyTempPath + sOutFile;

	CBXML->SaveToHTML(sOutFile);

	try
	{
		//WebBrowser->URL = (u"file://" + sOutFile);
		WebBrowser->Navigate(u"file://" + sOutFile);
	}
	catch(...)
	{
        //WebBrowser->Navigate(u"file://" + sOutFile);
    }

	// ���ͥؿ�

	String sMulu = StringReplace(sFile, "XML", "toc", TReplaceFlags() << rfReplaceAll);
	int iLen = sMulu.Length();
	sMulu = sMulu.SubString0(0,iLen-8); // �����̫᪺ _001.xml

	// Toc/T/T01/T01n0001 => Toc/T/T0001
	TRegEx *regex = new TRegEx();
	sMulu = regex->Replace(sMulu, "\\d+[\\/][A-Z]+\\d+n", "");
	sMulu = Bookcase->CBETA->Dir + sMulu + ".xml";
	if(TFile::Exists(sMulu))
	{
		if(MuluTree == 0 || sMulu != MuluTree->XMLFile)
		{
			LoadMuluTree(sMulu);
		}
	}

	// ��� form �� title

	if(SpineID >= 0)
	{
		String sBook = Bookcase->CBETA->Spine->BookID->Strings[SpineID];
		String sVol = Bookcase->CBETA->Spine->Vol->Strings[SpineID];
		String sSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];
		String sJuan = Bookcase->CBETA->Spine->Juan->Strings[SpineID];
		int iIndex = Bookcase->CBETA->Catalog->FindIndexBySutraNum(sBook, sSutra);
		String sName = Bookcase->CBETA->Catalog->SutraName->Strings[iIndex];

		sJuan = CMyStrUtil::TrimLeft(sJuan, u'0');
		sSutra = CMyStrUtil::TrimLeft(sSutra, u'0');
		String sCaption = ProgramTitle + u"�m" + sName + u"�n"
				+ sVol + u", No. " + sSutra + u", ��" + sJuan;
		Caption = sCaption;
	}
}
//---------------------------------------------------------------------------
// �ѥU�����e�{�g��
void __fastcall TfmMain::btGoBookClick(TObject *Sender)
{
	String sBook = cbGoBook_BookId->Items->Strings[cbGoBook_BookId->ItemIndex];

	int iPos = sBook.Pos0(u" ");
	sBook.SetLength(iPos);

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
	bool bHasRange = false;     // ���d��N�n�]�w
	if(cbSearchRange->IsChecked) bHasRange = true;

	// ��ܥ����˯�����, �Y�Y�@�謰 0 , �h��t�@�� (�� 0 �N���ޤF)
	SetSearchEngine();

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

		for (int i=0; i<SearchEngine->BuildFileList->FileCount; i++)
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

	for (int i=0; i<SearchEngine->BuildFileList->FileCount; i++)
	{
		// ���F
        if(SearchEngine->FileFound->Ints[i])
		{
			String sSutraNum  = SearchEngine->BuildFileList->SutraNum[i];		// ���o�g��
			String sBook = SearchEngine->BuildFileList->Book[i];

			// �o�̥i���� T220 �� 600 ��, �o�Ǧ^ T05 �Ӥ��O T07
			// ���ݧ�i�B�z ????
			if(i > 720)
			{
				int j;
				j++;
            }
			int iCatalogIndex = Catalog->FindIndexBySutraNum(sBook,sSutraNum);	// ���o TripitakaMenu ���s��

			// ���F

			sgTextSearch->Cells[0][iGridIndex]=SearchEngine->FileFound->Ints[i];
			sgTextSearch->Cells[1][iGridIndex]=Catalog->SutraName->Strings[iCatalogIndex];
			sgTextSearch->Cells[2][iGridIndex]=Catalog->ID->Strings[iCatalogIndex];
			sgTextSearch->Cells[3][iGridIndex]=Spine->VolNum->Strings[i];
			sgTextSearch->Cells[4][iGridIndex]=Catalog->Part->Strings[iCatalogIndex];
			sgTextSearch->Cells[5][iGridIndex]=Catalog->SutraNum->Strings[iCatalogIndex];
			sgTextSearch->Cells[6][iGridIndex]=SearchEngine->BuildFileList->JuanNum[i];
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
			TDialogService::ShowMessage(u"�䤣�������");
    	}
    }
    else
	{
		TDialogService::ShowMessage(u"�d�ߦr��y�k�����D�A�ЦA�ˬd�ݬݡC");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::sgTextSearchCellDblClick(TColumn * const Column, const int Row)

{
	int iIndex = sgTextSearch->Cells[8][Row].ToInt();

	String sFile = Bookcase->CBETA->Spine->Files->Strings[iIndex];
    // �n���
	ShowCBXML(sFile, true, SearchEngine);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::cbSearchRangeChange(TObject *Sender)
{
	if(cbSearchRange->IsChecked)
	{
		// �]�w�˯��d��
		TModalResult mr = fmSearchRange->ShowModal();
		if(mr == mrCancel) cbSearchRange->IsChecked = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btGoByKeywordClick(TObject *Sender)
{
	// �P�_�U�ت����e�����w�g�媺��k
	// 1. �歺 T01n0001_p0001a01
	// 2. �ޥνƻs T01,no.1,p.1,b5
	// 3. �S�w���N�X, �Ҧp : SN1.1

	TRegEx *regex;
	TMatchCollection reMatch;
	TGroupCollection reGroup;

	String sKey = edGoByKeyword->Text;
	// T01n0001_p0001a01
	String sPatten = "(\\D+)(\\d+)n.{5}p(.{4})(.)(\\d\\d)";

	regex = new TRegEx();
	reMatch = regex->Matches(sKey, sPatten);
	if(reMatch.Count)
	{
		reGroup = reMatch.Item[0].Groups;

		String sBook = reGroup.Item[1].Value;
		String sVol = reGroup.Item[2].Value;
		String sPage = reGroup.Item[3].Value;
		String sField = reGroup.Item[4].Value;
		String sLine = reGroup.Item[5].Value;

		CSeries * csCBETA = Bookcase->CBETA;

		String sFile = csCBETA->CBGetFileNameByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
		ShowCBXML(sFile);
	}

}
//---------------------------------------------------------------------------
// �إ߯�����
void __fastcall TfmMain::btBuildIndexClick(TObject *Sender)
{
	// �����������˯�����

	Bookcase->CBETA->FreeSearchEngine();

	// ���ͯ���

	fmBuildIndex->edBuildListDir->Text = Bookcase->CBETA->Dir;
	fmBuildIndex->edBuildList->Text = Bookcase->CBETA->Dir + Bookcase->CBETA->SpineFile;
	fmBuildIndex->edWordIndex->Text = Bookcase->CBETA->Dir + "index/wordindex.ndx";
	fmBuildIndex->edMainIndex->Text = Bookcase->CBETA->Dir + "index/main.ndx";
	fmBuildIndex->ShowModal();

	// ���s�إߥ����˯�����

	Bookcase->CBETA->LoadSearchEngine();

	SetSearchEngine();

}
//---------------------------------------------------------------------------
// �N�ɮ׸��J������
void __fastcall TfmMain::LoadNavTree(String sFile)
{
	if(NavTree != 0 && NavTree->XMLFile == sFile) return;

	if(NavTree) delete NavTree;
	NavTree = new CNavTree(sFile);
	NavTree->SaveToTreeView(tvNavTree, NavTreeItemClick);
}
//---------------------------------------------------------------------------
// �N�ɮ׸��J�ؿ���
void __fastcall TfmMain::LoadMuluTree(String sFile)
{
	if(MuluTree != 0 && MuluTree->XMLFile == sFile) return;

	if(MuluTree) delete MuluTree;
	MuluTree = new CNavTree(sFile);
	MuluTree->SaveToTreeView(tvMuluTree, NavTreeItemClick);

	// �i�}�Ĥ@�h

	for(int i=0; i<tvMuluTree->ChildrenCount; i++)
	{
		try
		{
			tvMuluTree->Items[i]->Expand();
		}
		catch(...)
		{
            // ����...
        }
	}

	// �ˬd�ѥذϬO���O�Y��̤p

	if(pnMulu->Width == 0)
		btMuluWidthSwitchClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btOpenBuleiNavClick(TObject *Sender)
{
	LoadNavTree(Bookcase->CBETA->Dir + Bookcase->CBETA->NavFile);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btOpenBookNavClick(TObject *Sender)
{
	LoadNavTree(Bookcase->CBETA->Dir + Bookcase->CBETA->Nav2File);
}
//---------------------------------------------------------------------------
// �W�@��
void __fastcall TfmMain::btPrevJuanClick(TObject *Sender)
{
	if(SpineID == -1) return;
	if(SpineID > 0)
	{
			// �ˬd�O���O�P�@�g
		String sThisSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];
		String sNextSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID-1];
		if(sThisSutra == sNextSutra)
		{
			String sFile = Bookcase->CBETA->Spine->CBGetFileNameBySpineIndex(SpineID-1);
			ShowCBXML(sFile);
			return;
		}
	}

	TDialogService::ShowMessage(u"�����w�O�̪��");
}
//---------------------------------------------------------------------------
// �U�@��
void __fastcall TfmMain::btNextJuanClick(TObject *Sender)
{
	if(SpineID == -1) return;
	if(SpineID + 1 < Bookcase->CBETA->Spine->Files->Count)
	{
		// �ˬd�O���O�P�@�g
		String sThisSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];
		String sNextSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID+1];
		if(sThisSutra == sNextSutra)
		{
			String sFile = Bookcase->CBETA->Spine->CBGetFileNameBySpineIndex(SpineID+1);
			ShowCBXML(sFile);
			return;
		}
	}

	TDialogService::ShowMessage(u"�����w�O�̫�@��");
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::MenuItem1Click(TObject *Sender)
{
	fmAbout->ShowModal();
}
//---------------------------------------------------------------------------

// �ˬd���S����s�{��
void __fastcall TfmMain::CheckUpdate(String sPara)
{

#ifdef _Windows

	//HWND handle = fmMain->Handle;

	HINSTANCE  hResult;
	int iResult;
	String sFileName = MyFullPath + u"update.exe";

	hResult = ShellExecute(0,L"open",sFileName.c_str(),sPara.c_str(),MyFullPath.c_str(), SW_SHOW);

	iResult = (int) hResult;
	switch(iResult)
	{
		case 0                    : TDialogService::ShowMessage(u"�t�ΰO����θ귽����, �L�k���Q�}��!"); break;
		case ERROR_FILE_NOT_FOUND : TDialogService::ShowMessage(u"�䤣���s�ˬd�ɮ�" + sFileName); break;
		case SE_ERR_NOASSOC       : TDialogService::ShowMessage(u"�����ɮ׵L�n�O, �L�k���Q�}��!"); break;
		//case SE_ERR_OOM           : TDialogService::ShowMessage("�O���餣��, �L�k���Q�}��!"); break;
		default:
		{
			if(iResult <= 32)  // �䥦���p�����~
			{
				TDialogService::ShowMessage("SE_Err:" + AnsiString(iResult) + " �L�k�}�Ҧ���!");
			}
		}
	}

	String sToday = GetTodayString();
	Setting->LastUpdateChk = sToday;
    Setting->SaveToFile();
#else
	TDialogService::ShowMessage("��p�I�ثe�u�� Windows ���~����s�\��C");
#endif
}
//---------------------------------------------------------------------------
// �ˬd��s
void __fastcall TfmMain::MenuItem4Click(TObject *Sender)
{
	CheckUpdate(u"show");
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btNavWidthSwitchClick(TObject *Sender)
{
	if(pnNav->Width == 0)
	{
		// �}�ҥD�\���
		fanNavWidth->StartValue = 0;
		fanNavWidth->StopValue = NavWidth;
		fanNavWidth->Interpolation = TInterpolationType::Quintic;
		fanNavWidth->Duration = 0.5;
		fanNavWidth->Start();
	}
	else
	{
		NavWidth = pnNav->Width;
		fanNavWidth->StartValue = NavWidth;
		fanNavWidth->StopValue = 0;
		fanNavWidth->Interpolation = TInterpolationType::Bounce;
        fanNavWidth->Duration = 1;
		fanNavWidth->Start();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btMuluWidthSwitchClick(TObject *Sender)
{
	if(pnMulu->Width == 0)
	{
		fanMuluWidth->StartValue = 0;
		fanMuluWidth->StopValue = MuluWidth;
		fanMuluWidth->Interpolation = TInterpolationType::Quintic;
		fanMuluWidth->Start();
	}
	else
	{
		MuluWidth = pnMulu->Width;
		fanMuluWidth->StartValue = MuluWidth;
		fanMuluWidth->StopValue = 0;
		fanMuluWidth->Interpolation = TInterpolationType::Quintic;
		fanMuluWidth->Start();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::fanNavWidthFinish(TObject *Sender)
{
	if(pnNav->Width == 0)
		btNavWidthSwitch->Text = u"�D�\��>>";
	else
		btNavWidthSwitch->Text = u"<<�D�\��";
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::fanMuluWidthFinish(TObject *Sender)
{
	if(pnMulu->Width == 0)
		btMuluWidthSwitch->Text = u"�ѥ�>>";
	else
		btMuluWidthSwitch->Text = u"<<�ѥ�";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::rbFontSmallChange(TObject *Sender)
{
	if(rbFontBig->IsChecked)
	{
		StyleBook = sbBig;
		fmOption->StyleBook = sbBig;
		fmSearchRange->StyleBook = sbBig;
		sgTextSearch->TextSettings->Font->Size = 16;
		sgFindSutra->TextSettings->Font->Size = 16;
	}
	else if(rbFontMid->IsChecked)
	{
		StyleBook = sbMid;
		fmOption->StyleBook = sbMid;
		fmSearchRange->StyleBook = sbMid;
		sgTextSearch->TextSettings->Font->Size = 14;
		sgFindSutra->TextSettings->Font->Size = 14;
	}
	else if(rbFontSmall->IsChecked)
	{
		StyleBook = sbSmall;
		fmOption->StyleBook = sbSmall;
		fmSearchRange->StyleBook = sbSmall;
		sgTextSearch->TextSettings->Font->Size = 12;
		sgFindSutra->TextSettings->Font->Size = 12;
	}
}
//---------------------------------------------------------------------------
// ��ܥ����˯�����
void __fastcall TfmMain::SetSearchEngine()
{
	// ��ܥ����˯�����, �Y�Y�@�謰 0 , �h��t�@�� (�� 0 �N���ޤF)
	if(Bookcase->CBETA->SearchEngine_CB == 0)
		SearchEngine = Bookcase->CBETA->SearchEngine_orig;  // ��ѯ���
	else if(Bookcase->CBETA->SearchEngine_orig == 0)
		SearchEngine = Bookcase->CBETA->SearchEngine_CB;  // CBETA ����

	else if(Setting->CollationType == ctCBETACollation)
		SearchEngine = Bookcase->CBETA->SearchEngine_CB;    // CBETA ����
	else
		SearchEngine = Bookcase->CBETA->SearchEngine_orig;  // ��ѯ���
}
//---------------------------------------------------------------------------
// ���o������, �榡 19991231
String __fastcall TfmMain::GetTodayString()
{
	TDateTime * Today = new TDateTime(Now());
	String str = Today->FormatString("yyyymmdd");
	delete Today;
	return str;
}
//---------------------------------------------------------------------------


