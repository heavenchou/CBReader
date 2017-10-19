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
	TocFile = "";     // 目錄文件
	SpineFile = "";   // 遍歷文件

	Catalog = 0;	  // 目錄

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
	if(!TocFile.IsEmpty())
	{
		Catalog->LoadCatalog(Dir + TocFile);
	}
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CSeries::~CSeries()
{

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

	Node = Document->DocumentElement->ChildNodes->Nodes["toc"];
	if(Node->HasAttribute("src"))
	{
		TocFile = Node->GetAttribute("src");
	}

	// 讀 Spine

	Node = Document->DocumentElement->ChildNodes->Nodes["spine"];
	if(Node->HasAttribute("src"))
	{
		SpineFile = Node->GetAttribute("src");
	}
}
// ---------------------------------------------------------------------------

