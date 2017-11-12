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
	// 取得設定檔並讀取所有設定

	// 取得 Bookcase 所有資料區

#ifdef _Windows
	//SetPermissions(); // 將 IE 設定到 IE 11 (如果沒 IE 11 的如何?)
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
// 是否有選擇套書了?
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

	// 逐一搜尋目錄
	if(IsSelectedBook())
	{
		CSeries * Series = (CSeries *) Bookcase->Books->Items[SelectedBook];
		CCatalog * Catalog = Series->Catalog;
		int iCount = Catalog->ID->Count;
		// 逐一檢查
		sgFindSutra->BeginUpdate();
		int iGridIndex = 0;
		sgFindSutra->RowCount = 10;
		for(int i=0; i<iCount; i++)
		{
			bool bFound = true;

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
void __fastcall TfmMain::ShowCBXML(String sFile)
{
	if(sFile == "")
	{
		ShowMessage("沒有找到正確檔案");
        return;
    }
	sFile = Bookcase->CBETA->Dir + sFile;
	CCBXML * CBXML = new CCBXML(sFile, Setting->CBXMLOption);

	// 先不用, 因為 mac os 產生出來的檔名是 /var/tmp/xxxxx
	// windows 是 xxxxxx
	// 所以日後還是自己寫吧
	//char cOutFile[14];
	//std::tmpnam(cOutFile);

	String sOutFile = sFile + ".htm";   // ???? 輸出的檔名暫時湊合著
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

	// 傳入 T, 1 , 傳回 "T01" 這種標準的冊數
	sVol = csCBETA->BookData->GetFullVolString(sBook, sVol);

	String sFile = csCBETA->CBGetFileNameByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	ShowCBXML(sFile);
}
//---------------------------------------------------------------------------

