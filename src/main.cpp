// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
#include "selectbook.h"
#include "searchrange.h"
#include "buildindex.h"
#include "logo.h"
#include "about.h"
#include "update.h"
#include "createhtml.h"

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
	// ��s�����`�N�ƶ�, �n�侀�U, �٦� project ������
    // �٦� fmAbout �������P������
	Application->Title = u"CBReader";
	ProgramTitle = u"CBETA �q�l��� 2018";
	Version = u"0.4.2.0";   // ���� .1 �O���转, .2 �O�转+CBETA
	DebugString = u"Heaven";     // debug �f�O
	IsDebug = false;           // debug �ܼ�

	// �转�b�b SLReader �M��
	//Application->Title = u"SLReader";

	bool IsAllSL = true;    // �O�_�������O�转?
	if(*Version.LastChar() != '1') IsAllSL = false;
	if(Application->Title == u"SLReader")
	{
		if(IsAllSL) ProgramTitle = u"�转�b�b���m����";
		Caption = ProgramTitle;

		if(IsAllSL)
		{
			cbFindSutra_BookId->Items->Clear();
			cbFindSutra_BookId->Items->Add(u"����");
		}
		cbFindSutra_BookId->Items->Add(u"DA �D�w�k�v�ۧ@����");
		cbFindSutra_BookId->Items->Add(u"ZY ���٪k�v�ۧ@����");
		cbFindSutra_BookId->Items->Add(u"HM �f�Ӫk�v������");
		cbFindSutra_BookId->ItemIndex = 0;

		if(IsAllSL)
		{
			cbGoSutra_BookId->Items->Clear();
			cbGoSutra_BookId->Items->Add(u"����");
		}
		cbGoSutra_BookId->Items->Add(u"DA �D�w�k�v�ۧ@����");
		cbGoSutra_BookId->Items->Add(u"ZY ���٪k�v�ۧ@����");
		cbGoSutra_BookId->Items->Add(u"HM �f�Ӫk�v������");
		cbGoSutra_BookId->ItemIndex = 0;

		if(IsAllSL)
		{
			cbGoBook_BookId->Items->Clear();
			cbGoBook_BookId->Items->Add(u"����");
		}
		cbGoBook_BookId->Items->Add(u"DA �D�w�k�v�ۧ@����");
		cbGoBook_BookId->Items->Add(u"ZY ���٪k�v�ۧ@����");
		cbGoBook_BookId->Items->Add(u"HM �f�Ӫk�v������");
		cbGoBook_BookId->ItemIndex = 0;

		if(IsAllSL)
		{
			Panel8->Visible = false;
			cbSearchRange->Visible = false;

			lbFindSutra_Book->Text = u"�O��";
			lbGoSutra_Book->Text = u"�O��";
			lbGoBook_Book->Text = u"�O��";

			lbFindSutra_SutraNum->Text = u"�s���q";
			lbGoSutra_SutraNum->Text = u"�s��";

			lbFindSutra_SutraName->Text = u"�ۧ@";

			sgFindSutra->Columns[0]->Header = u"�ۧ@";
			sgFindSutra->Columns[1]->Header = u"�O��";
			sgFindSutra->Columns[3]->Width = 1;
			sgFindSutra->Columns[4]->Header = u"�s��";

			sgTextSearch->Columns[1]->Header = u"�ۧ@";
			sgTextSearch->Columns[2]->Header = u"�O��";
			sgTextSearch->Columns[4]->Width = 1;
			sgTextSearch->Columns[5]->Header = u"�s��";
		}
	}

#ifdef _Windows
	MainMenu->Free();
#else
    MenuBar->Free();
#endif

	// �]�w�ؿ����
	InitialPath();

#ifdef _Windows
	SetPermissions(11001); // �N IE �]�w�� IE 11 (�p�G�S IE 11 ���p��?)
	// �R�h�ª�
	String sOld = ParamStr(0) + u".tmp";
	if(TFile::Exists(sOld))
        TFile::Delete(sOld);
#endif

	SearchEngine = 0;   // �����˯�����
	SearchSentence = "";    // �j�M�r��
	SearchWordList = new TStringList;	    // �s��C�@���˯�����, �����|�Ψ�

	// ���o�]�w�ɨ�Ū���Ҧ��]�w
	Setting = new CSetting(SettingFile);

	// ��l�e�����]�w
	btOpenBookcase->Visible = false;
	btOpenSimpleNav->Visible = false;
	btOpenBookNav->Visible = false;
	btCopy->Visible = false;
	btCiteCopy->Visible = false;

	// �]���U�Կ��e�פ��ର�_��, �ҥH�n�վ�
	if(Floor(cbFindSutra_BookId->Width) % 2 == 1) cbFindSutra_BookId->Width -= 1;
	if(Floor(cbGoSutra_BookId->Width) % 2 == 1) cbGoSutra_BookId->Width -= 1;
	if(Floor(cbGoBook_BookId->Width) % 2 == 1) cbGoBook_BookId->Width -= 1;

	#ifdef _Windows
	wmiDebug->Visible = false;
	#endif

	tcMainFunc->TabIndex = 0;
	SelectedBook = -1;   // �ثe�襤����, -1 ����٨S��
    pnMulu->Width = 0;  // �ѥإ��Y��̤p
	MuluWidth = 200;    // ��l�ѥؼe��

	sgTextSearch->OnKeyDown = sgTextSearchKeyDown;
	sgFindSutra->OnKeyDown = sgFindSutraKeyDown;
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
	MyFullPath = ExtractFilePath(ParamStr(0));
#else
	MyFullPath = System::Ioutils::TPath::GetHomePath();
#endif
	MyFullPath = IncludeTrailingPathDelimiter(MyFullPath);

	// Temp �ؿ�

	MyTempPath = System::Ioutils::TPath::GetTempPath();
	MyTempPath = IncludeTrailingPathDelimiter(MyTempPath + u"CBReader");

	if(!TDirectory::Exists(MyTempPath))
		TDirectory::CreateDirectory(MyTempPath);

	// �ϥΪ̭ӤH�ؿ�

	MyHomePath = System::Ioutils::TPath::GetHomePath();
	MyHomePath = IncludeTrailingPathDelimiter(MyHomePath);

	// �]�w�ɥؿ�

#ifdef _Windows
	MySettingPath = MyHomePath + u"CBETA/";
#else
	MySettingPath = MyHomePath + u".CBETA/";
#endif
	if(!TDirectory::Exists(MySettingPath))
		TDirectory::CreateDirectory(MySettingPath);
	MySettingPath += u"CBReader2X/";
	if(!TDirectory::Exists(MySettingPath))
		TDirectory::CreateDirectory(MySettingPath);

	MySettingPath = IncludeTrailingPathDelimiter(MySettingPath);
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
	Cursor = crArrow;

	// �ˬd��s
	String sToday = GetTodayString();
	if(sToday != Setting->LastUpdateChk)
		CheckUpdate();   // �ˬd��s
}
//---------------------------------------------------------------------------
// ��l���
void __fastcall TfmMain::InitialData()
{
	/* ���ΤF, Windows �w�g�T�w��o��ϥΪ̥ؿ��F
	#ifdef _Windows

	// �ˬd MyFullPath ���S�����~
	if(!TFile::Exists(String(MyFullPath + u"CBReader.exe")))
	{
		// �S�ݨ� CBReader.exe , �i�঳���D

		bool bFound = false;
		String sMyFullPath = u"";

		// �ˬd�]�w�����S�� MyFullPath , �ˬd�������T
		if(Setting->MyFullPath != u"")
		{
			sMyFullPath = Setting->MyFullPath;
			if(TFile::Exists(String(sMyFullPath + u"CBReader.exe")))
			{
				bFound = true;
			}
		}

		while(!bFound)
		{
			// ���䤣��ثe�ؿ�, �n�߰ݨϥΪ̤F
			TDialogService::ShowMessage(u"�S�����z�� CBReader.exe �Ҧb�ؿ��A�Ф�ʿ�ܥ��{���Ҧb�ؿ���m�C");
			SelectDirectory(u"��� CBReader.exe �Ҧb�ؿ���m",MyFullPath,sMyFullPath);

            if(TFile::Exists(String(sMyFullPath + u"CBReader.exe")))
			{
				bFound = true;
			}
        }

		// �ܦ����ӳ����F
		MyFullPath = sMyFullPath;
		if(Setting->MyFullPath != MyFullPath)
		{
			// �N MyFullPath �g�J
			Setting->MyFullPath = MyFullPath;
			Setting->SaveToFile();
		}
	}
	#endif
	*/

	// ���o Bookcase �Ҧ���ư�
	// ���J���d

	Bookcase = new CBookcase();

	String sBookcasePath = u"";

	// Bookcase �ؿ��B�z��h
	// 1. Windows �b�D�{���Ҧb�ؿ����U�h�� Bookcase �l�ؿ�
	// 2. Mac ���G���u������
	//    3.1 /User/xxx/Library/CBETA/Bookcase
	//    3.2 /Library/CBETA/Bookcase
	// 3. �W���Y�S��, �h�� Setting->BookcaseFullDir
	// 4. �H�W�Y���S��, �h�ѨϥΪ̴M��, ����s�b Setting->BookcaseFullDir


#ifdef _Windows
	sBookcasePath = MyFullPath + Setting->BookcasePath;
#else
	// Mac �Ĥ@�u��
	sBookcasePath = MyHomePath + u"Library/CBETA/" + Setting->BookcasePath;
	if(!TDirectory::Exists(sBookcasePath))
	{
		// Mac �ĤG�u��
		sBookcasePath = u"/Library/CBETA/" + Setting->BookcasePath;
	}
#endif

	// ���S���N�d�]�w
	if(!TDirectory::Exists(sBookcasePath))
	{
		if(Setting->BookcaseFullPath != u"")
		{
			sBookcasePath = Setting->BookcaseFullPath;
		}
	}

	// ���S���N�߰ݨϥΪ�
	if(!TDirectory::Exists(sBookcasePath))
	{
		TDialogService::ShowMessage(u"�S�����z�� Bookcase ���d�ؿ��A�Ф�ʿ�ܥؿ��Ҧb��m�C");
		// �ϥΫ��w�ؿ�
		SelectDirectory(u"��� Bookcase �ؿ��Ҧb��m", MyFullPath, sBookcasePath);

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

	if(iBookcaseCount != 0)
	{
		WebBrowser->URL = "file://" + Bookcase->CBETA->Dir + u"help/index.htm";
		WebBrowser->Navigate();
	}

	// �转�b�b SLReader �M��
	// �˯��d��n�[�W�转
	if(Application->Title == u"SLReader")
	{
    	// �������X�� 1 ����ܥ����转, �N���ΰ��˯��d��F
		if(*Version.LastChar() != '1')
		{
			TTreeViewItem * newItem1 = new TTreeViewItem(fmSearchRange->tvBook);
			newItem1->Text = u"DA �D�w�k�v�ۧ@����";   // ���D
			fmSearchRange->tvBook->AddObject(newItem1);

			TTreeViewItem * newItem2 = new TTreeViewItem(fmSearchRange->tvBook);
			newItem2->Text = u"ZY ���٪k�v�ۧ@����";   // ���D
			fmSearchRange->tvBook->AddObject(newItem2);

			TTreeViewItem * newItem3 = new TTreeViewItem(fmSearchRange->tvBook);
			newItem3->Text = u"HM �f�Ӫk�v������";   // ���D
			fmSearchRange->tvBook->AddObject(newItem3);
		}
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
	TDialogService::ShowMessage(u"�䤣�� CBETA ���");
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
	tvNavTree->Cursor = crHourGlass;

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
	tvNavTree->Cursor = crDefault;
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
	if(edFindSutra_SutraName->Text == DebugString)
	{
		IsDebug = true;
		edFindSutra_SutraName->Text = u"";
		#ifdef _Windows
		wmiDebug->Visible = true;
    	#endif
		return;
    }
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

			sgFindSutra->Cells[0][iGridIndex]=Catalog->ID->Strings[i];
			sgFindSutra->Cells[1][iGridIndex]=Catalog->Vol->Strings[i];
			sgFindSutra->Cells[2][iGridIndex]=Catalog->SutraNum->Strings[i];
			sgFindSutra->Cells[3][iGridIndex]=Catalog->SutraName->Strings[i];
			sgFindSutra->Cells[4][iGridIndex]=Catalog->JuanNum->Strings[i];
			sgFindSutra->Cells[5][iGridIndex]=Catalog->Part->Strings[i];
			sgFindSutra->Cells[6][iGridIndex]=Catalog->Byline->Strings[i];
			sgFindSutra->Cells[7][iGridIndex]=i;
			iGridIndex++;

			if(iGridIndex >= sgFindSutra->RowCount)
				sgFindSutra->RowCount += 10;
		}
		sgFindSutra->RowCount = iGridIndex;
		sgFindSutra->EndUpdate();

		lbFindSutraCount->Text = u"�@��� " + String(iGridIndex) + u" ��";
		if(iGridIndex == 0)
            TDialogService::ShowMessage(u"�S�����������󪺸��");
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

	if(sField == "1") sField = u"a";
	else if(sField == "2") sField = u"b";
	else if(sField == "3") sField = u"c";
	else if(sField == "4") sField = u"d";
	else if(sField == "5") sField = u"e";
	else if(sField == "6") sField = u"f";
	else if(sField == "7") sField = u"g";
	else if(sField == "8") sField = u"h";
	else if(sField == "9") sField = u"i";

	CSeries * csCBETA = Bookcase->CBETA;

	String sFile = csCBETA->CBGetFileNameBySutraNumJuan(sBook, "", sSutraNum, sJuan, sPage, sField, sLine);
	ShowCBXML(sFile);

}
//---------------------------------------------------------------------------
// ���J XML �óB�z������
void __fastcall TfmMain::ShowCBXML(String sFile, bool bShowHighlight, TmyMonster * seSearchEngine)
{
	if(sFile == "")
	{
		TDialogService::ShowMessage(u"�S����쥿�T�ɮ�");
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

	// toc/T/T01/T01n0001 => toc/T/T0001
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
		String sVolNum = Bookcase->CBETA->Spine->VolNum->Strings[SpineID];
		String sSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];
		String sJuan = Bookcase->CBETA->Spine->Juan->Strings[SpineID];
		int iIndex = Bookcase->CBETA->Catalog->FindIndexBySutraNum(sBook, sVolNum, sSutra);
		String sName = Bookcase->CBETA->Catalog->SutraName->Strings[iIndex];

		// �g�W���� (��X��-��x��)
		sName = CMyCBUtil::CutJuanAfterSutraName(sName);
		sJuan = CMyStrUtil::TrimLeft(sJuan, u'0');
		sSutra = CMyStrUtil::TrimLeft(sSutra, u'0');
		String sCaption = ProgramTitle + u"�m" + sName + u"�n"
				+ sVol + u", No. " + sSutra + u", ��/�g��" + sJuan;
		Caption = sCaption;

		// �N�g�W�᭱�� �]�W���U�@�G�T......�Q�^����
		sName = CMyCBUtil::CutNumberAfterSutraName(sName);
		cbSearchThisSutra->Text = u"�˯����g�G" + sName;
        cbSearchThisSutraChange(this);  // �]�w�˯����g���������
	}

	// �˯����g

	cbSearchThisSutra->Enabled = true;

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

	if(sField == "1") sField = u"a";
	else if(sField == "2") sField = u"b";
	else if(sField == "3") sField = u"c";
	else if(sField == "4") sField = u"d";
	else if(sField == "5") sField = u"e";
	else if(sField == "6") sField = u"f";
	else if(sField == "7") sField = u"g";
	else if(sField == "8") sField = u"h";
	else if(sField == "9") sField = u"i";

	CSeries * csCBETA = Bookcase->CBETA;

	String sFile = csCBETA->CBGetFileNameByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	ShowCBXML(sFile);
}
//---------------------------------------------------------------------------
// �ۭq���j�M�C�� OnKeyDown
void __fastcall TfmMain::sgFindSutraKeyDown(TObject *Sender, System::Word &Key, System::WideChar &KeyChar, System::Classes::TShiftState Shift)
{
	if(Key == 13)
	{
		sgFindSutraCellDblClick(sgFindSutra->Columns[0],sgFindSutra->Selected);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::sgFindSutraCellDblClick(TColumn * const Column, const int Row)
{
	int iIndex = sgFindSutra->Cells[7][Row].ToInt();

	CCatalog * cbCatalog = Bookcase->CBETA->Catalog;
	String sBookID = cbCatalog->ID->Strings[iIndex];
	String sVol = cbCatalog->Vol->Strings[iIndex];
	String sSutra = cbCatalog->SutraNum->Strings[iIndex];

	String sFile = Bookcase->CBETA->CBGetFileNameBySutraNumJuan(sBookID, sVol, sSutra);
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
	if(cbSearchRange->IsChecked || cbSearchThisSutra->IsChecked) bHasRange = true;

	// ��ܥ����˯�����, �Y�Y�@�謰 0 , �h��t�@�� (�� 0 �N���ޤF)
	SetSearchEngine();

	CCatalog * Catalog = Bookcase->CBETA->Catalog;
	CSpine * Spine = Bookcase->CBETA->Spine;
	bool bFindOK = SearchEngine->Find(SearchSentence,bHasRange);      // �b���o.........................................
	clock_t t2 = clock();

	int iFoundCount = SearchEngine->FileFound->Total;

    // �q�X���X�Ӫ��T��

	lbSearchMsg->Text = u"���" + String(iFoundCount) + u"���A�@��ɶ��G" + String((t2-t1)/1000.0) + u" ��";

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
			int iVol = SearchEngine->BuildFileList->VolNum[i];

			// �o�̥i���� T220 �� 600 ��, �o�Ǧ^ T05 �Ӥ��O T07
			// ���ݧ�i�B�z ????
			if(i > 720)
			{
				int j;
				j++;
            }
			int iCatalogIndex = Catalog->FindIndexBySutraNum(sBook,iVol,sSutraNum);	// ���o TripitakaMenu ���s��

			// ���F

			// �g�W�n���� (��X��)
			String sSutraName = CMyCBUtil::CutJuanAfterSutraName(Catalog->SutraName->Strings[iCatalogIndex]);

			sgTextSearch->Cells[0][iGridIndex]=SearchEngine->FileFound->Ints[i];
			sgTextSearch->Cells[1][iGridIndex]=Catalog->ID->Strings[iCatalogIndex];
			sgTextSearch->Cells[2][iGridIndex]=Spine->VolNum->Strings[i];
			sgTextSearch->Cells[3][iGridIndex]=Catalog->SutraNum->Strings[iCatalogIndex];
			sgTextSearch->Cells[4][iGridIndex]=sSutraName;
			sgTextSearch->Cells[5][iGridIndex]=SearchEngine->BuildFileList->JuanNum[i];
			sgTextSearch->Cells[6][iGridIndex]=Catalog->Part->Strings[iCatalogIndex];
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
// �ۭq���j�M�C�� OnKeyDown
void __fastcall TfmMain::sgTextSearchKeyDown(TObject *Sender, System::Word &Key, System::WideChar &KeyChar, System::Classes::TShiftState Shift)
{
	if(Key == 13)
	{
		sgTextSearchCellDblClick(sgTextSearch->Columns[0],sgTextSearch->Selected);
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
        else cbSearchThisSutra->IsChecked = false;
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
	String sPatten = "([A-Z]+)(\\d+)n.{5}p(.{4})(.)(\\d\\d)";

	regex = new TRegEx();
	reMatch = regex->Matches(sKey, sPatten);
	if(reMatch.Count == 0)
	{
		// (CBETA, T01, no. 1, p. 23c20-21)  , �s��
		// (CBETA, T01, no. 1, p. 23, c20-21) , �ª�
		sPatten = u"([A-Z]+)(\\d+)\\s*,\\s*no\\.\\s*.+?,\\s*pp?\\.\\s*(\\S+?)(?:\\s*,\\s*)?([a-z])(\\d+)";
		reMatch = regex->Matches(sKey, sPatten);
	}

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

void __fastcall TfmMain::btOpenSimpleNavClick(TObject *Sender)
{
	btOpenSimpleNav->Cursor = crHourGlass;
	LoadNavTree(Bookcase->CBETA->Dir + Bookcase->CBETA->NavFile);
	btOpenSimpleNav->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btOpenBookNavClick(TObject *Sender)
{
	btOpenBookNav->Cursor = crHourGlass;
	LoadNavTree(Bookcase->CBETA->Dir + Bookcase->CBETA->Nav2File);
	btOpenBookNav->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btOpenBuleiNavClick(TObject *Sender)
{
	btOpenBuleiNav->Cursor = crHourGlass;
	LoadNavTree(Bookcase->CBETA->Dir + Bookcase->CBETA->NavFile);
	btOpenBuleiNav->Cursor = crDefault;
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

	TDialogService::ShowMessage(u"�ثe�w�O�Ĥ@��/�g���C");
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

	TDialogService::ShowMessage(u"�ثe�w�O�̫�@��/�g���C");
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::mmiAboutClick(TObject *Sender)
{
	fmAbout->ShowModal();
}
//---------------------------------------------------------------------------

// �ˬd���S����s�{��
void __fastcall TfmMain::CheckUpdate(bool bShowNoUpdate)
{
	// ���o��ƪ���
	String sDataVer = Bookcase->CBETA->Version;

	fmUpdate->CheckUpdate(Version, sDataVer, bShowNoUpdate);

	if(!fmUpdate->IsUpdate)    // ����s�N���n�ק��s���
	{
		String sToday = GetTodayString();
		Setting->LastUpdateChk = sToday;
		Setting->SaveToFile();
	}

/* �ª�, Ū�� update.exe, ���ΤF
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
				TDialogService::ShowMessage("SE_Err:" + AnsiString(iResult) + u" �L�k�}�Ҧ���!");
			}
		}
	}

	String sToday = GetTodayString();
	Setting->LastUpdateChk = sToday;
	Setting->SaveToFile();
#else
	TDialogService::ShowMessage(u"��p�I�ثe�u�� Windows ���~����s�\��C");
#endif
*/
}
//---------------------------------------------------------------------------
// �ˬd��s
void __fastcall TfmMain::mmiUpdateClick(TObject *Sender)
{
	CheckUpdate(true);  // true ��ܨS����s�n�q�T��
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
		btMuluWidthSwitch->Text = u"�ئ�>>";
	else
		btMuluWidthSwitch->Text = u"<<�ئ�";
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
// �󴫧�s�� URL , �������եΪ�
void __fastcall TfmMain::wmiUpdateURLClick(TObject *Sender)
{
	wmiUpdateURL->IsChecked = !wmiUpdateURL->IsChecked;
	fmUpdate->UseLocalhostURL = wmiUpdateURL->IsChecked;
}
//---------------------------------------------------------------------------
// ��q���� HTML , �D�n�O�Ψ��ˬd���~
void __fastcall TfmMain::wmiCreateHtmlClick(TObject *Sender)
{
    fmCreateHtml->ShowModal();
}
//---------------------------------------------------------------------------
// �إ߯�����
void __fastcall TfmMain::wmiBuildIndexClick(TObject *Sender)
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
// �N�Ҧ��� Default ����
void __fastcall TfmMain::CancelAllDefault()
{
	btFindSutra->Default = false;
	btGoSutra->Default = false;
	btGoBook->Default = false;
	btGoByKeyword->Default = false;
	btTextSearch->Default = false;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::edFindSutra_VolFromEnter(TObject *Sender)
{
	CancelAllDefault();
	btFindSutra->Default = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::edGoSutra_SutraNumEnter(TObject *Sender)
{
	CancelAllDefault();
	btGoSutra->Default = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::edGoBook_VolEnter(TObject *Sender)
{
	CancelAllDefault();
	btGoBook->Default = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::edGoByKeywordEnter(TObject *Sender)
{
	CancelAllDefault();
	btGoByKeyword->Default = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::edTextSearchEnter(TObject *Sender)
{
	CancelAllDefault();
	btTextSearch->Default = true;
}
//---------------------------------------------------------------------------
// �˯����g
void __fastcall TfmMain::cbSearchThisSutraChange(TObject *Sender)
{
	if(cbSearchThisSutra->IsChecked)
	{
		// �]�w�˯����g
		if(SpineID == -1)
		{
			cbSearchThisSutra->IsChecked = false;
			return;
		}

		if(cbSearchRange->Visible)
			cbSearchRange->IsChecked = false;

		// ���X���g
		String sThisBookId = Bookcase->CBETA->Spine->BookID->Strings[SpineID];
		String sThisSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];

		if(Bookcase->CBETA->SearchEngine_CB)
			Bookcase->CBETA->SearchEngine_CB->BuildFileList->NoneSearch();
		if(Bookcase->CBETA->SearchEngine_orig)
			Bookcase->CBETA->SearchEngine_orig->BuildFileList->NoneSearch();

		if(Bookcase->CBETA->SearchEngine_CB)
			Bookcase->CBETA->SearchEngine_CB->BuildFileList->SearchThisSutra(sThisBookId,sThisSutra);
        if(Bookcase->CBETA->SearchEngine_orig)
			Bookcase->CBETA->SearchEngine_orig->BuildFileList->SearchThisSutra(sThisBookId,sThisSutra);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wmiOnlineDocClick(TObject *Sender)
{
#ifdef _Windows
	String sURL = u"http://www.cbeta.org/cbreader/doc/";
	ShellExecute(0,L"open",sURL.c_str(),L"",L"", SW_SHOW);
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wmiOnlineFAQClick(TObject *Sender)
{
#ifdef _Windows
	String sURL = u"http://www.cbeta.org/CBReader2X_FAQ.php";
	ShellExecute(0,L"open",sURL.c_str(),L"",L"", SW_SHOW);
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::mmiOnlineDocClick(TObject *Sender)
{
#ifdef __APPLE__
	String sURL = u"http://www.cbeta.org/cbreader/doc/";
	system(AnsiString("open " + AnsiString(sURL)).c_str());
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::mmiOnlineFAQClick(TObject *Sender)
{
#ifdef __APPLE__
	String sURL = u"http://www.cbeta.org/CBReader2X_FAQ.php";
	system(AnsiString("open " + AnsiString(sURL)).c_str());
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btBooleanClick(TObject *Sender)
{
	TPointF FP;
	// Initialize the coordinates to the origin of the button control.
	FP.X = 0;
	FP.Y = 0;
	// Transposes the coordinates in the context of the form.
	FP = btBoolean->LocalToAbsolute(FP);
	// Transposes the coordinates in the context of the screen.
	FP = ClientToScreen(FP);
	// Display the popup menu at the computed coordinates.
	pmBoolean->Popup(FP.X, FP.Y + btBoolean->Height + 10);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::miNearClick(TObject *Sender)
{
	edTextSearch->Text = edTextSearch->Text + u"+";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::miOrClick(TObject *Sender)
{
	edTextSearch->Text = edTextSearch->Text + u",";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::miExcludeClick(TObject *Sender)
{
	edTextSearch->Text = edTextSearch->Text + u"-";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::miAndClick(TObject *Sender)
{
	edTextSearch->Text = edTextSearch->Text + u"&";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::miBeforeClick(TObject *Sender)
{
	edTextSearch->Text = edTextSearch->Text + u"*";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btCopyClick(TObject *Sender)
{
	try
	{
		WebBrowser->EvaluateJavaScript("document.execCommand('copy');");
	}
	catch(...)
	{
		TDialogService::ShowMessage("�ƻs�����D");
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btCiteCopyClick(TObject *Sender)
{
	try
	{
		WebBrowser->EvaluateJavaScript("CBCopy.go()");
	}
	catch(...)
	{
		try
		{
			WebBrowser->EvaluateJavaScript("document.execCommand('copy');");
		}
		catch(...)
		{
			TDialogService::ShowMessage("�ޥνƻs�����D");
		}

	}
}
//---------------------------------------------------------------------------

