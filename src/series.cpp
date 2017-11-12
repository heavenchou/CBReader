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
	NavFile = "";     // 導覽文件
	CatalogFile = "";     // 目錄文件
	SpineFile = "";   // 遍歷文件
	BookDataFile = "";   // Bookdata 文件

	Catalog = 0;	  // 目錄
	Spine = 0;    		// 遍歷文件
	JuanLine = 0;   	// 各卷與頁欄行的關係物件, CBETA 專用
	BookData = 0;   // 每本書的資訊, 例如 T , 大正藏, 2

	//--------------

	Dir = sDir + "/";         // 本書的目錄

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
	if(ID == "CBETA")
	{
		JuanLine = new CJuanLine();
		JuanLine->LoadFromSpine(Spine);
	}
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CSeries::~CSeries()
{
	if(Catalog) delete Catalog;
	if(Spine) delete Spine;
	if(JuanLine) delete JuanLine;
	if(BookData) delete BookData;
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

	// 讀 nav

	Node = Document->DocumentElement->ChildNodes->Nodes["nav"];
	if(Node->HasAttribute("src"))
	{
		NavFile = Node->GetAttribute("src");
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
}
// ---------------------------------------------------------------------------
// 則由經卷去找 XML 檔名
String __fastcall CSeries::CBGetFileNameBySutraNumJuan(String sBookID, String sSutraNum, String sJuan)
{
	return Spine->CBGetFileNameBySutraNumJuan(sBookID, sSutraNum, sJuan);
}
// ---------------------------------------------------------------------------
// 由冊頁欄行找經文
String __fastcall CSeries::CBGetFileNameByVolPageFieldLine(String sBook, String sVol, String sPage, String sField, String sLine)
{
	int iIndex = JuanLine->CBGetSpineIndexByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	if(iIndex == -1) return "";

	return Spine->CBGetFileNameBySpineIndex(iIndex);
}
// ---------------------------------------------------------------------------



