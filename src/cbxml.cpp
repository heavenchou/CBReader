// ---------------------------------------------------------------------------

#pragma hdrstop

#include "cbxml.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 建構函式
// 傳入參數為 XML 檔, 呈現的設定
__fastcall CCBXML::CCBXML(String sFile, CCBXMLOption * cbxOption)
{
	// 初值
	XMLFile = sFile;
	CBXMLOption = cbxOption;
	HTMLText = "";

	Document = interface_cast<Xmlintf::IXMLDocument>(new TXMLDocument(NULL));
	Document->FileName = XMLFile;

	MakeHTMLHead(); // 先產生 html 的 head
	ParseXML();
    HTMLText += "\n</body>\n</html>";
}

// ---------------------------------------------------------------------------
__fastcall CCBXML::~CCBXML(void) // 解構函式
{
	// 底下的 delete 在 mac 會當掉, 要找正規方法 ???
	// if (Document) delete Document;
}

// ---------------------------------------------------------------------------
// 解析 XML , 儲存到 TreeRoot 中
void __fastcall CCBXML::ParseXML()
{
	_di_IXMLNode Node;
	Document->Active = true;

	// 遍歷 XML

	Node = Document->DocumentElement->ChildNodes->Nodes["text"];

	if (Node->ChildNodes->Count == 0)
		ShowMessage(u"錯誤：導覽文件找不到 text 標記。");
	else
	{
		ParseNode(Node);
	}
}

// ---------------------------------------------------------------------------
// 解析 XML Node
void __fastcall CCBXML::ParseNode(_di_IXMLNode Node)
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
		if (sTagName == "a")
			tag_a(Node);
		else if (sTagName == "p")
			tag_p(Node);
		else if (sTagName == "lb")
			tag_lb(Node);
		else if (sTagName == "pb")
			tag_pb(Node);
		else
			tag_default(Node);
	}
	else if (nodetype == 3)
	{
		// 文字節點
		HTMLText += Node->Text;
	}
}

// ---------------------------------------------------------------------------
// 解析 XML Child
void __fastcall CCBXML::parseChild(_di_IXMLNode Node)
{
	for (int i = 0; i < Node->ChildNodes->Count; i++)
	{
		_di_IXMLNode node = Node->ChildNodes->Get(i);
		ParseNode(node);
	}
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CCBXML::tag_default(_di_IXMLNode Node)
{
	parseChild(Node); // 處理內容
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CCBXML::tag_a(_di_IXMLNode Node)
{
	// 處理標記
	parseChild(Node); // 處理內容
	// 結束標記
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CCBXML::tag_lb(_di_IXMLNode Node)
{
	// 處理標記
	if(CBXMLOption->ShowLineFormat) // 呈現原書格式
		HTMLText += "<br/>\n";

	parseChild(Node); // 處理內容
	// 結束標記
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CCBXML::tag_pb(_di_IXMLNode Node)
{
	parseChild(Node); // 處理內容
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CCBXML::tag_p(_di_IXMLNode Node)
{

	if(!CBXMLOption->ShowLineFormat) // 不呈現原書格式
		HTMLText += "<p>";
	parseChild(Node); // 處理內容
	if(!CBXMLOption->ShowLineFormat) // 不呈現原書格式
		HTMLText += "</p>";
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CCBXML::tag_div(_di_IXMLNode Node)
{
	// 讀取 href 屬性
	//String sAttr = Node->Attributes["href"];
	//if (sAttr != "")
	{
	}
	parseChild(Node); // 處理內容
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
void __fastcall CCBXML::tag_note(_di_IXMLNode Node)
{
	parseChild(Node); // 處理內容
}
// ---------------------------------------------------------------------------
// 儲存至 HTML
void __fastcall CCBXML::SaveToHTML(String sFile)
{
	TFile::WriteAllText(sFile, HTMLText, TEncoding::UTF8);
}
// ---------------------------------------------------------------------------
// 先產生 html 的 head
void __fastcall CCBXML::MakeHTMLHead()
{
	HTMLText = u"<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
	"	<meta charset=\"utf-8\">\n"
	"	<title>CBETA 線上閱讀</title>\n"
	"</head>\n"
	"<body>\n";
}
// ---------------------------------------------------------------------------

