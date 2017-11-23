// ---------------------------------------------------------------------------

#pragma hdrstop

#include "cbxml.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 建構函式
// 傳入參數為 XML 檔, 呈現的設定
__fastcall CCBXML::CCBXML(String sFile, CSetting * cSetting)
{
	// 初值
	XMLFile = sFile;
	Setting = cSetting;
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
	///if(Setting->ShowLineFormat) // 呈現原書格式
	HTMLText += "<br class=\"lb_br\"/>";

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

	//if(!Setting->ShowLineFormat) // 不呈現原書格式
		HTMLText += "<p>";
	parseChild(Node); // 處理內容
	//if(!Setting->ShowLineFormat) // 不呈現原書格式
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
	"   <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n"
	"	<style>\n"
	"		body { background:#DDF1DC; font-weight: normal; line-height:20pt; color:#000000; font-size:16pt;}\n"
	"		span.SearchWord1 {color:#0000ff; background: #ffff66;}\n"
	"		span.SearchWord2 {color:#0000ff; background: #a0ffff;}\n"
	"		span.SearchWord3 {color:#0000ff; background: #99ff99;}\n"
	"		span.SearchWord4 {color:#0000ff; background: #ff9999;}\n"
	"		span.SearchWord5 {color:#0000ff; background: #ff66ff;}\n"
	"		span.guess1 {background: #fff0a0;}\n"
	"		span.guess2 {background: #ffd080;}\n"
	"		span.guess3 {background: #ffb060;}\n"
	"		span.guess4 {background: #ff9040;}\n"
	"		a.hover	{color:#0000ff;}\n"
	"		a:visited 	{color:#0000ff;}\n"
	"		a:link		{color:#0000ff;}\n"
	"		a:active	{color:#0000ff;}\n"
	"		.nonhan	{font-family:\"Times New Roman\", \"Gandhari Unicode\";}\n"
	"		.juannum {color:#008000; font-weight: normal; font-size:16pt;}\n"
	"		.juanname {color:#0000FF; font-weight: bold; font-size:18pt;}\n"
	"		.xu {color:#0000A0; font-weight: normal; font-size:16pt;}\n"
	"		.w {color:#0000A0; font-weight: normal; font-size:16pt;}\n"
	"		.byline {color:#408080; font-weight: normal; font-size:16pt;}\n"
	"		.headname {color:#0000A0; font-weight: bold; font-size:18pt;}\n"
	"		.linehead {color:#0000A0; font-weight: normal; font-size:14pt;}\n"
	"		.lg {color:#008040; font-weight: normal; font-size:16pt;}\n"
	"		.corr {color:#FF0000; font-weight: normal; }\n"
	"		.note {color:#9F5000; font-weight: normal; font-size:14pt;}\n";

	if(Setting->ShowLineFormat)
		HTMLText += u"		p {display:inline;}\n";
	else
		HTMLText += u"		p {display:block;}\n"
					"		br.lb_br {display:none;}\n";

	HTMLText += u"	</style>\n"
	"</head>\n"
	"<body>\n";

	// 檔頭程式

	HTMLText += u"<script>\n"
	"function ShowLine()\n"
	"{\n"
	"	$(\"p\").css(\"display\",\"inline\");\n"
	"	$(\"br.lb_br\").css(\"display\",\"inline\");\n"
	"}\n"
	"function ShowPara()\n"
	"{\n"
	"	$(\"p\").css(\"display\",\"block\");\n"
	"	$(\"br.lb_br\").css(\"display\",\"none\");\n"
	"}\n"
	"</script>\n"
	"<div align=\"right\" style=\"position:fixed; border:1px; ; margin:10px; padding:10px; background-color:#1e7e7e; right:0px;\">\n"
	"		<input type=\"button\" value=\"原書\" onclick=\"ShowLine()\"/>\n"
	"		<input type=\"button\" value=\"段落\" onclick=\"ShowPara()\"/>\n"
	"</div>\n";

}
// ---------------------------------------------------------------------------

