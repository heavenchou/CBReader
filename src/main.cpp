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
	SearchSentence = "";    // 搜尋字串
	SearchWordList = new TStringList;	    // 存放每一個檢索的詞, 日後塗色會用到

	// 取得設定檔並讀取所有設定

	// 取得 Bookcase 所有資料區

#ifdef _Windows
	SetPermissions(11001); // 將 IE 設定到 IE 11 (如果沒 IE 11 的如何?)
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

	SelectedBook = -1;   // 目前選中的書, -1 表示還沒選

	// 載入書櫃

	Bookcase = new CBookcase();
	Bookcase->LoadBookcase(MyFullPath + Setting->BookcaseDir);

	// 在書櫃選擇叢書
	int iBookcaseCount = Bookcase->Count();
	if(iBookcaseCount == 0)
	{
		ShowMessage(u"書櫃中一本書都沒有");
    }
	else if(iBookcaseCount == 1)
	{
		// 只有一本書就直接開了
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
// 將 IE 設定為 IE 11
// copy from
// http://docwiki.embarcadero.com/Libraries/Tokyo/en/FMX.WebBrowser.TWebBrowser
//
// 參考 MSDN
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
// 開啟指定的書櫃
void __fastcall TfmMain::OpenBookcase(int iID)
{
	if(iID == SelectedBook) return;	// 同一本, 不要重開
	if(iID == -1) return;   	// 沒選書

	SelectedBook = iID;

	// 載入叢書的起始目錄
	if(NavTree) delete NavTree;
	CSeries * s = (CSeries *) Bookcase->Books->Items[iID];
	NavTree = new CNavTree(s->Dir + s->NavFile);
	NavTree->SaveToTreeView(tvNavTree, NavTreeItemClick);
}

//---------------------------------------------------------------------------
// NavTree Item 點二下的作用
// Item->TagString 儲存 URL
// Item->Tag 儲存 Type
// Item->TagObject 儲存 SNavItem
void __fastcall TfmMain::NavTreeItemClick(TObject *Sender)
{
	// Item
	TTreeViewItem * tvItem = (TTreeViewItem *) Sender;
	String sURL = tvItem->TagString;
	CSeries * sSeries = (CSeries *)Bookcase->Books->Items[SelectedBook];

	if(sURL == "")  // 沒有 URL
	{
		// 如果有子層, 就切換展開或閉合狀態
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

	// 一般連結
	if(iType == nit_NormalLink)
	{
		if(sURL.SubString(1,4) == "http")
			WebBrowser->URL = sURL;
		else
			WebBrowser->URL = "file://" + sSeries->Dir + sURL;
		WebBrowser->Navigate();
	}
	// 目錄連結
	else if(iType == nit_NavLink)
	{
		if(NavTree) delete NavTree;     // 這部份應該物件化 ???
		NavTree = new CNavTree(sSeries->Dir + sURL);
		NavTree->SaveToTreeView(tvNavTree, NavTreeItemClick);
	}
	// CBETA 經文
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
// 是否有選擇套書了?
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

	// 先用 CBETA 版 ???
	// 逐一搜尋目錄
	//if(IsSelectedBook())
	{
		// 先用 CBETA 版 ???
		// CSeries * Series = (CSeries *) Bookcase->Books->Items[SelectedBook];
		CSeries * Series = Bookcase->CBETA;

		CCatalog * Catalog = Series->Catalog;
		int iCount = Catalog->ID->Count;
		// 逐一檢查
		sgFindSutra->BeginUpdate();
		int iGridIndex = 0;
		sgFindSutra->RowCount = 10;
		for(int i=0; i<iCount; i++)
		{
			bool bFound = true;

			// 找冊數
			if(!sVolFrom.IsEmpty())
				if(Catalog->Vol->Strings[i].ToInt() < sVolFrom.ToInt())
					continue;
			// 找冊數
			if(!sVolTo.IsEmpty())
				if(Catalog->Vol->Strings[i].ToInt() > sVolTo.ToInt())
					continue;
			// 找經號
			if(!sSutraFrom.IsEmpty())
			{
				// 經號標準化
				sSutraFrom = Series->Spine->CBGetSutraNumFormat(sSutraFrom);
				String sSutra = Series->Spine->CBGetSutraNumFormat(Catalog->SutraNum->Strings[i]);
				sSutraFrom = sSutraFrom.LowerCase();
				sSutra = sSutra.LowerCase();
				if(sSutra < sSutraFrom)
					continue;
			}
			// 找經號
			if(!sSutraTo.IsEmpty())
			{
				// 經號標準化
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
			// 找經名
			if(!sSutraName.IsEmpty())
				if(Catalog->SutraName->Strings[i].Pos(sSutraName) <= 0)
					continue;
			// 找譯者
			if(!sByline.IsEmpty())
				if(Catalog->Byline->Strings[i].Pos(sByline) <= 0)
					continue;

			// 找到了

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
// 由經卷頁欄行呈現經文
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
// 載入 XML 並處理成網頁
void __fastcall TfmMain::ShowCBXML(String sFile, bool bShowHighlight, TmyMonster * seSearchEngine)
{
	if(sFile == "")
	{
		ShowMessage("沒有找到正確檔案");
        return;
    }
	String sXMLFile = Bookcase->CBETA->Dir + sFile;
	String sJSFile = Bookcase->CBETA->Dir + Bookcase->CBETA->JSFile;
	CCBXML * CBXML = new CCBXML(sXMLFile, Setting, sJSFile, bShowHighlight, seSearchEngine);

	// 先不用, 因為 mac os 產生出來的檔名是 /var/tmp/xxxxx
	// windows 是 xxxxxx
	// 所以日後還是自己寫吧
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
// 由冊頁欄行呈現經文
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
	SetPermissions(7000); // 設定為 IE 7
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btTextSearchClick(TObject *Sender)
{
	SearchSentence = edTextSearch->Text;

	// 去除頭尾的萬用字元
	SearchSentence = CMyStrUtil::Trim(SearchSentence, u'?');

	if(SearchSentence == "") return;    // 沒輸入

	//RememberWord(cbSearchWord);		// 將查詢的字存起來
	//UpdateSearchHistory = true;

	//SearchSentenceOrig = cbSearchWord->Text;        // 最原始的檢索句字, 可能包含 unicode


	// 改變滑鼠
	TCursor csOldCursor = Cursor;
	Cursor = crHourGlass;

	clock_t t1 = clock();
	bool bHasRange = false;     // 有範圍就要設定 ????

	TmyMonster * SearchEngine = Bookcase->CBETA->SearchEngine;
	CCatalog * Catalog = Bookcase->CBETA->Catalog;
	CSpine * Spine = Bookcase->CBETA->Spine;
	bool bFindOK = SearchEngine->Find(SearchSentence,bHasRange);      // 在找囉.........................................
	clock_t t2 = clock();

	int iFoundCount = SearchEngine->FileFound->Total;

    // 秀出找到幾個的訊息

	lbSearchMsg->Text = u"找到" + String(iFoundCount) + u"筆，共花時間：" + String(t2-t1);

    int iTotalSearchFileNum = 0;
    bool bShowAll = false;
    int iMaxSearchNum = 0;

    if(bFindOK)
    {
		SearchWordList->Clear();
		for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
			SearchWordList->Add(SearchEngine->SearchWordList->Strings[i]);	// 存起查詢的詞

        // 先檢查有沒有超過限制

        for (int i=0; i<BuildFileList->FileCount; i++)
    	{
	    	if(SearchEngine->FileFound->Ints[i])
		    {
			    iTotalSearchFileNum++;
            }
        }

        // 將結果放入 list 列表中
	}

	sgTextSearch->BeginUpdate();
	int iGridIndex = 0;
	sgTextSearch->RowCount = 10;

	for (int i=0; i<BuildFileList->FileCount; i++)
	{
		// 找到了
        if(SearchEngine->FileFound->Ints[i])
		{
			String sSutraNum  = BuildFileList->SutraNum[i];		// 取得經號
			String sBook = BuildFileList->Book[i];

			// 這裡可能找到 T220 第 600 卷, 卻傳回 T05 而不是 T07
			// 有待改進處理 ????
			int iCatalogIndex = Catalog->FindIndexBySutraNum(sBook,sSutraNum);	// 取得 TripitakaMenu 的編號

			// 找到了

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

	// 還原滑鼠
	Cursor=csOldCursor;

    if(bFindOK)
    {
		if(sgTextSearch->RowCount == 0)
	    {
			ShowMessage(u"找不到任何資料");
    	}
    }
    else
	{
		ShowMessage(u"查詢字串語法有問題，請再檢查看看。");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::sgTextSearchCellDblClick(TColumn * const Column, const int Row)

{
	int iIndex = sgTextSearch->Cells[8][Row].ToInt();

	String sFile = Bookcase->CBETA->Spine->Files->Strings[iIndex];
    // 要塗色
	ShowCBXML(sFile, true, Bookcase->CBETA->SearchEngine);
}
//---------------------------------------------------------------------------

