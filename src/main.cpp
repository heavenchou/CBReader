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
	// 更新版本注意事項, 要改底下, 還有 project 的版本
    // 還有 fmAbout 的版本與日期資料
	Application->Title = u"CBReader";
	ProgramTitle = u"CBETA 電子佛典 2018";
	Version = u"0.4.2.0";   // 末位 .1 是全西蓮, .2 是西蓮+CBETA
	DebugString = u"Heaven";     // debug 口令
	IsDebug = false;           // debug 變數

	// 西蓮淨苑 SLReader 專用
	//Application->Title = u"SLReader";

	bool IsAllSL = true;    // 是否全部都是西蓮?
	if(*Version.LastChar() != '1') IsAllSL = false;
	if(Application->Title == u"SLReader")
	{
		if(IsAllSL) ProgramTitle = u"西蓮淨苑文獻集成";
		Caption = ProgramTitle;

		if(IsAllSL)
		{
			cbFindSutra_BookId->Items->Clear();
			cbFindSutra_BookId->Items->Add(u"全部");
		}
		cbFindSutra_BookId->Items->Add(u"DA 道安法師著作全集");
		cbFindSutra_BookId->Items->Add(u"ZY 智諭法師著作全集");
		cbFindSutra_BookId->Items->Add(u"HM 惠敏法師蓮風集");
		cbFindSutra_BookId->ItemIndex = 0;

		if(IsAllSL)
		{
			cbGoSutra_BookId->Items->Clear();
			cbGoSutra_BookId->Items->Add(u"全部");
		}
		cbGoSutra_BookId->Items->Add(u"DA 道安法師著作全集");
		cbGoSutra_BookId->Items->Add(u"ZY 智諭法師著作全集");
		cbGoSutra_BookId->Items->Add(u"HM 惠敏法師蓮風集");
		cbGoSutra_BookId->ItemIndex = 0;

		if(IsAllSL)
		{
			cbGoBook_BookId->Items->Clear();
			cbGoBook_BookId->Items->Add(u"全部");
		}
		cbGoBook_BookId->Items->Add(u"DA 道安法師著作全集");
		cbGoBook_BookId->Items->Add(u"ZY 智諭法師著作全集");
		cbGoBook_BookId->Items->Add(u"HM 惠敏法師蓮風集");
		cbGoBook_BookId->ItemIndex = 0;

		if(IsAllSL)
		{
			Panel8->Visible = false;
			cbSearchRange->Visible = false;

			lbFindSutra_Book->Text = u"叢書";
			lbGoSutra_Book->Text = u"叢書";
			lbGoBook_Book->Text = u"叢書";

			lbFindSutra_SutraNum->Text = u"編號從";
			lbGoSutra_SutraNum->Text = u"編號";

			lbFindSutra_SutraName->Text = u"著作";

			sgFindSutra->Columns[0]->Header = u"著作";
			sgFindSutra->Columns[1]->Header = u"叢書";
			sgFindSutra->Columns[3]->Width = 1;
			sgFindSutra->Columns[4]->Header = u"編號";

			sgTextSearch->Columns[1]->Header = u"著作";
			sgTextSearch->Columns[2]->Header = u"叢書";
			sgTextSearch->Columns[4]->Width = 1;
			sgTextSearch->Columns[5]->Header = u"編號";
		}
	}

#ifdef _Windows
	MainMenu->Free();
#else
    MenuBar->Free();
#endif

	// 設定目錄初值
	InitialPath();

#ifdef _Windows
	SetPermissions(11001); // 將 IE 設定到 IE 11 (如果沒 IE 11 的如何?)
	// 刪去舊版
	String sOld = ParamStr(0) + u".tmp";
	if(TFile::Exists(sOld))
        TFile::Delete(sOld);
#endif

	SearchEngine = 0;   // 全文檢索引擎
	SearchSentence = "";    // 搜尋字串
	SearchWordList = new TStringList;	    // 存放每一個檢索的詞, 日後塗色會用到

	// 取得設定檔並讀取所有設定
	Setting = new CSetting(SettingFile);

	// 初始畫面的設定
	btOpenBookcase->Visible = false;
	btOpenSimpleNav->Visible = false;
	btOpenBookNav->Visible = false;
	btCopy->Visible = false;
	btCiteCopy->Visible = false;

	// 因為下拉選單寬度不能為奇數, 所以要調整
	if(Floor(cbFindSutra_BookId->Width) % 2 == 1) cbFindSutra_BookId->Width -= 1;
	if(Floor(cbGoSutra_BookId->Width) % 2 == 1) cbGoSutra_BookId->Width -= 1;
	if(Floor(cbGoBook_BookId->Width) % 2 == 1) cbGoBook_BookId->Width -= 1;

	#ifdef _Windows
	wmiDebug->Visible = false;
	#endif

	tcMainFunc->TabIndex = 0;
	SelectedBook = -1;   // 目前選中的書, -1 表示還沒選
    pnMulu->Width = 0;  // 書目先縮到最小
	MuluWidth = 200;    // 初始書目寬度

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
// ---------------------------------------------------------------------------
// 	路徑初值設定
void __fastcall TfmMain::InitialPath()
{
	// 程式主目錄
#ifdef _Windows
	MyFullPath = ExtractFilePath(ParamStr(0));
#else
	MyFullPath = System::Ioutils::TPath::GetHomePath();
#endif
	MyFullPath = IncludeTrailingPathDelimiter(MyFullPath);

	// Temp 目錄

	MyTempPath = System::Ioutils::TPath::GetTempPath();
	MyTempPath = IncludeTrailingPathDelimiter(MyTempPath + u"CBReader");

	if(!TDirectory::Exists(MyTempPath))
		TDirectory::CreateDirectory(MyTempPath);

	// 使用者個人目錄

	MyHomePath = System::Ioutils::TPath::GetHomePath();
	MyHomePath = IncludeTrailingPathDelimiter(MyHomePath);

	// 設定檔目錄

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
	// 設定檔

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

	// 檢查更新
	String sToday = GetTodayString();
	if(sToday != Setting->LastUpdateChk)
		CheckUpdate();   // 檢查更新
}
//---------------------------------------------------------------------------
// 初始資料
void __fastcall TfmMain::InitialData()
{
	/* 不用了, Windows 已經確定能得到使用者目錄了
	#ifdef _Windows

	// 檢查 MyFullPath 有沒有錯誤
	if(!TFile::Exists(String(MyFullPath + u"CBReader.exe")))
	{
		// 沒看到 CBReader.exe , 可能有問題

		bool bFound = false;
		String sMyFullPath = u"";

		// 檢查設定中有沒有 MyFullPath , 檢查正不正確
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
			// 都找不到目前目錄, 要詢問使用者了
			TDialogService::ShowMessage(u"沒有找到您的 CBReader.exe 所在目錄，請手動選擇本程式所在目錄位置。");
			SelectDirectory(u"選擇 CBReader.exe 所在目錄位置",MyFullPath,sMyFullPath);

            if(TFile::Exists(String(sMyFullPath + u"CBReader.exe")))
			{
				bFound = true;
			}
        }

		// 至此應該都找到了
		MyFullPath = sMyFullPath;
		if(Setting->MyFullPath != MyFullPath)
		{
			// 將 MyFullPath 寫入
			Setting->MyFullPath = MyFullPath;
			Setting->SaveToFile();
		}
	}
	#endif
	*/

	// 取得 Bookcase 所有資料區
	// 載入書櫃

	Bookcase = new CBookcase();

	String sBookcasePath = u"";

	// Bookcase 目錄處理原則
	// 1. Windows 在主程式所在目錄底下去找 Bookcase 子目錄
	// 2. Mac 有二個優先順序
	//    3.1 /User/xxx/Library/CBETA/Bookcase
	//    3.2 /Library/CBETA/Bookcase
	// 3. 上面若沒有, 則找 Setting->BookcaseFullDir
	// 4. 以上若都沒有, 則由使用者尋找, 找到後存在 Setting->BookcaseFullDir


#ifdef _Windows
	sBookcasePath = MyFullPath + Setting->BookcasePath;
#else
	// Mac 第一優先
	sBookcasePath = MyHomePath + u"Library/CBETA/" + Setting->BookcasePath;
	if(!TDirectory::Exists(sBookcasePath))
	{
		// Mac 第二優先
		sBookcasePath = u"/Library/CBETA/" + Setting->BookcasePath;
	}
#endif

	// 都沒有就查設定
	if(!TDirectory::Exists(sBookcasePath))
	{
		if(Setting->BookcaseFullPath != u"")
		{
			sBookcasePath = Setting->BookcaseFullPath;
		}
	}

	// 都沒有就詢問使用者
	if(!TDirectory::Exists(sBookcasePath))
	{
		TDialogService::ShowMessage(u"沒有找到您的 Bookcase 書櫃目錄，請手動選擇目錄所在位置。");
		// 使用指定目錄
		SelectDirectory(u"選擇 Bookcase 目錄所在位置", MyFullPath, sBookcasePath);

		Setting->BookcaseFullPath = sBookcasePath;
		Setting->SaveToFile();
	}

	Bookcase->LoadBookcase(sBookcasePath);

	// 在書櫃選擇叢書
	int iBookcaseCount = Bookcase->Count();
	if(iBookcaseCount == 0)
	{
		TDialogService::ShowMessage(u"書櫃中一本書都沒有");
	}
	// else if(iBookcaseCount == 1)
	else
	{
		// 只有一本書就直接開了
		// OpenBookcase(0); // ???? 暫時取消, 這一版要直接開啟 CBETA
		OpenCBETABook();    // ???? 取消上面, 因為這一版要直接開啟 CBETA
	}

	MuluTree = 0;

    // 這要先處理, 動一下內容, 不然欄位標題就還可以移動
	sgTextSearch->RowCount = 1;
	sgFindSutra->RowCount = 1;

	lbSearchMsg->Text = ""; // 清空搜尋訊息
	SpineID = -1;	// 初值表示沒開啟

	if(iBookcaseCount != 0)
	{
		WebBrowser->URL = "file://" + Bookcase->CBETA->Dir + u"help/index.htm";
		WebBrowser->Navigate();
	}

	// 西蓮淨苑 SLReader 專用
	// 檢索範圍要加上西蓮
	if(Application->Title == u"SLReader")
	{
    	// 版本末碼為 1 的表示全部西蓮, 就不用做檢索範圍了
		if(*Version.LastChar() != '1')
		{
			TTreeViewItem * newItem1 = new TTreeViewItem(fmSearchRange->tvBook);
			newItem1->Text = u"DA 道安法師著作全集";   // 標題
			fmSearchRange->tvBook->AddObject(newItem1);

			TTreeViewItem * newItem2 = new TTreeViewItem(fmSearchRange->tvBook);
			newItem2->Text = u"ZY 智諭法師著作全集";   // 標題
			fmSearchRange->tvBook->AddObject(newItem2);

			TTreeViewItem * newItem3 = new TTreeViewItem(fmSearchRange->tvBook);
			newItem3->Text = u"HM 惠敏法師蓮風集";   // 標題
			fmSearchRange->tvBook->AddObject(newItem3);
		}
	}
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
	TDialogService::ShowMessage(u"找不到 CBETA 資料");
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
	tvNavTree->Cursor = crHourGlass;

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
	tvNavTree->Cursor = crDefault;
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

		lbFindSutraCount->Text = u"共找到 " + String(iGridIndex) + u" 筆";
		if(iGridIndex == 0)
            TDialogService::ShowMessage(u"沒有滿足此條件的資料");
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
// 載入 XML 並處理成網頁
void __fastcall TfmMain::ShowCBXML(String sFile, bool bShowHighlight, TmyMonster * seSearchEngine)
{
	if(sFile == "")
	{
		TDialogService::ShowMessage(u"沒有找到正確檔案");
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

	// 找出 spine id , -1 表示沒找到
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

	// 產生目錄

	String sMulu = StringReplace(sFile, "XML", "toc", TReplaceFlags() << rfReplaceAll);
	int iLen = sMulu.Length();
	sMulu = sMulu.SubString0(0,iLen-8); // 扣掉最後的 _001.xml

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

	// 更改 form 的 title

	if(SpineID >= 0)
	{
		String sBook = Bookcase->CBETA->Spine->BookID->Strings[SpineID];
		String sVol = Bookcase->CBETA->Spine->Vol->Strings[SpineID];
		String sVolNum = Bookcase->CBETA->Spine->VolNum->Strings[SpineID];
		String sSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];
		String sJuan = Bookcase->CBETA->Spine->Juan->Strings[SpineID];
		int iIndex = Bookcase->CBETA->Catalog->FindIndexBySutraNum(sBook, sVolNum, sSutra);
		String sName = Bookcase->CBETA->Catalog->SutraName->Strings[iIndex];

		// 經名移除 (第X卷-第x卷)
		sName = CMyCBUtil::CutJuanAfterSutraName(sName);
		sJuan = CMyStrUtil::TrimLeft(sJuan, u'0');
		sSutra = CMyStrUtil::TrimLeft(sSutra, u'0');
		String sCaption = ProgramTitle + u"《" + sName + u"》"
				+ sVol + u", No. " + sSutra + u", 卷/篇章" + sJuan;
		Caption = sCaption;

		// 將經名後面的 （上中下一二三......十）移除
		sName = CMyCBUtil::CutNumberAfterSutraName(sName);
		cbSearchThisSutra->Text = u"檢索本經：" + sName;
        cbSearchThisSutraChange(this);  // 設定檢索本經的相關資料
	}

	// 檢索本經

	cbSearchThisSutra->Enabled = true;

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
// 自訂的搜尋列表 OnKeyDown
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
	if(cbSearchRange->IsChecked || cbSearchThisSutra->IsChecked) bHasRange = true;

	// 選擇全文檢索引擎, 若某一方為 0 , 則選另一方 (全 0 就不管了)
	SetSearchEngine();

	CCatalog * Catalog = Bookcase->CBETA->Catalog;
	CSpine * Spine = Bookcase->CBETA->Spine;
	bool bFindOK = SearchEngine->Find(SearchSentence,bHasRange);      // 在找囉.........................................
	clock_t t2 = clock();

	int iFoundCount = SearchEngine->FileFound->Total;

    // 秀出找到幾個的訊息

	lbSearchMsg->Text = u"找到" + String(iFoundCount) + u"筆，共花時間：" + String((t2-t1)/1000.0) + u" 秒";

    int iTotalSearchFileNum = 0;
    bool bShowAll = false;
    int iMaxSearchNum = 0;

    if(bFindOK)
    {
		SearchWordList->Clear();
		for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
			SearchWordList->Add(SearchEngine->SearchWordList->Strings[i]);	// 存起查詢的詞

        // 先檢查有沒有超過限制

		for (int i=0; i<SearchEngine->BuildFileList->FileCount; i++)
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

	for (int i=0; i<SearchEngine->BuildFileList->FileCount; i++)
	{
		// 找到了
        if(SearchEngine->FileFound->Ints[i])
		{
			String sSutraNum  = SearchEngine->BuildFileList->SutraNum[i];		// 取得經號
			String sBook = SearchEngine->BuildFileList->Book[i];
			int iVol = SearchEngine->BuildFileList->VolNum[i];

			// 這裡可能找到 T220 第 600 卷, 卻傳回 T05 而不是 T07
			// 有待改進處理 ????
			if(i > 720)
			{
				int j;
				j++;
            }
			int iCatalogIndex = Catalog->FindIndexBySutraNum(sBook,iVol,sSutraNum);	// 取得 TripitakaMenu 的編號

			// 找到了

			// 經名要移除 (第X卷)
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

	// 還原滑鼠
	Cursor=csOldCursor;

    if(bFindOK)
    {
		if(sgTextSearch->RowCount == 0)
	    {
			TDialogService::ShowMessage(u"找不到任何資料");
    	}
    }
    else
	{
		TDialogService::ShowMessage(u"查詢字串語法有問題，請再檢查看看。");
	}
}
//---------------------------------------------------------------------------
// 自訂的搜尋列表 OnKeyDown
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
    // 要塗色
	ShowCBXML(sFile, true, SearchEngine);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::cbSearchRangeChange(TObject *Sender)
{
	if(cbSearchRange->IsChecked)
	{
		// 設定檢索範圍
		TModalResult mr = fmSearchRange->ShowModal();
		if(mr == mrCancel) cbSearchRange->IsChecked = false;
        else cbSearchThisSutra->IsChecked = false;
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
	String sPatten = "([A-Z]+)(\\d+)n.{5}p(.{4})(.)(\\d\\d)";

	regex = new TRegEx();
	reMatch = regex->Matches(sKey, sPatten);
	if(reMatch.Count == 0)
	{
		// (CBETA, T01, no. 1, p. 23c20-21)  , 新版
		// (CBETA, T01, no. 1, p. 23, c20-21) , 舊版
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

	// 展開第一層

	for(int i=0; i<tvMuluTree->ChildrenCount; i++)
	{
		try
		{
			tvMuluTree->Items[i]->Expand();
		}
		catch(...)
		{
            // 忽略...
        }
	}

	// 檢查書目區是不是縮到最小

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
// 上一卷
void __fastcall TfmMain::btPrevJuanClick(TObject *Sender)
{
	if(SpineID == -1) return;
	if(SpineID > 0)
	{
			// 檢查是不是同一經
		String sThisSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];
		String sNextSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID-1];
		if(sThisSutra == sNextSutra)
		{
			String sFile = Bookcase->CBETA->Spine->CBGetFileNameBySpineIndex(SpineID-1);
			ShowCBXML(sFile);
			return;
		}
	}

	TDialogService::ShowMessage(u"目前已是第一卷/篇章。");
}
//---------------------------------------------------------------------------
// 下一卷
void __fastcall TfmMain::btNextJuanClick(TObject *Sender)
{
	if(SpineID == -1) return;
	if(SpineID + 1 < Bookcase->CBETA->Spine->Files->Count)
	{
		// 檢查是不是同一經
		String sThisSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID];
		String sNextSutra = Bookcase->CBETA->Spine->Sutra->Strings[SpineID+1];
		if(sThisSutra == sNextSutra)
		{
			String sFile = Bookcase->CBETA->Spine->CBGetFileNameBySpineIndex(SpineID+1);
			ShowCBXML(sFile);
			return;
		}
	}

	TDialogService::ShowMessage(u"目前已是最後一卷/篇章。");
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::mmiAboutClick(TObject *Sender)
{
	fmAbout->ShowModal();
}
//---------------------------------------------------------------------------

// 檢查有沒有更新程式
void __fastcall TfmMain::CheckUpdate(bool bShowNoUpdate)
{
	// 取得資料版本
	String sDataVer = Bookcase->CBETA->Version;

	fmUpdate->CheckUpdate(Version, sDataVer, bShowNoUpdate);

	if(!fmUpdate->IsUpdate)    // 有更新就不要修改更新日期
	{
		String sToday = GetTodayString();
		Setting->LastUpdateChk = sToday;
		Setting->SaveToFile();
	}

/* 舊的, 讀取 update.exe, 不用了
#ifdef _Windows

	//HWND handle = fmMain->Handle;

	HINSTANCE  hResult;
	int iResult;
	String sFileName = MyFullPath + u"update.exe";

	hResult = ShellExecute(0,L"open",sFileName.c_str(),sPara.c_str(),MyFullPath.c_str(), SW_SHOW);

	iResult = (int) hResult;
	switch(iResult)
	{
		case 0                    : TDialogService::ShowMessage(u"系統記憶體或資源不足, 無法順利開啟!"); break;
		case ERROR_FILE_NOT_FOUND : TDialogService::ShowMessage(u"找不到更新檢查檔案" + sFileName); break;
		case SE_ERR_NOASSOC       : TDialogService::ShowMessage(u"此類檔案無登記, 無法順利開啟!"); break;
		//case SE_ERR_OOM           : TDialogService::ShowMessage("記憶體不足, 無法順利開啟!"); break;
		default:
		{
			if(iResult <= 32)  // 其它情況的錯誤
			{
				TDialogService::ShowMessage("SE_Err:" + AnsiString(iResult) + u" 無法開啟此檔!");
			}
		}
	}

	String sToday = GetTodayString();
	Setting->LastUpdateChk = sToday;
	Setting->SaveToFile();
#else
	TDialogService::ShowMessage(u"抱歉！目前只有 Windows 版才有更新功能。");
#endif
*/
}
//---------------------------------------------------------------------------
// 檢查更新
void __fastcall TfmMain::mmiUpdateClick(TObject *Sender)
{
	CheckUpdate(true);  // true 表示沒有更新要秀訊息
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btNavWidthSwitchClick(TObject *Sender)
{
	if(pnNav->Width == 0)
	{
		// 開啟主功能區
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
		btNavWidthSwitch->Text = u"主功能>>";
	else
		btNavWidthSwitch->Text = u"<<主功能";
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::fanMuluWidthFinish(TObject *Sender)
{
	if(pnMulu->Width == 0)
		btMuluWidthSwitch->Text = u"目次>>";
	else
		btMuluWidthSwitch->Text = u"<<目次";
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
// 選擇全文檢索引擎
void __fastcall TfmMain::SetSearchEngine()
{
	// 選擇全文檢索引擎, 若某一方為 0 , 則選另一方 (全 0 就不管了)
	if(Bookcase->CBETA->SearchEngine_CB == 0)
		SearchEngine = Bookcase->CBETA->SearchEngine_orig;  // 原書索引
	else if(Bookcase->CBETA->SearchEngine_orig == 0)
		SearchEngine = Bookcase->CBETA->SearchEngine_CB;  // CBETA 索引

	else if(Setting->CollationType == ctCBETACollation)
		SearchEngine = Bookcase->CBETA->SearchEngine_CB;    // CBETA 索引
	else
		SearchEngine = Bookcase->CBETA->SearchEngine_orig;  // 原書索引
}
//---------------------------------------------------------------------------
// 取得今日日期, 格式 19991231
String __fastcall TfmMain::GetTodayString()
{
	TDateTime * Today = new TDateTime(Now());
	String str = Today->FormatString("yyyymmdd");
	delete Today;
	return str;
}
//---------------------------------------------------------------------------
// 更換更新的 URL , 換成測試用的
void __fastcall TfmMain::wmiUpdateURLClick(TObject *Sender)
{
	wmiUpdateURL->IsChecked = !wmiUpdateURL->IsChecked;
	fmUpdate->UseLocalhostURL = wmiUpdateURL->IsChecked;
}
//---------------------------------------------------------------------------
// 批量產生 HTML , 主要是用來檢查錯誤
void __fastcall TfmMain::wmiCreateHtmlClick(TObject *Sender)
{
    fmCreateHtml->ShowModal();
}
//---------------------------------------------------------------------------
// 建立索引檔
void __fastcall TfmMain::wmiBuildIndexClick(TObject *Sender)
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

	SetSearchEngine();
}
//---------------------------------------------------------------------------
// 將所有的 Default 取消
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
// 檢索本經
void __fastcall TfmMain::cbSearchThisSutraChange(TObject *Sender)
{
	if(cbSearchThisSutra->IsChecked)
	{
		// 設定檢索此經
		if(SpineID == -1)
		{
			cbSearchThisSutra->IsChecked = false;
			return;
		}

		if(cbSearchRange->Visible)
			cbSearchRange->IsChecked = false;

		// 取出本經
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
		TDialogService::ShowMessage("複製有問題");
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
			TDialogService::ShowMessage("引用複製有問題");
		}

	}
}
//---------------------------------------------------------------------------

