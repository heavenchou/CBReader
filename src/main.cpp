// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
#include "selectbook.h"
#include "searchrange.h"
#include "buildindex.h"

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
	InitialPath();  // 設定目錄初值

#ifdef _Windows
	SetPermissions(11001); // 將 IE 設定到 IE 11 (如果沒 IE 11 的如何?)
#endif

	SearchSentence = "";    // 搜尋字串
	SearchWordList = new TStringList;	    // 存放每一個檢索的詞, 日後塗色會用到

	// 取得設定檔並讀取所有設定
	Setting = new CSetting(SettingFile);

	SelectedBook = -1;   // 目前選中的書, -1 表示還沒選

	// 取得 Bookcase 所有資料區
	// 載入書櫃

	Bookcase = new CBookcase();
	String sBookcasePath = MyFullPath + Setting->BookcaseDir;
	if(!TDirectory::Exists(sBookcasePath))
	{
        // 使用指定目錄 ???? 該改為使用者指定目錄才好
		sBookcasePath = u"d:\\Dropbox\\CBReader2X\\Bookcase";
	}
	Bookcase->LoadBookcase(sBookcasePath);

	// 在書櫃選擇叢書
	int iBookcaseCount = Bookcase->Count();
	if(iBookcaseCount == 0)
	{
		ShowMessage(u"書櫃中一本書都沒有");
    }
	else if(iBookcaseCount == 1)
	{
		// 只有一本書就直接開了
		// OpenBookcase(0); // ???? 暫時取消, 這一版要直接開啟 CBETA
	}
	OpenCBETABook();    // ???? 取消上面, 因為這一版要直接開啟 CBETA

	MuluTree = 0;

    // 這要先處理, 動一下內容, 不然欄位標題就還可以移動
	sgTextSearch->RowCount = 1;
	sgFindSutra->RowCount = 1;

	lbSearchMsg->Text = ""; // 清空搜尋訊息
	btOpenBookcase->Visible = false;
	btBuildIndex->Visible = false;
	SpineID = -1;	// 初值表示沒開啟
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
// 	路徑初值設定
void __fastcall TfmMain::InitialPath()
{
	// 程式主目錄
#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	// MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012";
	// MyFullPath = GetCurrentDir();
	// MyFullPath = StringReplace(MyFullPath, "/CBReader.app/Contents/MacOS", "", TReplaceFlags() << rfReplaceAll);

	MyFullPath = System::Ioutils::TPath::GetHomePath();
	MyFullPath += "/Desktop";

#endif
	MyFullPath += "/";

	// Temp 目錄
	MyTempPath = System::Ioutils::TPath::GetTempPath();
	MyTempPath = MyTempPath + "CBReader/";

	if(!TDirectory::Exists(MyTempPath))
		TDirectory::CreateDirectory(MyTempPath);

	// 使用者個人目錄
	MyHomePath = System::Ioutils::TPath::GetHomePath();
	MyHomePath += "/CBETA/";
	if(!TDirectory::Exists(MyHomePath))
		TDirectory::CreateDirectory(MyHomePath);
	MyHomePath += "CBReader2X/";
	if(!TDirectory::Exists(MyHomePath))
		TDirectory::CreateDirectory(MyHomePath);

	// 設定檔
	SettingFile = MyHomePath + u"cbreader.ini";
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
// 開啟CBETA書櫃
void __fastcall TfmMain::OpenCBETABook()
{
	for(int i=0; i<Bookcase->Count(); i++)
	{
		if(Bookcase->Books->Items[i] == Bookcase->CBETA)
		{
			// 找到 CBETA 了
			OpenBookcase(i);
			return;
		}
	}
	ShowMessage("找不到 CBETA 資料");
    return;
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
	// ???? 這行取巧, 日後要拿掉
	// 因為從單經書目點選時, 有時沒有開啟 SelectedBook
	if(SelectedBook <0) SelectedBook = 0;
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
		LoadNavTree(sSeries->Dir + sURL);
	}
	// CBETA 經文
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


// 是否有選擇套書了?
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

			// 找藏經
			if(!sBook.IsEmpty())
				if(Catalog->ID->Strings[i] != sBook)
					continue;

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
				if(Catalog->SutraName->Strings[i].Pos0(sSutraName) < 0)
					continue;
			// 找譯者
			if(!sByline.IsEmpty())
				if(Catalog->Byline->Strings[i].Pos0(sByline) < 0)
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
// 載入 XML 並處理成網頁
void __fastcall TfmMain::ShowCBXML(String sFile, bool bShowHighlight, TmyMonster * seSearchEngine)
{
	if(sFile == "")
	{
		ShowMessage("沒有找到正確檔案");
        return;
	}

	// 如果傳來的是 XML/T/T01/T01n0001_001.xml#p0001a01
	// 則要把 p0001a01 分離出來

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

	// 找出 spine id
	if(!Bookcase->CBETA->Spine->Files->Find(sFile, SpineID))
	{
		SpineID = -1;	// 表示沒找到
	}

	String sOutFile = sFile + ".htm";
	sOutFile = StringReplace(sOutFile, "/", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = StringReplace(sOutFile, "\\", "_", TReplaceFlags() << rfReplaceAll);
	sOutFile = MyTempPath + sOutFile;

	CBXML->SaveToHTML(sOutFile);

	WebBrowser->URL = "file://" + sOutFile;
	WebBrowser->Navigate();

	// 產生目錄

	String sMulu = StringReplace(sFile, "XML", "toc", TReplaceFlags() << rfReplaceAll);
	int iLen = sMulu.Length();
	sMulu = sMulu.SubString0(0,iLen-8); // 扣掉最後的 _001.xml

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
}
//---------------------------------------------------------------------------
// 由冊頁欄行呈現經文
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
	bool bHasRange = false;     // 有範圍就要設定
	if(cbSearchRange->IsChecked) bHasRange = true;

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
void __fastcall TfmMain::cbSearchRangeChange(TObject *Sender)
{
	if(cbSearchRange->IsChecked)
	{
		// 設定檢索範圍
		TModalResult mr = fmSearchRange->ShowModal();
		if(mr == mrCancel) cbSearchRange->IsChecked = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btGoByKeywordClick(TObject *Sender)
{
	// 判斷各種直接前往指定經文的方法
	// 1. 行首 T01n0001_p0001a01
	// 2. 引用複製 T01,no.1,p.1,b5
	// 3. 特定的代碼, 例如 : SN1.1

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
// 建立索引檔
void __fastcall TfmMain::btBuildIndexClick(TObject *Sender)
{
	// 先關掉全文檢索引擎

	Bookcase->CBETA->FreeSearchEngine();

	// 產生索引

	fmBuildIndex->edBuildListDir->Text = Bookcase->CBETA->Dir;
	fmBuildIndex->edBuildList->Text = Bookcase->CBETA->Dir + Bookcase->CBETA->SpineFile;
	fmBuildIndex->edWordIndex->Text = Bookcase->CBETA->Dir + "index/wordindex.ndx";
	fmBuildIndex->edMainIndex->Text = Bookcase->CBETA->Dir + "index/main.ndx";
	fmBuildIndex->ShowModal();

	// 重新建立全文檢索引擎

	Bookcase->CBETA->LoadSearchEngine();
}
//---------------------------------------------------------------------------
// 將檔案載入導覽樹
void __fastcall TfmMain::LoadNavTree(String sFile)
{
	if(NavTree != 0 && NavTree->XMLFile == sFile) return;

	if(NavTree) delete NavTree;
	NavTree = new CNavTree(sFile);
	NavTree->SaveToTreeView(tvNavTree, NavTreeItemClick);
}
//---------------------------------------------------------------------------
// 將檔案載入目錄樹
void __fastcall TfmMain::LoadMuluTree(String sFile)
{
	if(MuluTree != 0 && MuluTree->XMLFile == sFile) return;

	if(MuluTree) delete MuluTree;
	MuluTree = new CNavTree(sFile);
	MuluTree->SaveToTreeView(tvMuluTree, NavTreeItemClick);
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
// 上一卷
void __fastcall TfmMain::btPrevJuanClick(TObject *Sender)
{
	if(SpineID > 0)
	{
		String sFile = Bookcase->CBETA->Spine->CBGetFileNameBySpineIndex(SpineID-1);
		ShowCBXML(sFile);
	}
}
//---------------------------------------------------------------------------
// 下一卷
void __fastcall TfmMain::btNextJuanClick(TObject *Sender)
{
	if(SpineID + 1 < Bookcase->CBETA->Spine->Files->Count)
	{
		String sFile = Bookcase->CBETA->Spine->CBGetFileNameBySpineIndex(SpineID+1);
		ShowCBXML(sFile);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::MenuItem1Click(TObject *Sender)
{
    ShowMessage("CBETA CBReader 2X");
}
//---------------------------------------------------------------------------

