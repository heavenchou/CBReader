// ---------------------------------------------------------------------------

#pragma hdrstop

#include "series.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 建構式
__fastcall CSeries::CSeries(String sDir)
{
	// 初值

	Title = "";       // 標題
	Creator = "";     // 作者
	PublishDate = ""; // 出版日期
	NavFile = "";     // 導覽文件
	Nav2File = "";     // 導覽文件
	Nav3File = "";     // 導覽文件
	CatalogFile = "";     // 目錄文件
	SpineFile = "";   	// 遍歷文件
	BookDataFile = "";   // Bookdata 文件
	JSFile = "";          // CBReader 專用的 js 檔
	Version = "";       // 資料版本

	Catalog = 0;	  	// 目錄
	Spine = 0;    		// 遍歷文件
	JuanLine = 0;   	// 各卷與頁欄行的關係物件, CBETA 專用
	BookData = 0;   	// 每本書的資訊, 例如 T , 大正藏, 2

	SearchEngine_orig = 0;   // 全文檢索引擎
	SearchEngine_CB = 0;   // 全文檢索引擎

	//--------------

	Dir = IncludeTrailingPathDelimiter(sDir);	// 本書的目錄

	// 判斷目錄中有沒有 metadata : index.xml

	String sMeta = Dir + "index.xml";

	String asFileName = FileSearch("index.xml", Dir);
	if (!asFileName.IsEmpty())
	{
		LoadMetaData(asFileName);
	}

	Catalog = new CCatalog(); // 載入目錄資料
	if(!CatalogFile.IsEmpty())
	{
		Catalog->LoadCatalog(Dir + CatalogFile);
	}

	Spine = new CSpine();
	if(!SpineFile.IsEmpty())
	{
		Spine->LoadSpineFile(Dir + SpineFile);
	}

	Spine = new CSpine();
	if(!SpineFile.IsEmpty())
	{
		Spine->LoadSpineFile(Dir + SpineFile);
	}

	BookData = new CBookData();
	if(!BookDataFile.IsEmpty())
	{
		BookData->LoadBookDataFile(Dir + BookDataFile);
	}

	// CBETA 專用, 要處理 JuanLine 資料
	if((ID == "CBETA")||(ID == "SEELAND"))
	{
		JuanLine = new CJuanLine();
		JuanLine->LoadFromSpine(Spine);
	}

	// 載入全文檢索
	LoadSearchEngine();
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CSeries::~CSeries()
{
	if(Catalog) delete Catalog;
	if(Spine) delete Spine;
	if(JuanLine) delete JuanLine;
	if(BookData) delete BookData;
	if(SearchEngine_orig)
	{
		delete SearchEngine_orig;
		SearchEngine_orig = 0;
	}
	if(SearchEngine_CB)
	{
		delete SearchEngine_CB;
		SearchEngine_CB = 0;
	}
}
// ---------------------------------------------------------------------------
// 載入後設文件
void __fastcall CSeries::LoadMetaData(String sMeta)
{
	// 分析 index.xml
	_di_IXMLDocument Document;
	_di_IXMLNode Node;

	Document = interface_cast<Xmlintf::IXMLDocument>(new TXMLDocument(NULL));
	Document->FileName = sMeta;
	Document->Active = true;

	// 讀 ID

	Node = Document->DocumentElement->ChildNodes->Nodes["id"];
	if(Node->ChildNodes->Count > 0)
	{
		ID = Node->ChildNodes->Get(0)->Text;
	}

	// 讀 Title

	Node = Document->DocumentElement->ChildNodes->Nodes["title"];
	if(Node->ChildNodes->Count > 0)
	{
		Title = Node->ChildNodes->Get(0)->Text;
	}

	// 讀 Creator

	Node = Document->DocumentElement->ChildNodes->Nodes["creator"];
	if(Node->ChildNodes->Count > 0)
	{
		Creator = Node->ChildNodes->Get(0)->Text;
	}

	// 讀 date

	Node = Document->DocumentElement->ChildNodes->Nodes["date"];
	if(Node->ChildNodes->Count > 0)
	{
		PublishDate = Node->ChildNodes->Get(0)->Text;
	}

	// 讀 nav

	Node = Document->DocumentElement->ChildNodes->Nodes["nav"];
	if(Node->HasAttribute("src"))
	{
		NavFile = Node->GetAttribute("src");
	}

	// 讀 nav

	Node = Document->DocumentElement->ChildNodes->Nodes["nav2"];
	if(Node->HasAttribute("src"))
	{
		Nav2File = Node->GetAttribute("src");
	}

	Node = Document->DocumentElement->ChildNodes->Nodes["nav3"];
	if(Node->HasAttribute("src"))
	{
		Nav3File = Node->GetAttribute("src");
	}

	// 讀 toc

	Node = Document->DocumentElement->ChildNodes->Nodes["catalog"];
	if(Node->HasAttribute("src"))
	{
		CatalogFile = Node->GetAttribute("src");
	}

	// 讀 Spine

	Node = Document->DocumentElement->ChildNodes->Nodes["spine"];
	if(Node->HasAttribute("src"))
	{
		SpineFile = Node->GetAttribute("src");
	}

	// 讀 BookData
	Node = Document->DocumentElement->ChildNodes->Nodes["bookdata"];
	if(Node->HasAttribute("src"))
	{
		BookDataFile = Node->GetAttribute("src");
	}

	// 讀 JSFile
	Node = Document->DocumentElement->ChildNodes->Nodes["javascript"];
	if(Node->HasAttribute("src"))
	{
		JSFile = Node->GetAttribute("src");
	}

	// 讀 Version

	Node = Document->DocumentElement->ChildNodes->Nodes["version"];
	if(Node->ChildNodes->Count > 0)
	{
		Version = Node->ChildNodes->Get(0)->Text;
	}
}
// ---------------------------------------------------------------------------
// 則由經卷去找 XML 檔名
String __fastcall CSeries::CBGetFileNameBySutraNumJuan(String sBookID, String sSutraNum, String sJuan, String sPage, String sField, String sLine)
{
	String sFileName = Spine->CBGetFileNameBySutraNumJuan(sBookID, sSutraNum, sJuan);

 	// 檔名要補上 #p0001a01 這種格式的位置

	String sPageLine = CBGetPageLine(sPage, sField, sLine);
	if(sPageLine != "")
	{
		sFileName += "#p" + sPageLine;
	}

	return sFileName;
}
// ---------------------------------------------------------------------------
// 由冊頁欄行找經文
String __fastcall CSeries::CBGetFileNameByVolPageFieldLine(String sBook, String sVol, String sPage, String sField, String sLine)
{
	// 傳入 T, 1 , 傳回 "01" 這種標準的冊數
	sVol = BookData->GetNormalVolNumString(sBook, sVol);

	int iIndex = JuanLine->CBGetSpineIndexByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	if(iIndex == -1) return "";

	String sFileName = Spine->CBGetFileNameBySpineIndex(iIndex);

	// 檔名要補上 #p0001a01 這種格式的位置

	String sPageLine = CBGetPageLine(sPage, sField, sLine);
	if(sPageLine != "")
	{
		sFileName += "#p" + sPageLine;
	}

	return sFileName;
}
// ---------------------------------------------------------------------------
// 由頁欄行取得標準 0001a01 格式的字串
String __fastcall CSeries::CBGetPageLine(String sPage, String sField, String sLine)
{
	if(sPage != "" || sField != "" || sLine != "")
	{
		if(sPage == "") sPage = "0001";
		int iLen = sPage.Length();
		if(iLen < 4)
		{
			// 補 0 , 不過若第一個字是英文字, 要保留
			if(*sPage.begin() >= '0' && *sPage.begin() <= '9')
				sPage = String().StringOfChar(L'0', 4 - iLen) + sPage;
			else
			{
				// 第一字不是數字
				String sFirst = *sPage.begin();
				sPage.Delete0(0,1);
				sPage = sFirst + String().StringOfChar(L'0', 4 - iLen) + sPage;
            }
		}
		else if(iLen > 4)
		{
			sPage.Delete0(0,iLen-4);
		}

		if(sField == "") sField = "a";
		iLen = sField.Length();
		if(iLen > 1)
		{
			sField.Delete0(0,iLen-1);
		}

		if(sLine == "") sLine = "01";
		iLen = sLine.Length();
		if(iLen < 2)
		{
			sLine = String().StringOfChar(L'0', 2 - iLen) + sLine;
		}
		else if(iLen > 2)
		{
			sLine.Delete0(0,iLen-2);
		}

		return sPage + sField + sLine;
	}
	else
	{
		return "";
    }
}
// ---------------------------------------------------------------------------
// 載入全文檢索引擎
void __fastcall CSeries::LoadSearchEngine()
{
	if(TDirectory::Exists(Dir + "index"))
	{
		// CBETA 版的索引
		String sWordIndexFile = Dir + "index/wordindex.ndx";
		String sMainIndexFile = Dir + "index/main.ndx";
		if(TFile::Exists(Dir + SpineFile) &&
			TFile::Exists(sWordIndexFile) &&
			TFile::Exists(sMainIndexFile))
		{
			SearchEngine_CB = new TmyMonster(Dir + SpineFile, sWordIndexFile, sMainIndexFile);
		}

		// 原書的索引

		sWordIndexFile = Dir + "index/wordindex_o.ndx";
		sMainIndexFile = Dir + "index/main_o.ndx";
		if(TFile::Exists(Dir + SpineFile) &&
			TFile::Exists(sWordIndexFile) &&
			TFile::Exists(sMainIndexFile))
		{
			SearchEngine_orig = new TmyMonster(Dir + SpineFile, sWordIndexFile, sMainIndexFile);
		}
	}
}
// ---------------------------------------------------------------------------
// 釋放全文檢索引擎
void __fastcall CSeries::FreeSearchEngine()
{
	if(SearchEngine_orig)
	{
		delete SearchEngine_orig;
		SearchEngine_orig = 0;
	}
	if(SearchEngine_CB)
	{
		delete SearchEngine_CB;
		SearchEngine_CB = 0;
	}
}
// ---------------------------------------------------------------------------



