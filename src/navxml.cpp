// ---------------------------------------------------------------------------

#pragma hdrstop

#include "navxml.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CNavXML::CNavXML(String sFile) // 建構函式
{
	// 初值
	XMLFile = sFile;
	TreeRoot = 0;
	ThisLevel = 0;
	ClearThisItem();

	Document = interface_cast<Xmlintf::IXMLDocument>(new TXMLDocument(NULL));
	Document->FileName = XMLFile;
}

// ---------------------------------------------------------------------------
__fastcall CNavXML::~CNavXML(void) // 解構函式
{
	// 底下的 delete 在 mac 會當掉, 要找正規方法 ???
	// if (Document) delete Document;
}

// ---------------------------------------------------------------------------
// 清除 ThisItem
void __fastcall CNavXML::ClearThisItem()
{
	ThisItem.Level = -1;
	ThisItem.Type = nit_Title;
	ThisItem.Title = "";
	ThisItem.URL = "";
}

// ---------------------------------------------------------------------------
// 將目前的 ThisItem 加入 Tree 並清除
void __fastcall CNavXML::AddThisItem()
{
	SNavItem * item = new SNavItem;
	if(*ThisItem.Title.FirstChar() == ' ')
	{
		ThisItem.Title.Delete0(0,1);
	}
	*item = ThisItem;
	TreeRoot->Add(item);
}

// ---------------------------------------------------------------------------
// 將 XML Parse 後存入 TreeRoot 中
void __fastcall CNavXML::SaveToTree(TList * root)
{
	TreeRoot = root;
	ParseXML();
}

// ---------------------------------------------------------------------------
// 解析 XML , 儲存到 TreeRoot 中
void __fastcall CNavXML::ParseXML()
{
	_di_IXMLNode Node;

	TreeRoot->Clear();
	Document->Active = true;

	// 遍歷 XML

	Node = Document->DocumentElement->ChildNodes->Nodes["body"];

	if (Node->ChildNodes->Count == 0)
		TDialogService::ShowMessage(u"錯誤：導覽文件找不到 body 標記。");
	else
	{
		ParseNode(Node);
		if (ThisItem.Level != -1)
			AddThisItem();
	}
}

// ---------------------------------------------------------------------------
// 解析 XML Node
void __fastcall CNavXML::ParseNode(_di_IXMLNode Node)
{
#ifdef _Windows
	TNodeType nodetype = Node->NodeType;
#else
	Xml::Internal::Omnixml::TNodeType nodetype = Node->NodeType;
#endif
	/* Node Type
	1	ELEMENT_NODE
	2	ATTRIBUTE_NODE
	3	TEXT_NODE
	4	CDATA_SECTION_NODE
	5	ENTITY_REFERENCE_NODE
	6	ENTITY_NODE
	7	PROCESSING_INSTRUCTION_NODE
	8	COMMENT_NODE
	9	DOCUMENT_NODE
	10	DOCUMENT_TYPE_NODE
	11	DOCUMENT_FRAGMENT_NODE
	12	NOTATION_NODE
*/

	if (nodetype == 1)
	{
		// 一般節點
		String sTagName = Node->NodeName;
		if (sTagName == "nav")
			tag_nav(Node);
		else if (sTagName == "ol")
			tag_ol(Node);
		else if (sTagName == "li")
			tag_li(Node);
		else if (sTagName == "a")
			tag_a(Node);
		else if (sTagName == "cblink")
			tag_cblink(Node);
		else if (sTagName == "navlink")
			tag_navlink(Node);
		else
			tag_default(Node);
	}
	else if (nodetype == 3)
	{
		// 文字節點
		ThisItem.Title += Node->Text;
	}
}

// ---------------------------------------------------------------------------
// 解析 XML Child
void __fastcall CNavXML::parseChild(_di_IXMLNode Node)
{
	for (int i = 0; i < Node->ChildNodes->Count; i++)
	{
		_di_IXMLNode node = Node->ChildNodes->Get(i);
		ParseNode(node);
	}
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CNavXML::tag_default(_di_IXMLNode Node)
{
	parseChild(Node); // 處理內容
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CNavXML::tag_nav(_di_IXMLNode Node)
{
	// 處理標記

	// 遇到 nav 標記要先儲存前一筆資料
	if (ThisItem.Level != -1)
		AddThisItem();

	ClearThisItem();
	ThisLevel = 0; // 回到最上層
	ThisItem.Level = 0; // 設定新的 Item

	parseChild(Node); // 處理內容
	// 結束標記
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CNavXML::tag_ol(_di_IXMLNode Node)
{
	// 處理標記
	ThisLevel++; // 回到最上層
	parseChild(Node); // 處理內容
	// 結束標記
	ThisLevel--;
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CNavXML::tag_li(_di_IXMLNode Node)
{
	// 遇到 li 標記要先儲存前一筆資料
	if (ThisItem.Level != -1)
		AddThisItem();

	ClearThisItem();
	ThisItem.Level = ThisLevel;
	parseChild(Node); // 處理內容
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CNavXML::tag_a(_di_IXMLNode Node)
{
	// 讀取 href 屬性
	String sAttr = Node->Attributes["href"];
	if (sAttr != "")
	{
		ThisItem.URL = sAttr;
		ThisItem.Type = nit_NormalLink; // 一般的連結
	}
	parseChild(Node); // 處理內容
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CNavXML::tag_navlink(_di_IXMLNode Node)
{
		// 讀取 href 屬性
	String sAttr = Node->Attributes["href"];
	if (sAttr != "")
	{
		ThisItem.URL = sAttr;
		ThisItem.Type = nit_NavLink; // 一般的連結
	}
	parseChild(Node); // 處理內容
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CNavXML::tag_cblink(_di_IXMLNode Node)
{
	// 讀取 href 屬性
	String sAttr = Node->Attributes["href"];
	if (sAttr != "")
	{
		ThisItem.URL = sAttr;
		ThisItem.Type = nit_CBLink; // 一般的連結
	}
	parseChild(Node); // 處理內容
}
// ---------------------------------------------------------------------------
