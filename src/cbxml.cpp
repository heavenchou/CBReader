// ---------------------------------------------------------------------------

#pragma hdrstop

#include "cbxml.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 建構函式
// 傳入參數為 XML 檔, 呈現的設定
__fastcall CCBXML::CCBXML(String sFile, String sLink, CSetting * cSetting, String sJSFile , bool bShowHighlight, TmyMonster * seSearchEngine)
{
	// 初值
	XMLFile = sFile;
	Setting = cSetting;
	HTMLText = "";
	HTMLCollation = "";    // HTML 校註
	JSFile = sJSFile;
	ShowHighlight = bShowHighlight;    // 是否塗色
	mOrigNote.clear();

	SerialPath = "";    // 系列(書)的主要位置

	GetInitialFromFileName();   // 由經名取得一切相關資訊

	// 初值
	DivCount = 0;   // Div 的層次
	ListCount = 0;    // list 層次
	InByline = false;       // 用來判斷是否是作譯者

	// 偈頌相關
	IsFindLg = false;		// 一遇到 <lg> 就 true, 第一個 <l> 就會處理並設為 false;
	LgCount = 0;            // 判斷是不是在 <lg> 之中, 主要是用來處理偈頌中的標點要不要呈現.
	LgNormal = true;		    // lg 的 type 是不是 normal? 有 normal 及 abnormal 二種
	LgInline = false;          // lg 的 place 是不是 inline?
	LgMarginLeft = "";      // Lg 整段的位移
	// L
	LTagNum = 0;		    // <l> 出現的數字, 用來判斷要在普及版寫幾個空格
	LMarginLeft = "";       // L 整段的位移

	MarginLeft = "";		// 移位


	ListCount = 0;          // 計算 list 的數目, 有一些地方需要用到
	for(int i=0; i<30; i++) ItemNum[i] = 0; // 用來判斷 item 出現的次數

    CellNum = 0;        // cell 格式數量歸 0
	OtherColspan = 0;   // 因本 cell 佔 n 格以上, 所以和後面的 cell 要空 (n-1)*3 的空格, 此即記錄 n-1 的數字

		InFuWen = 0;			// 用來判斷是否是附文, 因為有巢狀, 所以用 int
		InSutraNum = false;		// 用來判斷本行是否是經號
		InPinHead = false;		// 用來判斷本行是否是品名
		InFuWenHead = false;	// 用來判斷本行是否是附文標題
		InOtherHead = false;	// 用來判斷本行是否是其它標題
		InHead = false;			// 用來判斷本行是否是標題
		InTTNormal = false;		// 在 <tt rend="normal"> 中, 這時每一個 <t> 都要換行 , T54n2133A : <lb n="1194c17"/><p><tt rend="normal"><t lang="san-sd">
		PreFormatCount = 0;		// 判斷是否是要依據原始經文格式切行, 要累加的, 因為可能有巢狀的 pre
	NoNormal = 0;          // 用來判斷是否可用通用字 , 大於 0 就不用通用字, 這二種就不用通用字 <text rend="no_nor"> 及 <term rend="no_nor">

	// 要判斷品的範圍, 若出現品的 mulu, 則記錄 level, 等到 level 數字再次大於或等於時, 此品才結束
	// <mulu level="3" label="3 轉輪聖王品" type="品"/>
	MuluLevel = 999;      // 目錄的層次, 一開始要設到最小
	MuluLabel = u"";      // 目錄的名稱
	InMulu = false;         // 判斷是不是在 <cb:mulu> 之中.
	InMuluPin = false;      // 判斷是不是在 <cb:mulu> 之中, 且是 "品" 目錄.


	Document = interface_cast<Xmlintf::IXMLDocument>(new TXMLDocument(NULL));
	Document->FileName = XMLFile;

	HTMLText += MakeHTMLHead(); // 先產生 html 的 head

	// 是否有塗色? 有就表示有檢索字串
	if(bShowHighlight)
	{
		// 加上搜尋字串的 <div>
		HTMLText += u"<div id=\"SearchHead\">檢索字串：" + seSearchEngine->SearchSentence + u"<hr></div>";
    }

	HTMLText += ParseXML();     		// 處理內文

	HTMLText = AddOrigNote(HTMLText);   // 原本的 orig 校勘還沒加入, 此時才要加入

	// 塗色否?
	if(bShowHighlight)
	{
		CHighlight * HL = new CHighlight(seSearchEngine);
		HTMLText = HL->AddHighlight(&HTMLText);
        delete HL;
	}

	// 記錄校勘
	HTMLText += "<div id=\"CollationList\" style=\"display:none\">\n";
	HTMLText += HTMLCollation;
	HTMLText += "</div>\n";

	if(sLink != "")
	{
		HTMLText += "<script>\n"
		"location.href=\"#" + sLink + "\";"
		"</script>\n";
	}

	HTMLText += "\n</body>\n</html>";
}

// ---------------------------------------------------------------------------
__fastcall CCBXML::~CCBXML(void) // 解構函式
{
	// 底下的 delete 在 mac 會當掉, 要找正規方法 ???
	// if (Document) delete Document;
}

// 先產生 html 的 head
String __fastcall CCBXML::MakeHTMLHead()
{
	String sHtml = u"<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
	"	<meta charset=\"utf-8\">\n"
	"	<title>CBETA 線上閱讀</title>\n"
	"   <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n"
	"   <script src=\"";
	sHtml += JSFile;
	sHtml += u"\"></script>\n"
	"	<style>\n"
	"		body { background:#DDF1DC; font-weight: normal; line-height:20pt; color:#000000; font-size:16pt;}\n"
	"		a.SearchWord0 {color:#0000ff; background: #ffff66;}\n"
	"		a.SearchWord1 {color:#0000ff; background: #a0ffff;}\n"
	"		a.SearchWord2 {color:#0000ff; background: #99ff99;}\n"
	"		a.SearchWord3 {color:#0000ff; background: #ff9999;}\n"
	"		a.SearchWord4 {color:#0000ff; background: #ff66ff;}\n"
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
	"		.headname2 {color:#0000A0; font-weight: bold; font-size:18pt;}\n"
	"		.headname3 {color:#0000A0; font-weight: bold; font-size:18pt;}\n"
	"		.headname4 {color:#0000A0; font-weight: bold; font-size:18pt;}\n"
	"		.linehead {color:#0000A0; font-weight: normal; font-size:14pt;}\n"
	"		.parahead {color:#0000A0; font-weight: normal; font-size:14pt;}\n"
	"		.pts_head {color:#0000A0; font-weight: normal; font-size:14pt;}\n"
	"		.lg {color:#008040; font-weight: normal; font-size:16pt;}\n"
	"		.corr {color:#FF0000; font-weight: normal; }\n"
	"		.note {color:#9F5000; font-weight: normal; font-size:14pt;}\n";

    // 行首格式
	if(Setting->ShowLineFormat)
				  // 原書
		sHtml += u"		div {display:inline;}\n"
				  "		p {display:inline;}\n"
				  "		br.lg_br {display:none;}\n"
				  "     table {display: inline;}\n"
				  "     tr {display: inline;}\n"
				  "     td {display: inline;}\n"
				  "     p.juannum {display:inline; margin-left:0em;}\n"    // 經號
				  "     p.headname2 {display:inline; margin-left:0em;}\n"    // head 標題
				  "     p.headname3 {display:inline; margin-left:0em;}\n"    // head 標題
				  "     p.headname4 {display:inline; margin-left:0em;}\n"    // head 標題
				  "     p.byline {display:inline; margin-left:0em;}\n"    // byline
				  "     span.line_space {display:inline;}\n"  // 行首空格
				  "     ul {display:inline;padding-left:0;}\n"     	// <ul>
				  "     li {display:inline;}\n";     // <li>
	else
				  // 段落格式
		sHtml += u"		div {display:block;}\n"
				  "		p {display:block;}\n"
				  "		br.lb_br {display:none;}\n"
				  "		br.lg_br {display:inline;}\n"
				  "     table {display: table;}\n"
				  "     tr {display: table-row;}\n"
				  "     td {display: table-cell;}\n"
				  "     p.juannum {display:block; margin-left:2em;}\n"  // 經號
				  "     p.headname2 {display:block; margin-left:2em;}\n"    // head 標題
				  "     p.headname3 {display:block; margin-left:3em;}\n"    // head 標題
				  "     p.headname4 {display:block; margin-left:4em;}\n"    // head 標題
				  "     p.byline {display:block; margin-left:4em;}\n"    // byline
				  "     span.line_space {display:none;}\n"     // 行首空格
				  "     ul {display:block;padding-left:40;}\n"     	// <ul>
				  "     li {display:list-item;}\n";     // <li>

	if(Setting->ShowLineHead)
	{
		if(Setting->ShowLineFormat)
			sHtml += u"		.linehead {display:inline;}\n"
					  "		.parahead {display:none;}\n";
		else
			sHtml += u"		.linehead {display:none;}\n"
					  "		.parahead {display:inline;}\n";
		sHtml += u"		.pts_head {display:inline;}\n";
	}
	else
	{
		sHtml += u"		.linehead {display:none;}\n"
				  "		.parahead {display:none;}\n"
				  "		.pts_head {display:none;}\n";
    }


	// 校勘呈現
	if(Setting->CollationType == ctNoCollation)
		sHtml += u"		.note_orig {display:none;}\n"
				  "		.note_mod {display:none;}\n"
				  "		.note_star {display:none;}\n";
	else if(Setting->CollationType == ctOrigCollation)
		sHtml += u"		.note_mod {display:none;}\n"
				  "		.note_orig {display:inline;}\n"
				  "		.note_star {display:inline;}\n";
	else if(Setting->CollationType == ctCBETACollation)
		sHtml += u"		.note_orig {display:none;}\n"
				  "		.note_mod {display:inline;}\n"
				  "		.note_star {display:inline;}\n";

	sHtml += u"	</style>\n"
	"</head>\n"
	"<body>\n";

	return sHtml;
}
// ---------------------------------------------------------------------------
// 解析 XML , 儲存到 TreeRoot 中
String __fastcall CCBXML::ParseXML()
{
	_di_IXMLNode Node;
	Document->Active = true;
	String sHtml = "";

	// 讀取缺字
	_di_IXMLNode NodeGaijis = Document->DocumentElement->ChildNodes->Nodes["teiHeader"]->ChildNodes->Nodes["encodingDesc"]->ChildNodes->Nodes["charDecl"];
	ReadGaiji(NodeGaijis); // 讀取缺字

	// 遍歷 XML

	Node = Document->DocumentElement->ChildNodes->Nodes["text"];

	if (Node->ChildNodes->Count == 0)
		ShowMessage(u"錯誤：找不到 text 標記。");
	else
		sHtml = ParseNode(Node);

	return sHtml;
}
// ---------------------------------------------------------------------------
// 解析 XML Node
String __fastcall CCBXML::ParseNode(_di_IXMLNode Node)
{
	String sHtml = "";

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

		if (sTagName == u"anchor")			sHtml = tag_anchor(Node);
		else if (sTagName == u"app")		sHtml = tag_app(Node);
		else if (sTagName == u"byline")		sHtml = tag_byline(Node);
		else if (sTagName == u"cell")		sHtml = tag_cell(Node);
		else if (sTagName == u"div")		sHtml = tag_div(Node);
		else if (sTagName == u"cb:docNumber")	sHtml = tag_docNumber(Node);
		else if (sTagName == u"entry")		sHtml = tag_entry(Node);
		else if (sTagName == u"foreign")	sHtml = tag_foreign(Node);
		else if (sTagName == u"form")		sHtml = tag_form(Node);
		else if (sTagName == u"formula")	sHtml = tag_formula(Node);
		else if (sTagName == u"g")			sHtml = tag_g(Node);
		else if (sTagName == u"graphic")	sHtml = tag_graphic(Node);
		else if (sTagName == u"head")		sHtml = tag_head(Node);
		else if (sTagName == u"hi")			sHtml = tag_formula(Node);  // 二標記處理法相同
		else if (sTagName == u"item")		sHtml = tag_item(Node);
		else if (sTagName == u"cb:juan") 	sHtml = tag_juan(Node);
		else if (sTagName == u"l")			sHtml = tag_l(Node);
		else if (sTagName == u"lb")			sHtml = tag_lb(Node);
		else if (sTagName == u"lem")		sHtml = tag_lem(Node);
		else if (sTagName == u"lg")			sHtml = tag_lg(Node);
		else if (sTagName == u"list")		sHtml = tag_list(Node);
		else if (sTagName == u"cb:mulu")	sHtml = tag_mulu(Node);
		else if (sTagName == u"note")		sHtml = tag_note(Node);
		else if (sTagName == u"p")			sHtml = tag_p(Node);
		else if (sTagName == u"pb")			sHtml = tag_pb(Node);
		else if (sTagName == u"rdg")		sHtml = tag_rdg(Node);
		else if (sTagName == u"ref")		sHtml = tag_ref(Node);
		else if (sTagName == u"row")		sHtml = tag_row(Node);
		else if (sTagName == u"cb:sg")		sHtml = tag_sg(Node);
		else if (sTagName == u"cb:space")	sHtml = tag_space(Node);
		else if (sTagName == u"cb:t")		sHtml = tag_t(Node);
		else if (sTagName == u"table")		sHtml = tag_table(Node);
		else if (sTagName == u"term")		sHtml = tag_term(Node);
		else if (sTagName == u"text")		sHtml = tag_term(Node); // text 和 term 處理法相同
		else if (sTagName == u"trailer")	sHtml = tag_trailer(Node);
		else if (sTagName == u"unclear")	sHtml = tag_unclear(Node);
		else                      			sHtml = tag_default(Node);
	}
	else if (nodetype == 3)
	{
		// 文字節點
		sHtml = Node->Text;
		sHtml = StringReplace(sHtml, "\n", "", TReplaceFlags() << rfReplaceAll);
		sHtml = StringReplace(sHtml, "\t", "", TReplaceFlags() << rfReplaceAll);

        // 移除新標待處理 ????
	}

	return sHtml;
}

// ---------------------------------------------------------------------------
// 解析 XML Child
String __fastcall CCBXML::parseChild(_di_IXMLNode Node)
{
	String sHtml = "";

	for (int i = 0; i < Node->ChildNodes->Count; i++)
	{
		_di_IXMLNode node = Node->ChildNodes->Get(i);
		sHtml += ParseNode(node);
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_default(_di_IXMLNode Node)
{
	String sHtml = u"";
	//String sXXX = GetAttr(Node, u"xxx");
	sHtml = parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
// 舊版 CBReader 有很多種 anchor , 現在剩二種
// 1. 沒有處理的星號
// <anchor xml:id="fxT01p0009a09"/>
// 2. 雙圈 ◎
// <anchor type="circle"/>
String __fastcall CCBXML::tag_anchor(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sXMLId = GetAttr(Node, u"xml:id");
	String sType = GetAttr(Node, u"type");

	// 沒有處理的星號 <anchor xml:id="fxT01p0009a09"/>
	if(sXMLId.SubString0(0,2) == u"fx")
	{
		sHtml += u"<span class=\"note_star\">[＊]</span>";
	}

	// 雙圈 ◎ <anchor type="circle"/>
	if(sType == u"circle")
	{
		sHtml += u"◎";
	}

	//sHtml = parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_app(_di_IXMLNode Node)
{
	String sHtml = "";
	// 處理標記
	sHtml = parseChild(Node); // 處理內容
	// 結束標記

	return sHtml;
}
// ---------------------------------------------------------------------------
 // <byline cb:type="Translator">
String __fastcall CCBXML::tag_byline(_di_IXMLNode Node)
{
	String sHtml = "";
	if(ListCount == 0) InByline = true;

	sHtml = parseChild(Node); // 處理內容

	if(ListCount)
		sHtml = u"<span class=\"byline\">　" + sHtml + u"</span>";
	else
	{
		InByline = false;
		sHtml = u"<span class=\"line_space\">　　　　</span>"
				u"<p class=\"byline\">" + sHtml + u"</p>";
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_cell(_di_IXMLNode Node)
{
	String sHtml = "";
	String sCols = GetAttr(Node, u"cols");
	String sRows = GetAttr(Node, u"rows");

	CellNum++;        // cell 格式數量累加
	String sColspan = "";
	String sRowspan = "";

	int iColspan = 0;
	if(sCols != "")
	{
		sColspan = sCols;
		iColspan = sColspan.ToIntDef(1)-1;
		sColspan = " colspan = \"" + sColspan + "\"";
	}

	if(sRows != "")
	{
		sRowspan = " rowspan = \"" + sRows + "\"";
	}

	sHtml += u"<td";
	sHtml += sColspan;
	sHtml += sRowspan;
	sHtml += u">";

	// 第一個空一格, 其它空三格

	sHtml += u"<span class=\"line_space\">";
	if(CellNum == 1)
	{
		sHtml += u"　";
	}
	else
	{
		sHtml += u"　　　";
	}

	for(int i=0; i<OtherColspan; i++)
		sHtml += u"　　　";                  //印出前一個 cell 應有的空格

	OtherColspan = iColspan;
	sHtml += u"</span>";

	sHtml += parseChild(Node); // 處理內容
	sHtml += u"&nbsp;</td>";

	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_div(_di_IXMLNode Node)
{
	String sHtml = "";
	// 讀取 href 屬性
	//String sAttr = Node->Attributes["href"];
	//if (sAttr != "")

	DivCount++;
	sHtml = parseChild(Node); // 處理內容
	DivCount--;

    // div 很多待處理 ????
	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_docNumber(_di_IXMLNode Node)
{
	String sHtml = "";

	sHtml += u"<span class=\"line_space\">　　</span>";
	sHtml += u"<p class=\"juannum\">";

	sHtml += parseChild(Node); // 處理內容

	sHtml += "</p>";

	return sHtml;
}
// ---------------------------------------------------------------------------
//String sXXX = GetAttr(Node, "xxx");

// 注意: 這裡有 <p xml:id="pX15p0218b2206" rend="margin-left:1em;inline"> ,
// 在處理 <p> 時要注意 rend 會有 inline 的問題?????

// 辭書的處理 <entry><form>名詞</form><cb:def>解釋..........</cb:def></entry>
// <entry><form>多陀阿伽陀</form><cb:def><p xml:id="pX15p0218b2206" rend="margin-left:1em;inline">如來也。</p></cb:def></entry>
// <entry rend="margin-left:1em"><form>
// 若是行中的 <entry cb:place="inline" rend=.....> 不管 rend 如何, 一律空三格
String __fastcall CCBXML::tag_entry(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sRend = GetAttr(Node, u"rend");
	String sPlace = GetAttr(Node, u"cb:place");

	String sOldMarginLeft = MarginLeft;
	String sTextIndentSpace = MarginLeft;	// 先設為原來的 MarginLeft

	int iMarginLeft = 0;
	int iTextIndent = 0;

	if(sRend != u"")
	{
		CRendAttr * myRend = new CRendAttr(sRend);
		iMarginLeft = myRend->MarginLeft;
		iTextIndent = myRend->TextIndent;

		MarginLeft += String::StringOfChar(u'　',iMarginLeft);// 這是第二行之後要空的
		sTextIndentSpace += String::StringOfChar(u'　',iMarginLeft+iTextIndent);
	}

	// place="inline" 要加三個空格
	if(sPlace == u"inline")			// 行中段落加句點
	{
		sTextIndentSpace = u"　　　";
	}

	sHtml += u"<div class=\"entry\" style=\"text-indent: ";
	sHtml += String(iTextIndent);
	sHtml += u"em; margin-left: ";
	sHtml += String(iMarginLeft);
	sHtml += u"em\">";

	sHtml += u"<span class=\"line_space\">";
	sHtml += sTextIndentSpace;
	sHtml += u"</span>";

	//------------------------------------
	sHtml += parseChild(Node); // 處理內容
	//------------------------------------

	sHtml += u"</div>";
	MarginLeft = sOldMarginLeft;

	return sHtml;
}
// ---------------------------------------------------------------------------
// no.26 <foreign n="0442001" lang="pli" resp="Taisho" cb:place="foot">gabbhaseyy&amacron; punabhav&amacron;bhimbbatti.</foreign>
// cb:place="foot" 就不要呈現
// 印順法師著作集則有很多類似如下外文 <foreign xml:lang="sa">Patna</foreign>
String __fastcall CCBXML::tag_foreign(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sPlace = GetAttr(Node, u"cb:place");
	if(sPlace == u"foot") return u"";

	sHtml = parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_form(_di_IXMLNode Node)
{
	String sHtml = u"<p>";
	sHtml += parseChild(Node); // 處理內容
	sHtml += u"</p>";
	return sHtml;
}
// ---------------------------------------------------------------------------
// <formula rend="vertical-align:super">(1)</formula>
// 轉成 <sup>(1)</sup>

// <formula>S<hi rend="vertical-align:super">n</hi></formula>
// 轉成 S<sup>n</sup>

// formula 或 hi , 誰有  rend="vertical-align:super" 就使用 <sup>...</sup>
String __fastcall CCBXML::tag_formula(_di_IXMLNode Node)
{
	String sHtml = "";
	String sRend = GetAttr(Node, "rend");

	if(sRend == u"vertical-align:super")
	{
		sHtml += u"<sup>";
		sHtml += parseChild(Node); // 處理內容
		sHtml += u"</sup>";
	}
	else
		sHtml += parseChild(Node); // 處理內容

	return sHtml;
}
// ---------------------------------------------------------------------------
// <g ref="#CB00166"/>
String __fastcall CCBXML::tag_g(_di_IXMLNode Node)
{
	String sHtml = "";
	String sCBCode = GetAttr(Node, "ref");
	sCBCode = CMyStrUtil::SubString(sCBCode,1);
	String sCBCodeType = CMyStrUtil::SubString(sCBCode,0,2);

	if(sCBCodeType == "CB")
	{
		sHtml = "<span class=\"gaiji\"";

		// 組字式
		String sDes = CB2des[sCBCode];
		if(!sDes.IsEmpty())
		{
			sHtml += " data-des=\"" + sDes + "\"";
		}
		// 通用字
		String sNor = CB2nor[sCBCode];
		if(!sNor.IsEmpty() && NoNormal == 0)    // 有 NoNormal 就不使用通用字
		{
			sHtml += " data-nor=\"" + sNor + "\"";
		}
		// Unicode
		String sUni = CB2uni[sCBCode];
		if(!sUni.IsEmpty())
		{
			sHtml += " data-uni=\"" + sUni + "\"";
		}
		// Normal Unicode
		String sNorUni = CB2nor_uni[sCBCode];
		if(!sNorUni.IsEmpty())
		{
			sHtml += " data-noruni=\"" + sNorUni + "\"";
		}
		sHtml += ">";

		// 顯示的文字
		// 1. 如果 unicode 優先
		// 2. 如果 通用字 優先

		bool bHasGaiji = true;
		if(Setting->GaijiUniExtFirst)
		{
			if(Setting->GaijiUseUniExt && !sUni.IsEmpty())
				// unicode 優先
				sHtml += sUni;
			else if(Setting->GaijiUseUniExt && Setting->GaijiUseNormal && !sNorUni.IsEmpty() && NoNormal == 0)
				// 通用 unicode 次之
				sHtml += sNorUni;
			else if(Setting->GaijiUseNormal && !sNor.IsEmpty() && NoNormal == 0)
				// 通用字最後
				sHtml += sNor;
			else bHasGaiji = false;
		}
		else
		{
			if(Setting->GaijiUseNormal && !sNor.IsEmpty() && NoNormal == 0)
				// 通用字優先
				sHtml += sNor;
			else if(Setting->GaijiUseUniExt && !sUni.IsEmpty())
				// unicode 次之
				sHtml += sUni;
			else if(Setting->GaijiUseUniExt && Setting->GaijiUseNormal && !sNorUni.IsEmpty() && NoNormal == 0)
				// 通用 unicode 最後
				sHtml += sNorUni;
			else bHasGaiji = false;
		}

		if(bHasGaiji == false)
		{
			// 還沒有 unicode 及通用字
			if(Setting->GaijiDesFirst)
			{
				// 優先使用組字式
				if(!sDes.IsEmpty()) sHtml += sDes;
				//else
					// 圖檔, 暫時不用 ????
			}
			else
			{
				// 圖檔優先 , 暫時不用 ????
				if(!sDes.IsEmpty()) sHtml += sDes;
            }

        }

		// 結束標記
		sHtml += "</span>";
    }

    // 悉曇, 蘭札待處理 ????
	return sHtml;
}
// ---------------------------------------------------------------------------
// 圖檔 <figure><graphic url="../figures/T/T18p0146_01.gif"></graphic></figure>
String __fastcall CCBXML::tag_graphic(_di_IXMLNode Node)
{
	String sHtml = "";
	String sURL = GetAttr(Node, "url");
	if(sURL != "")
	{
		String sPicOrigFile = SerialPath + sURL.Delete0(0,3);
		String sPicFile = ExpandFileName(sPicOrigFile);

		sHtml += u"<img src=\"";
		sHtml += sPicFile;
		sHtml += u"\">";
	}
	//sHtml = parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
// <head>
String __fastcall CCBXML::tag_head(_di_IXMLNode Node)
{
	String sHtml = "";
	// 處理標記

	IsFindLg = false;       // 因為 IsFindLg 是判斷 <l> 是不是第一個, 若是就不空格, 但有一種是 <lg><head>...</head><l> , 所以這也不算第一個 <l>

	String sType = GetAttr(Node, "type");

	if(sType == u"add")
	{
		// 額外的, 不處理 ,
		// 只出現在 T06, T07 <head type="added">No. 220</head>,
		// T21 <head type="added">No. 1222</head>
		return "";
    }

	if(sType == u"pin")     // 品名
	{
		InPinHead = true;
	}
	else if(sType == u"other")    // 附文標題
	{
		InOtherHead = true;
	}

	// head 是否在 list 中待處理 ????
	/*
	if(sxmlParentNodeName == "list")
	{
		// list 的規則參考 list 的項目
		WriteTo("<span class=\"headname\">");
	}
	else
    */
	{
		// 設定 head 待處理 ????
		/*
		if(DivType[DivCount] == "pin")          // 品的標題
			IsPinHead = true;
		else if(DivType[DivCount] == "w")		// 附文的標題
			IsFuWenHead = true;
		else if(DivType[DivCount] == "other")   // 其它的標題
			IsOtherHead = true;
		else
			IsHead = true;
		*/

		// Q1 ==> 空2格
		// Q2 ==> 空3格
		// Q3 ==> 空4格
		// Q4 ==> 空2格
		// Q5 ==> 空3格
		// Q6 ==> 空4格
		// Q7 ==> 空2格
		// Q8 ==> 空3格

		// 原本不應該有 0 , 但有時 head 不在 div 中, 就會有 0 了
		if(DivCount == 0 || DivCount == 1 || DivCount == 4 || DivCount == 7)
			sHtml += u"<span class=\"line_space\">　　</span>"
					"<p class=\"headname2\">";
		else if(DivCount == 2 || DivCount == 5 || DivCount == 8)
			sHtml += u"<span class=\"line_space\">　　　</span>"
					"<p class=\"headname3\">";
		else if(DivCount == 3 || DivCount == 6 || DivCount == 9)
			sHtml += u"<span class=\"line_space\">　　　　</span>"
					"<p class=\"headname4\">";
	}

	sHtml += parseChild(Node); // 處理內容
	// 結束標記

	sHtml += u"</p>";

	if(sType == u"pin")     // 品名
	{
		InPinHead = false;
	}
	else if(sType == u"other")    // 附文標題
	{
		InOtherHead = false;
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_item(_di_IXMLNode Node)
{
	String sHtml = "";

    ItemNum[ListCount]++;

        // 先看其前是不是 lb, list, head.

		String sPreNodeName = u"";
		String sPreChildNodeName = u"";

		_di_IXMLNode PreNode = Node->PreviousSibling();
		if(PreNode)
		{
			sPreNodeName = PreNode->NodeName;

			_di_IXMLNode PreChildNode = PreNode->GetChildNodes()->Last();
			if(PreChildNode) sPreChildNodeName = PreChildNode->NodeName;
		}

        // 這是在 item 之間空格用的, 原本 item 在第一個或行首才要依 list 的層次來空格
        // 但若它前一層有 list, 則空格也要依原來的層次.
        // 就類似 <I5>.....<I6>....<I5> , 則 <I5> 也要空5格.
        // 實例 X26n0524.xml <lb ed="X" n="0633a23"/>

        sHtml += u"<li>";
		//if(Setting->CutLine)	// 大正藏切行
		{
			String sItemId = GetAttr(Node, u"xml:id");

            // 第一個, 或在行首, 就要依 list 數量來空格
			if(ItemNum[ListCount] == 1 || sPreNodeName == u"lb" || sPreChildNodeName == u"list")
            {
				//itemX63p0502b0319 是特例
				// 在切換校註呈現時如何處理? 待研究 ????
				//if(Setting->CorrSelect == 0 && sItemId == u"itemX63p0502b0319"){}
				//else
					sHtml += u"<span class=\"line_space\">" + String::StringOfChar(u'　',ListCount) + u"</span>";
            }
            else
            {
				// 這幾組在呈現修訂用字時不空格
				// 在切換校註呈現時如何處理? 待研究 ????
                /*
				if(Setting->CorrSelect == 0)
				{
					if(sItemId.SubString(1,7) == u"itemX63")
					{
						if     (sItemId == u"itemX63p0416b1820") {}
						else if(sItemId == u"itemX63p0445c0619") {}
						else if(sItemId == u"itemX63p0448b0220") {}
						else if(sItemId == u"itemX63p0450b1619") {}
						else if(sItemId == u"itemX63p0452a2319") {}
						else if(sItemId == u"itemX63p0452b0619") {}
						else if(sItemId == u"itemX63p0453b1820") {}
						else if(sItemId == u"itemX63p0455b1019") {}
						else if(sItemId == u"itemX63p0456b1319") {}
						else if(sItemId == u"itemX63p0456c1119") {}
						else if(sItemId == u"itemX63p0456c1320") {}
						else if(sItemId == u"itemX63p0456c2020") {}
						else if(sItemId == u"itemX63p0457b0919") {}
						else if(sItemId == u"itemX63p0457b1020") {}
						else if(sItemId == u"itemX63p0457b1520") {}
						else if(sItemId == u"itemX63p0468c2329") {}
						else if(sItemId == u"itemX63p0469a0337") {}
						else if(sItemId == u"itemX63p0474a0320") {}
						else if(sItemId == u"itemX63p0474a0719") {}
						else if(sItemId == u"itemX63p0474a1320") {}
						else if(sItemId == u"itemX63p0488a0919") {}
						else if(sItemId == u"itemX63p0488a1320") {}
						else if(sItemId == u"itemX63p0489a0520") {}
						else if(sItemId == u"itemX63p0489a0819") {}
						else if(sItemId == u"itemX63p0489c2420") {}
						else if(sItemId == u"itemX63p0490a2219") {}
						else if(sItemId == u"itemX63p0492c1019") {}
						else if(sItemId == u"itemX63p0493a1019") {}
						else if(sItemId == u"itemX63p0502a0919") {}
						else if(sItemId == u"itemX63p0502a1519") {}
						else if(sItemId == u"itemX63p0502b0319") {}
						else if(sItemId == u"itemX63p0505b0120") {}
						else if(sItemId == u"itemX63p0508c0220") {}
						else if(sItemId == u"itemX63p0508c1319") {}
						else sHtml += u"　";  // 行中的 item 只空一格
					}
					else sHtml += u"　";  // 行中的 item 只空一格
				}
				else
				*/
					sHtml += u"<span class=\"line_space\">　</span>";  // 行中的 item 只空一格
			}
        }


	// item 的屬性要印出來 , <list xml:lang="sa-Sidd" type="ordered">
	//                         <item n="（一）" xml:id="itemT18p0087a1401">....
	String sN = GetAttr(Node, u"n");
	if(sN != u"") sHtml += sN;

	// -----------------------------------
	sHtml += parseChild(Node); // 處理內容
	// -----------------------------------

	sHtml += u"</li>";

	return sHtml;
}
// ---------------------------------------------------------------------------
// <cb:juan n="001" fun="open"><cb:mulu n="001" type="卷"></cb:mulu><cb:jhead><title>雜阿含經</title>卷第一</cb:jhead></cb:juan>
// T03n0158 : <lb n="0240a07" ed="T"/>尼。</p><cb:juan n="001" fun="close" place="inline">

String __fastcall CCBXML::tag_juan(_di_IXMLNode Node)
{
	String sHtml = "";

	sHtml = u"<span class=\"juanname\">";
	String sPlace = GetAttr(Node, "place");
	if(sPlace == u"inline")
	{
		sHtml += u"<span class=\"line_space\">　</span>";
	}
	sHtml += parseChild(Node); // 處理內容
	sHtml += u"</span>";
	return sHtml;
}
// ---------------------------------------------------------------------------
// <lg><l>無上二足尊</l><l>照世大沙門</l>

// <lg rend=... type=...>
//   rend="margin-left:1;text-indent:0" : 表示整段偈誦都會在行首空一格
//   type="normal" (預設): 第一個 <l> 不空格, 其餘 <l> 空二格, 有 rend 的 <l> 依 rend 處理.
//   type="abnormal" : <l> 皆不空格, 有 rend 的 <l> 依 rend 處理.

//   若沒設 type , 一律預設為 normal ,
//   若沒設 rend , type="normal" 時, 預設為 rend="margin-left:1",
//                 type="abnormal" 時, 預設 rend="margin-left:0",

//   若 place="inline", 沒指定 rend , 且 <lg> 前面的文字不是空格時, 則自動加一個空格.

//   <l> 的 rend 不論是 margin-left:1 或是 text-indent:1 , 一律當成 text-indent:1 處理. 前者是為了相容舊版的.

String __fastcall CCBXML::tag_l(_di_IXMLNode Node)
{
	String sHtml = "";

	LTagNum++;					// 若是第一個, 只空一格

	LMarginLeft = "";           // l 整段要空的格
	String sLTextIndent = "";	// l 開頭要空的格

	int iMarginLeft = 0;
	int iTextIndent = 0;
	bool bHasRend = false;	// 若有 rend 就不使用預設的空格
	// 檢查移位 <l rend="margin-left:1">

	String sRend = GetAttr(Node, "rend");

	if(sRend != ""
		&& !(!Setting->ShowPunc && LgNormal)      //若不秀標點且是標準格式, 就不依 rend
		&& !(Setting->NoShowLgPunc && LgNormal))  //若在偈頌中且偈頌不秀新標
	{
		bHasRend = true;
		CRendAttr * myRend = new CRendAttr(sRend);
		iMarginLeft = myRend->MarginLeft;
		iTextIndent = myRend->TextIndent;
		if(iMarginLeft)
			LgMarginLeft += String::StringOfChar(u'　',myRend->MarginLeft);  // lg 整段要空的格
	}
	else
	{
		//   type="normal" (預設): 第一個 <l> 不空格, 其餘 <l> 空二格, 有 rend 的 <l> 依 rend 處理.
		//   type="abnormal" : <l> 皆不空格, 有 rend 的 <l> 依 rend 處理.

		if(LgNormal)
		{
			if(LTagNum != 1)
			{
				iMarginLeft = 2;
				LMarginLeft = u"　　";
			}
		}
	}

	if(LTagNum == 1)
	{
		//if(!Setting->CutLine)
		{
			// 因為 IsFindLg 是判斷 <l> 是不是第一個, 若是就不空格, 但有一種是 <lg><head>...</head><l> ,
			// 所以在 head 就會取消 IsFindLg , 這也不算第一個 <l> 才不會有
			// OK
			//T10n0297_p0881b18║　圓寂宮城門　　能摧戶扇者
			//T10n0297_p0881b19║　諸佛法受用　　救世我頂禮
			//T10n0297_p0881b20║　自手流清水　　能除餓鬼渴
			// no ok
			//圓寂宮城門　　能摧戶扇者
			//　諸佛法受用　　救世我頂禮
			//　自手流清水　　能除餓鬼渴
			if(IsFindLg)		// lg 之後的第一個 <l>
			{
				IsFindLg = false;
			}
			else
			{
				// 在 2016 之前, 標準或非標準的偈頌, 不依原書時, 第一個 <l> 都會折行.
				// 目前的新規則, 只要是不依原書, 非標準偈頌, 且有設定 Setting->LgType = 1 , 就不折行. 不過這只限在 GA 及 GB, 因為舊的經文還是折行較好
				if(!(LgNormal == false && (BookId == u"GA" || BookId == u"GB")))
				{
					sHtml += u"<br class=\"lg_br\"/>";	// 偈頌折行 , 待測試 ????
				}
				sHtml += LgMarginLeft;
			}
		}
	}

	sLTextIndent += String::StringOfChar(u'　',iMarginLeft + iTextIndent); // lg 整段要空的格
	sHtml += sLTextIndent;

	// -----------------------------------
	sHtml += parseChild(Node); // 處理內容
	// -----------------------------------

	LMarginLeft = u"";

	return sHtml;
}
// ---------------------------------------------------------------------------
// <lb>
// <lb n="0150b09" ed="T"/>
String __fastcall CCBXML::tag_lb(_di_IXMLNode Node)
{
	String sHtml = "";
	// 處理標記

	// 有二種情況要忽略
	// 1. 卍續藏有二種 lb , ed 不是 "X" 的要忽略 : <lb ed="R150" n="0705a03"/>
	// 2. 印順導師也有二種 lb , type="old" 要忽略 : <lb n="a003a06" ed="Y" type="old"/>

	if(BookId == u"Y")
	{
		String sType = GetAttr(Node, "type");
		if(sType == u"old") return "";
	}

	String sEd = GetAttr(Node, "ed");
	if(sEd != BookId) return "";

    LTagNum = 0;		// 還原 <l> 的數目, 要來判斷要不要折行或空格數目

	sHtml = "<br class=\"lb_br\"/>";
	String sn = GetAttr(Node, "n");
	PageLine = sn;
	LineHead = BookVolnSutra + "p" + PageLine + "║";
	sHtml += "<a\nname=\"p" + PageLine + "\"></a><span class=\"linehead\">" + LineHead + "</span>";
	// sHtml += parseChild(Node); // 處理內容
	// 結束標記

	// 隔行對照待處理 ????

	// 行首空格待處理 ????

    // 目錄品名待處理 ????

	return sHtml;
}
// ---------------------------------------------------------------------------
// 解析 XML 標記
String __fastcall CCBXML::tag_lem(_di_IXMLNode Node)
{
	String sHtml = "";
	// 處理標記
	sHtml += parseChild(Node); // 處理內容
	// 結束標記

	return sHtml;
}
// ---------------------------------------------------------------------------

// <lg><l>無上二足尊</l><l>照世大沙門</l>

// <lg rend=... type=...>
//   rend="margin-left:1;text-indent:0" : 表示整段偈誦都會在行首空一格

//   type="normal" (預設): 第一個 <l> 不空格, 其餘 <l> 空二格, 有 rend 的 <l> 依 rend 處理.
//   type="abnormal" : <l> 皆不空格, 有 rend 的 <l> 依 rend 處理.

//   若沒設 type , 一律預設為 normal ,
//   若沒設 rend , type="normal" 時, 預設為 rend="margin-left:1",
//                 type="abnormal" 時, 預設 rend="margin-left:0",

//   若 place="inline", 沒指定 rend , 且 <lg> 前面的文字不是空格時, 則自動加一個空格.

//   <l> 的 rend 不論是 margin-left:1 或是 text-indent:1 , 一律當成 text-indent:1 處理. 前者是為了相容舊版的.

// 新標的處理法 : (2007/10/03)
//   不呈現標點時, 在 typle=normal 的情況下, 忽略 lg 與 l 的 rend 屬性, 因為 rend 會因為新標的引號而調整. 故不呈現時就不要處理.
//   呈現標點時, 也可以選擇偈頌不呈現標點, 這時只忽略標點, 但不忽略上下引號, 這樣比較好看.

// 2016 的新作法:
// 原本整段的空格是用空格. 例如 : <lg rend="margin-left:1"> , 2014 之前的版本是每一行前面都加上一個空格. 不過折行就不會空格了.
// 2016 就改成用段落 <p class="lg" style="margin-left:1em;"> , 因此每一行前面就不用加空格.
// 不過因此 copy 再貼在純文字, 就會少了行首的空格.
// 至於引用複製, 就要在原本行首的空格加上 <spane data-space="1"> 表示空一格, 到時再用引用複製來還原一個空格.

String __fastcall CCBXML::tag_lg(_di_IXMLNode Node)
{
	String sHtml = "";

	IsFindLg = true;			// 一遇到 <lg> 就 true, 第一個 <l> 就會處理並設為 false;
	LgCount++;                  // 判斷是不是在 <lg> 之中, 主要是用來處理偈頌中的標點要不要呈現.
	LgNormal = true;			// 預設值, 因為有些舊的 xml 沒有 <lg type=normal>
	LgInline = false;      		// lg 的 place 是不是 inline?
	LgMarginLeft = "";			// lg 整段要空的格
	LTagNum = 0;		        // 還原 <l> 的數目, 要來判斷要不要折行或空格數目

	bool bHasRend = false;		// 先假設沒有 rend 屬性
	String sLgTextIndent = "";	// lg 開頭要空的格
	bool bIsNote = false;       // 若 type 是 note1 or note2 , 則偈誦前後要加小括號

	// 先處理 type 屬性

	String sType = GetAttr(Node, "type");

	if(sType == u"normal") LgNormal = true;		// lg 的 type 是 normal
	if(sType == u"abnormal") LgNormal = false;    // 因為舊版有 type=inline
	if(sType == u"note1" || sType == u"note2")	// 在偈誦前後要加小括號
	{
		LgNormal = true;
		bIsNote = true;
	}

	// 再處理 rend 屬性

	String sRend = GetAttr(Node, "rend");

	int iMarginLeft = 0;
	int iTextIndent = 0;
	// 檢查移位 <lg rend="margin-left:1">
	// 舊版的會有 <lg type="inline">或<lg rend="inline''> , 要改成 type=abnormal
	if(sRend != ""
		&& !(!Setting->ShowPunc && LgNormal)    //若不秀標點且是標準格式, 就不依 rend
		&& !(Setting->NoShowLgPunc && LgNormal))     //若在偈頌中且偈頌不秀新標
	{
		bHasRend = true;
		// 尾端補上 ;
		if(*sRend.LastChar() != u';') sRend += u";";


		int iPos;
		String sMarginLeft = "";
		String sTextIndent = "";

		// 逐一取出 rend 裡面的內容, 可能是 "margin-left:2em;text-indent:1em;inline;"
		while(sRend != "")
		{
			String sTmp = "";

			if((iPos = sRend.Pos0(";")) >= 0)
			{
				sTmp = sRend.SubString0(0,iPos);
				sRend.Delete0(0,iPos+1);
			}

			if(sTmp.SubString0(0,12) == u"margin-left:")
			{
				sMarginLeft = sTmp;
			}
			else if(sTmp.SubString0(0,12) == u"text-indent:")
			{
				sTextIndent = sTmp;
			}
			else if(sTmp == u"inline;")  // 舊版, 新版好像也有
			{
				LgNormal = false;
				LgInline = true;
			}
			// 其他情況就不管了
		}

		// 如果有 MarginLeft:
		if(sMarginLeft != u"")
		{
			// 支援 rend="margin-left:1em" 格式
			if((iPos = sMarginLeft.Pos0(u"em")) >= 0)
			{
				sMarginLeft = sMarginLeft.SubString0(12,iPos-12); // 取出數字
			}
			iMarginLeft = sMarginLeft.ToIntDef(0);
			// lg 整段要空的格
			LgMarginLeft = String::StringOfChar(u'　',iMarginLeft);
		}

		// 如果有 sTextIndent:
		if(sTextIndent != "")
		{
			// 支援 rend="text-indent:1em" 格式
			if((iPos = sTextIndent.Pos0(u"em")) > 0)
			{
				sTextIndent = sTextIndent.SubString0(12,iPos-12); // 取出數字
			}
			iTextIndent = sTextIndent.ToIntDef(0);
		}
	}
	else
	{
		if(LgNormal)
		{
			iMarginLeft = 1;
			LgMarginLeft = "　";	// 非 normal 的可能是 inline, 不一定要空格
		}
	}

	// lg 整段要空的格
	sLgTextIndent = String::StringOfChar(u'　',iMarginLeft + iTextIndent);

	// 處理 place="inline"
	// V2.0 之後, 由 type=inline 改成 place=inline

	String sPlace = GetAttr(Node, "place");

	if(sPlace == "inline")			// 行中段落加句點
	{
		LgInline = true;
	}

	if(LgInline && LgMarginLeft == "" && !bHasRend)	// 即在行中, 又沒有空白, 前一個字也不是空白時, 就加上空白
	{
		// 檢查前一個字是不是空格?
		// 先不管了, 一律加上空格 ????
		sHtml += u"<span class=\"line_space\">　</span>";
	}

	// 如果是不依原書, 且不是 normal 偈頌, 且指定用段落的方式 (LgTYpe = 1), 則處理成 <p style="margin-left::2em;text-indent:xxem;"><lg class="lg"> 這種格式
	//if(Setting->LgType == 1 && LgNormal == false)
	if(LgNormal == false)
	{
		if(iMarginLeft !=0 || iTextIndent != 0)
		{
			sHtml += u"<p style=\"";
			if(iMarginLeft != 0)
			{
				sHtml += u"margin-left:";
				sHtml += String(iMarginLeft);
				sHtml += u"em;";
			}
			if(iTextIndent != 0)
			{
				sHtml += u"text-indent:";
				sHtml += String(iTextIndent);
				sHtml += u"em;";
			}
			sHtml += u"\">";
		}
		else
		{
			sHtml += u"<p>";
		}
	}
	else
	{
		// 傳統舊方式, 用空格來處理, <p>　　<span class="lg">
		sHtml += u"<p>";
		sHtml += sLgTextIndent;
	}

	sHtml += u"<span class=\"lg\">";	// 偈頌折行
	if(bIsNote) sHtml += u"(";		// type 是 note1 or note2 要在偈誦前後要加括號

	// -----------------------------------
	sHtml += parseChild(Node); // 處理內容
    // -----------------------------------

	LgCount--;               // 判斷是不是在 <lg> 之中, 主要是用來處理偈頌中的標點要不要呈現.
	LgMarginLeft = "";
	if(bIsNote) sHtml += u")";
	sHtml += u"</span>";
	sHtml += u"</p>";

	return sHtml;
}
// ---------------------------------------------------------------------------
/* list 規則 :

	1.遇到 list , 如果之後是 head , 則在 head 之後, 才加上 <ul>, 否則直接加上 <ul>
	2.<item> 換成 <li>, </item> 換成 </li>
	3.</list> 換成 </ul>

<list><head>卷第二</head>
<item xml:id="itemX78p0575a1401">臨濟宗</item>
<item xml:id="itemX78p0575a1401"><list><head>臨濟宗</head>
<item xml:id="itemX78p0575a1501">臨濟慧照禪師</item>
<item xml:id="itemX78p0575a1507">興化獎禪師</item>
<item xml:id="itemX78p0575a1601">南院顒禪師</item></list></item></list>

卷第二
<ul>
  <li>臨濟宗</li>
  <li>臨濟宗
    <ul>
      <li>臨濟慧照禪師</li>
      <li>興化獎禪師</li>
      <li>南院顒禪師</li>
    </ul>
   </li>
</ul>

<lb ed="X" n="0418a36"/><item xml:id="itemX86p0418a3601">中梁山崇</item>
<lb ed="X" n="0418a37"/><item xml:id="itemX86p0418a3701">南臺勤
<lb ed="X" n="0418a38"/><list><item xml:id="itemX86p0418a3801">石霜節誠
<lb ed="X" n="0418a39"/><list><item xml:id="itemX86p0418a3901">岳麓珪</item></list></item>
<lb ed="X" n="0418a40"/><item xml:id="itemX86p0418a4001">高陽法廣</item></list></item>

依原書格式呈現 :

行首 <list> : 不理
行首 <item> : 依 <list> 的層數來空格, 第一個空一格, 第二個空二格....
行中 <list> : 不理
行中 <item> : 第一個 item 依 list 的層數來空格, 第二個之後一律空一格.
<lb> 要依 list 留空白, 除非其後是 <list> 或 <item>

*/
String __fastcall CCBXML::tag_list(_di_IXMLNode Node)
{
	String sHtml = "";

	ListCount++;
	ItemNum[ListCount] = 0; // 歸零

	// 看看下一個是不是 <head>
	if(Node->HasChildNodes)
	{
		_di_IXMLNode ChildNode = Node->ChildNodes->First();

		if(ChildNode->GetNodeName() == u"head")
		{
			// 第一個 list 才要切斷, 不然 a06 的卷第一不會換行
			// <lb ed="X" n="0575a04"/><div1 type="other"><mulu type="其他" level="1" label="希叟和尚正宗贊目錄"/><head>希叟和尚正宗贊目錄</head>
			// <lb ed="X" n="0575a05"/>
			// <lb ed="X" n="0575a06"/><list><head>卷第一</head>
			if(ListCount == 1) sHtml += u"<br><br>";     // 第一層才要
		}
	}

	// 如果是 <list rendition="simple"> 要轉成 <ul style="list-style:none;">
	String sRendition = GetAttr(Node, "rendition");
	if(sRendition == u"simple")
		sHtml += u"<ul style=\"list-style:none;\">";
	else
		sHtml += u"<ul>";

	sHtml += parseChild(Node); // 處理內容

	sHtml += u"</ul>";
	ListCount--;
	return sHtml;
}
// ---------------------------------------------------------------------------
// 目錄
// 要判斷品的範圍, 若出現品的 mulu, 則記錄 level, 等到 level 數字再次大於或等於時, 此品才結束

// <mulu level="3" label="1 閻浮提州品" type="品"/>
// <mulu n="001" type="卷"/>

// <cb:mulu level="3" type="品">1 閻浮提州品</cb:mulu>
// <cb:mulu level="1" type="序">序</cb:mulu>
// <cb:mulu level="1" type="分">1 分</cb:mulu>
// <cb:mulu n="001" type="卷"></cb:mulu>
String __fastcall CCBXML::tag_mulu(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sLevel = GetAttr(Node, u"level");
	String sType = GetAttr(Node, u"type");
	int iLevel = sLevel.ToIntDef(0);

	InMulu = true;

	if(sLevel == u"") return u"";     // 不是品這種格式(應該是卷的格式), 不管它

	// 處理法
	// 1. 之前是不是品?
	//    是 : 比較 level , 若 level >= 之前的, 前面的品結束.
	// 2. 檢查自己是不是品, 若是, 就加進去, 並記錄起來.

	if(MuluLabel != u"")     // 之前是品
	{
		if(iLevel <= MuluLevel)     // 目前的比較小, 所以舊的品要結束
		{
			sHtml += u"<a pin_name=\"\"></a>";
			MuluLabel = u"";
			MuluLevel = 0;
		}
		else return u"";     // 之前雖然是品, 但層次不夠, 保持現況
	}

	if(sType == u"品")   // 而目前是品, 就記錄下來
	{
		MuluLabel = u"";
		MuluLevel = iLevel;

		sHtml += u"<a pin_name=\"";
		InMuluPin = true;

		// 目錄的內容有可能有這些標記, 要處理掉 ????
		// 1. 羅馬拼音加上 <span class="nonhan">xx</span>
		// 2. 缺字會加上 <span class="gaiji">...</span>
		// 3. 缺字的 <!--gaiji,缽,1[金*本],2&#Xxxxx;,3-->

		// 同時要注意, 若在校註中, 目錄的內容會不會跑到校註中? (舊版CBR會把文字送到校註中)
		// MuluLabel 的處理也要注意, 這大概是在 lb 會處理????

		String sMulu = parseChild(Node); // 處理內容

		MuluLabel = sMulu;
		sHtml += sMulu;

		InMulu = false;
		InMuluPin = false;	// 先設成 false, 以免底下的內容被記錄至 MuluLabel 中 (舊版才會啦)
		sHtml += u"\"></a>";
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
// 校註標記
String __fastcall CCBXML::tag_note(_di_IXMLNode Node)
{

/* 目前 note 總類

1:無 type

<note place="inline"> : 雙行小註
<note place="interlinear"> : 傍註 (有時是在校勘中用文字寫著 "傍註" 二字)
<note resp="xxxxx" place="inline"> : 雙行小註
<note resp="xxxxx" target="xxxxx"> : CBETA 自己加的註解
<note target="xxxxx"> : 在 p4 是 <foreign> => T01n0026.xml <note target="#beg0434012"><foreign n="0434012" cb:resp="#resp1" xml:lang="pi" cb:place="foot">Niga??has?vaka.</foreign></note>
<note xml:lang="zh" place="inline">

2. 各種 type

<note n="xxxxx" resp="xxxxx" type="orig" place="foot text" target="xxxxx">  : place="foot text" , 校勘同時出現在文字區(數字)及註解區(數字及文字)
<note n="xxxxx" resp="xxxxx" type="orig" place="foot" target="xxxxx">       : place="foot" , 有時文字區沒有校勘數字, 可能用◎符號, 因此 place 只有 foot (T17n0721 一開始)
<note n="xxxxx" resp="xxxxx" type="orig" place="text" target="xxxxx">       : place="text" , 有時文字區有校勘數字, 但校勘欄卻沒有, 因此 plaee 只有 text (T20n1144)
<note n="xxxxx" resp="xxxxx" type="orig" target="xxxxx">                    : T02n0009 # 0003b29 有一個 CBETA 自己加進去的校勘, 所以沒有 place
<note n="xxxxx" resp="xxxxx" type="orig" place="margin-top" target="xxxxx"> : X14n0288 有頭註型式 , 因此 place= "margin-top"

<note n="xxxxx" resp="xxxxx" type="mod" target="xxxxx">
<note n="xxxxx" resp="xxxxx" type="mod" place="foot text" target="xxxxx">


<note n="xxxxx" resp="xxxxx" place="foot text" type="orig_biao" target="xxxxx">	: 卍續藏特有的 "科, 標, 解"
<note n="xxxxx" resp="xxxxx" place="foot text" type="orig_jie" target="xxxxx">
<note n="xxxxx" resp="xxxxx" place="foot text" type="orig_ke" target="xxxxx">
<note n="xxxxx" resp="xxxxx" place="foot" type="orig_ke" target="xxxxx">
<note n="xxxxx" resp="xxxxx" place="text" type="orig_ke" target="xxxxx">

<note n="xxxxx" resp="xxxxx" type="mod_biao" target="xxxxx">
<note n="xxxxx" resp="xxxxx" type="mod_jie" target="xxxxx">
<note n="xxxxx" resp="xxxxx" type="mod_ke" target="xxxxx">                       : 卍續藏特有的 "科, 標, 解"


<note n="xxxxx" type="cf." place="foot" target="xxxxx">
<note n="xxxxx" type="equivalent" place="foot" target="xxxxx">
<note n="xxxxx" type="rest" place="foot" target="xxxxx">
<note n="xxxxx" resp="xxxxx" type="rest" target="xxxxx">

<note type="cf1">
<note type="cf2">
<note type="cf3">

<note type="star" corresp="xxxxx"> : 在南傳才遇到, 因為星號在之前都是出現在 app 標記, 但南傳某些沒有 app 標記, 卻需要星號來表示重覆的註解, 待處理 ?????

2016 新增 type = "editor" , 表示是編輯者自行加入的, 若 resp 是 CBETA , 就表示是 CBETA 自己加上的註解,
                            又如 DILA 自己在佛寺志加上註解, 也是用 "editor" , 表示不是佛寺志原有的註解
    B36n0159.xml
p5a:<note n="0836001" resp="CBETA" type="editor">CBETA 按：「校記 A」 條目已加入本文註標連結。</note>
p5 :<note n="0836001" resp="#resp2" type="editor" target="#nkr_note_editor_0836001">CBETA 按：「校記 A」 條目已加入本文註標連結。</note>

*/

	String sHtml = "";
	String sAttrType = GetAttr(Node, "type");
	String sId = GetAttr(Node, "n");
	String sIdNum = "";	// 0001001a 取得 1a

	// 原書校勘
	if(sAttrType == u"orig")
	{
		if(sId == "") ShowMessage ("錯誤 : 校勘沒有 n 屬性");
		else sIdNum = NoteId2Num(sId);	// 0001001a 取得 1a

		// <a id="note_orig_0001001" class="note_orig" href="" onclick="return false;">

		// note 要暫存起來, 要同時有 note_orig 和 mod
		// 等到真的遇到 mod , 再把 class 的 note_mod 移除

		String sTmp = "<a id=\"note_orig_" + sId +
				 "\" class=\"note_orig note_mod\" href=\"\" onclick=\"return false;\">[" +
				 sIdNum + "]</a>";

		sHtml += "<<tmp_note_orig_" + sId + ">>"; // 先做個記錄

		mOrigNote[sId] = sTmp;

		String sNoteText = parseChild(Node);
		// <div id="txt_note_orig_0001001">校勘內容</div>
		HTMLCollation += "<div id=\"txt_note_orig_" + sId + "\">" + sNoteText + "</div>\n";

	}
	if(sAttrType == u"mod")
	{
		if(sId == "") ShowMessage ("錯誤 : 校勘沒有 n 屬性");
		else sIdNum = NoteId2Num(sId);	// 0001001a 取得 1a

		sHtml += "<a id=\"note_mod_" + sId +
				 "\" class=\"note_mod\" href=\"\" onclick=\"return false;\">[" +
				 sIdNum + "]</a>";
		String sNoteText = parseChild(Node);
		HTMLCollation += "<div id=\"txt_note_mod_" + sId + "\">" + sNoteText + "</div>\n";

		String sIdNormal = sId.SubString0(0,7); // 取出標準的 ID, 因為有些有 abc...

		ThisNoteHasMod(sIdNormal);  // 通知 note orig , 此校勘有 mod 版

	}

	// parseChild(Node); // 處理內容

	return sHtml;
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
String __fastcall CCBXML::tag_p(_di_IXMLNode Node)
{

	String sHtml = "";
	sHtml = "<p><span class=\"parahead\">[" + PageLine + "] </span>";
	sHtml += parseChild(Node); // 處理內容
	sHtml += "</p>";

    // p 很多內容待處理 ????
	return sHtml;
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
String __fastcall CCBXML::tag_pb(_di_IXMLNode Node)
{
	String sHtml = "";
	sHtml = parseChild(Node); // 處理內容

	return sHtml;
}

// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_row(_di_IXMLNode Node)
{
	String sHtml = "";
	//String sXXX = GetAttr(Node, "xxx");
    CellNum = 0;        // cell 格式數量歸 0
	OtherColspan = 0;   // 因本 cell 佔 n 格以上, 所以和後面的 cell 要空 (n-1)*3 的空格, 此即記錄 n-1 的數字

	sHtml += u"<tr>";
	sHtml += parseChild(Node); // 處理內容
	sHtml += u"</tr>";

	return sHtml;
}
// ---------------------------------------------------------------------------
// <cb:sg> 要加括號 , <cb:tt><cb:t xml:lang="sa-Sidd"><g ref="#SD-E074">??</g></cb:t><cb:t xml:lang="zh"><cb:yin><cb:zi>唵</cb:zi><cb:sg>引</cb:sg></cb:yin></cb:t></cb:tt>
String __fastcall CCBXML::tag_sg(_di_IXMLNode Node)
{
	String sHtml = u"(";
	sHtml += parseChild(Node); // 處理內容
	sHtml += u")";
	return sHtml;
}
// ---------------------------------------------------------------------------
// 空格 <space quantity="1" unit="chars"/>
// 沒有字 <space quantity="0"/>
String __fastcall CCBXML::tag_space(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sUnit = GetAttr(Node, u"unit");

	if(sUnit != u"")
	{
		if(sUnit == u"chars")
		{
			String sQuantity = GetAttr(Node, u"quantity");
			int iSpace = sQuantity.ToIntDef(0);
			sHtml += String::StringOfChar(u'　',iSpace);
		}
	}
	else
	{
		// 隔行對照時, 沒有字的地方也要秀出空格, 以便與悉曇字對應
		if(true) // if(NextLineOfTT->InNextLineTT) // ???? 隔行對照待處理
			sHtml += u"　";
		else
			sHtml += u"";
	}

	//sHtml = parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_t(_di_IXMLNode Node)
{
	String sHtml = "";
	String sAttrPlace = GetAttr(Node, "place");
	if(sAttrPlace == u"foot") return "";

	sHtml = parseChild(Node); // 處理內容

	return sHtml;
}
// ---------------------------------------------------------------------------
// 表格的處理
// <table cols="4" border="0">
// <table cols="3" rend="border:0">
// <lb ed="X" n="0018b14"/><row><cell>壬寅</cell><cell></cell><cell cols="2">黃武元</cell></row>
// 依原書格式切行： 第一個<cell>空一格，<cell>空三格。<cell cols="n"> 結束後要再多空(n-1)*3格。
// 不依原書：border 為無線表格, <cell cols=2> 表示佔二個位置 => <td colspan="2"> => OtherColspan = 2-1
// <cell rows="n"> 依原書不管 rows , 不依原書則變成 <td rowspan="n">
String __fastcall CCBXML::tag_table(_di_IXMLNode Node)
{
	String sHtml = "";
	String sRend = GetAttr(Node, "rend");
	String sBorder = "1";      // 預設表格線為 1

	CRendAttr * myRend = new CRendAttr(sRend);
	if(myRend->Border != "") sBorder = myRend->Border;

	sHtml += u"<table border=\"";
	sHtml += sBorder;
	sHtml += u"\">";

	sHtml += parseChild(Node); // 處理內容

	sHtml += u"</table>";
	return sHtml;
}
// ---------------------------------------------------------------------------
// 用來判斷是否可用通用字 , 大於 0 就不用通用字,
// 這二種就不用通用字 <text rend="no_nor"> 及 <term rend="no_nor">
String __fastcall CCBXML::tag_term(_di_IXMLNode Node)
{
	String sHtml = "";
	String sRend = GetAttr(Node, u"rend");

	if(sRend == u"no_nor")  NoNormal++;
	sHtml += parseChild(Node); // 處理內容
	if(sRend == u"no_nor")  NoNormal--;

	return sHtml;
}
// ---------------------------------------------------------------------------
// 後面的名字, 當段落用 <trailer><title>般若波羅蜜多心經</title></trailer>
String __fastcall CCBXML::tag_trailer(_di_IXMLNode Node)
{
	String sHtml = "";
	sHtml = u"<p>";
	sHtml += parseChild(Node); // 處理內容
    sHtml += u"</p>";
	return sHtml;
}
// ---------------------------------------------------------------------------
// 百品新的標記, 表示猜測字 <unclear reason="damage" cert="high"></unclear>
// 依據猜測程度 , cert 有如下內容 高:high 中高:above_medium 中:medium 低:low
// <unclear/> 無法辨別的字, 呈現 ▆
String __fastcall CCBXML::tag_unclear(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sCert = GetAttr(Node, u"cert");

	if(sCert == u"high")
	{
		sHtml += u"<span class=\"guess1\" title=\"本字為推測字，信心程度：高\">";
	}
	else if(sCert == u"above_medium")
	{
		sHtml += u"<span class=\"guess2\" title=\"本字為推測字，信心程度：中高\">";
	}
	else if(sCert == u"medium")
	{
		sHtml += u"<span class=\"guess3\" title=\"本字為推測字，信心程度：中\">";
	}
	else if(sCert == u"low")
	{
		sHtml += u"<span class=\"guess4\" title=\"本字為推測字，信心程度：低\">";
	}

	if(Node->HasChildNodes)
		sHtml += parseChild(Node); // 處理內容
	else
		sHtml += u"<span title=\"未知的文字\">▆";

	sHtml += u"</span>";
	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_rdg(_di_IXMLNode Node)
{
	String sHtml = "";
	//sHtml = parseChild(Node); // 處理內容

	return sHtml;
}
// ---------------------------------------------------------------------------
// 南傳經文的巴利藏對照頁數
// <ref target="#PTS.Vin.3.1"></ref>
// 呈現 [P.1]
// 實際上則是 <span class="linehead" title="PTS.Vin.3.1">[P.1]</span>

// 不過在各卷最前面, 可能有一個隱形的標記, 記錄著上一卷最後一個 PTS 頁碼, 這個就不要呈現出來
// 它的格式是 <ref target="#PTS.Vin.3.109" type="PTS_hide"></ref>
String __fastcall CCBXML::tag_ref(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sType= GetAttr(Node, u"type");
	String sTarget= GetAttr(Node, u"target");

	bool bHidePTS = false;  // 判斷是不是隱藏版的 PTS 標記
	if(sType == u"PTS_hide") bHidePTS = true;

	if(sTarget != u"")
	{
		if(sTarget.SubString0(0,4) == u"#PTS")
		{
			sTarget.Delete(0,1);
			String sPage = sTarget;
			int iPos = sPage.LastDelimiter0(u".");	// 找到最後.的位置
			sPage.Delete0(0,iPos+1);   // 最後一個數字, 也就是頁碼

			// 隱藏的加要寫? 可能是引用複製要用的吧, 我也忘了....
			String sMsg = u"<span class=\"pts_head\" title=\"" + sTarget + "\">";
			if(bHidePTS == false)
				sMsg += u" [P." + sPage + u"] ";
			sMsg += u"</span>";
			sHtml += sMsg;
		}
	}
	sHtml += parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
// 儲存至 HTML
void __fastcall CCBXML::SaveToHTML(String sFile)
{
	TFile::WriteAllText(sFile, HTMLText, TEncoding::UTF8);
}
// ---------------------------------------------------------------------------
// 取得屬性
inline String __fastcall CCBXML::GetAttr(_di_IXMLNode Node, String sAttr)
{
	//return Node->HasAttribute(sAttr) ? Node->Attributes[sAttr] : "";

	if(Node->HasAttribute(sAttr))
		return Node->Attributes[sAttr];
	else
		return "";

}
// ---------------------------------------------------------------------------
// 把校勘ID 變成校勘數字 0001001 -> 1 , 0001001a -> 1a
String __fastcall CCBXML::NoteId2Num(String sId)
{
	if(sId == "") return "";

	//int iLen = sId.Length();

	int iStdLen;
	String::iterator it = sId.begin();
	it += 4; // 跳過頁碼

	while(*(it) == '0')
	{
		it++;
    }

	return String(it);
}

//---------------------------------------------------------------------------
// 讀取缺字資料
//---------------

/*
<charDecl>
    一般缺字
	<char xml:id="CB00166">
		<charName>CBETA CHARACTER CB00166</charName>
		<charProp>
			<localName>composition</localName>
			<value>[月*庸]</value>
		</charProp>
		<charProp>
			<localName>normalized form</localName>
			<value>傭</value>
		</charProp>
		<mapping type="unicode">U+267DB</mapping>
		<mapping cb:dec="983206" type="PUA">U+F00A6</mapping>
	</char>

    悉曇字

	<char xml:id="SD-A6A9">
		<charName>CBETA CHARACTER SD-A6A9</charName>
		<charProp>
			<localName>Romanized form in CBETA transcription</localName>
			<value>haa</value>
		</charProp>
		<charProp>
			<localName>Character in the Siddham font</localName>
			<value>扣</value>
		</charProp>
		<charProp>
			<localName>Romanized form in Unicode transcription</localName>
			<value>h?</value>
		</charProp>
		<mapping cb:dec="1066665" type="PUA">U+1046A9</mapping>
	</char>

    有 big5 對映字的悉曇字 , see <localName>big5</localName>

    <char xml:id="SD-E35A">
		<charName>CBETA CHARACTER SD-E35A</charName>
		<charProp>
			<localName>big5</localName>
			<value>（</value>
		</charProp>
		<charProp>
			<localName>Character in the Siddham font</localName>
			<value>劄</value>
		</charProp>
		<mapping cb:dec="1082202" type="PUA">U+10835A</mapping>
	</char>

    蘭札字

    <char xml:id="RJ-C947">
		<charName>CBETA CHARACTER RJ-C947</charName>
		<charProp>
			<localName>Romanized form in CBETA transcription</localName>
			<value>aa.h</value>
		</charProp>
		<charProp>
			<localName>rjchar</localName>
			<value>乇</value>
		</charProp>
		<charProp>
			<localName>Romanized form in Unicode transcription</localName>
			<value>??</value>
		</charProp>
		<mapping cb:dec="1100103" type="PUA">U+10C947</mapping>
	</char>

    .............
    .............
</charDecl>
*/

void __fastcall CCBXML::ReadGaiji(_di_IXMLNode NodeGaijis)
{


/*
    IXMLDOMNode * xmlNodeGaiji;
    IXMLDOMNode * xmlNodeTmp;
    BSTR xmlNodeName;
    BSTR xmlNodeText;
    IXMLDOMNamedNodeMap * xmlNodeAttributes;
    AnsiString sCBCode;
    AnsiString sTmp;

    long listLength;
	NodeGaijis->get_length(&listLength);
*/

	for (int i = 0; i < NodeGaijis->ChildNodes->Count; i++)
	{
		_di_IXMLNode NodeGaiji = NodeGaijis->ChildNodes->Get(i);

		// 逐一處理每一筆

		// <char xml:id="CB00166">
		String sCBCode = GetAttr(NodeGaiji, "xml:id");

		if(!sCBCode.IsEmpty())
		{
			String sType = CMyStrUtil::SubString(sCBCode,0,2);  // CB or SD or RJ

			for (int j = 0; j < NodeGaiji->ChildNodes->Count; j++)
			{
				_di_IXMLNode Node = NodeGaiji->ChildNodes->Get(j);

				if(Node->GetNodeName() == "charProp")
				{
					String sLocalName = Node->ChildNodes->Nodes["localName"]->GetText();
					String sValue = Node->ChildNodes->Nodes["value"]->GetText();

					// 組字式
					if(sLocalName == "composition")
						CB2des[sCBCode] = sValue;
					// 查詢通用字
					if(sLocalName == "normalized form")
						CB2nor[sCBCode] = sValue;
					// 悉曇字、蘭札字
					// 羅馬轉寫字 (純文字)
					if(sLocalName == "Romanized form in CBETA transcription")
						SD2roma[sCBCode] = sValue;
					// 羅馬轉寫字 (unicode)
					if(sLocalName == "Romanized form in Unicode transcription")
						SD2uni[sCBCode] = sValue;
					// char (以 ttf 字型呈現)
					if(sLocalName == "Character in the Siddham font")
						SD2char[sCBCode] = sValue;
					if(sLocalName == "rjchar")
						SD2char[sCBCode] = sValue;
					// 對映的 Big5 字
					if(sLocalName == "big5")
						SD2big5[sCBCode] = sValue;
				}
				else if(Node->GetNodeName() == "mapping")
				{
					String sType = GetAttr(Node, "type");
					String sText = Node->GetText();

					// <mapping type="unicode">U+267DB</mapping>
					// Unicode
					if(sType == "unicode")
					{
						unsigned long ul = CMyStrUtil::StrToULong(CMyStrUtil::SubString(sText,2),16);
						CB2uni[sCBCode] = CMyStrUtil::LongToUnicode(ul);
					}
					// <mapping type="normal_unicode">U+25873</mapping>
					// 通用 Unicode
					if(sType == "normal_unicode")
					{
						unsigned long ul = CMyStrUtil::StrToULong(CMyStrUtil::SubString(sText,2),16);
						CB2nor_uni[sCBCode] = CMyStrUtil::LongToUnicode(ul);
					}
				}
			}
		}
	}

}
// ---------------------------------------------------------------------------
// 通知 note orig , 此校勘有 mod 版
// 就會把 orig note 中 class 的 note_mod 移除
void __fastcall CCBXML::ThisNoteHasMod(String sIdNormal)
{
	if(mOrigNote.find(sIdNormal) != mOrigNote.end())
	{
		mOrigNote[sIdNormal] = StringReplace(mOrigNote[sIdNormal],
		u"class=\"note_orig note_mod\"", u"class=\"note_orig\"", TReplaceFlags());
	}
}
// ---------------------------------------------------------------------------
// 原本的 orig 校勘還沒加入, 此時才要加入
String __fastcall CCBXML::AddOrigNote(String HTMLText)
{
	vector<System::WideChar> vOut;

	System::WideChar * pPoint = HTMLText.FirstChar();

	// 標記用的 <<tmp_note_orig_xxxxxxx>>
	while(*pPoint)
	{
		if(*pPoint != u'<' && *(pPoint+1) != u'<')
		{
			vOut.push_back(*pPoint);
			pPoint++;
		}
		else
		{
			// 也許找到校勘記號
			String sNote = String(pPoint,16);
			if(sNote == u"<<tmp_note_orig_")
			{
				// 找到校勘記號
				String sId = String(pPoint+16,7);
				pPoint+=25;
				String sNote = mOrigNote[sId];  // 取出真正的校勘

				System::WideChar * pNote = sNote.FirstChar();
				while(*pNote)
				{
					vOut.push_back(*pNote);
					pNote++;
				}
			}
			else
			{
				// 不是校勘記號
				vOut.push_back(*pPoint);
				pPoint++;
            }
		}
	}

	String sOut = String(&(vOut[0]),vOut.size());
	return sOut;
}
// ---------------------------------------------------------------------------
// 由經名取得一切相關資訊
void __fastcall CCBXML::GetInitialFromFileName()
{
	// 檔名是 T01n0001_001.xml
	String sPatten = "([^\\\\/\\d]+)(\\d+)n(.*?)_(\\d\\d\\d)\\.xml";
	TRegEx *regex;
	TMatchCollection mycoll;
	TGroupCollection mygrps;

	regex = new TRegEx();
	mycoll = regex->Matches(XMLFile, sPatten);

	if(mycoll.Count != 1)   // 失敗
	{
		ShowMessage(L"檔名格式不正確, 無法分析");
		return;
    }

	mygrps = mycoll.Item[0].Groups;

	BookId = mygrps.Item[1].Value;		// 內容是 'T'(大正藏), 'X'(卍續藏)
	VolId = mygrps.Item[2].Value;      // 內容是 01
	SutraId = mygrps.Item[3].Value;		// 內容是 "0001" or "0143a"
	SutraId_ = SutraId;
	if(SutraId.Length() == 4) SutraId_ += L"_";
	JuanNum = String(mygrps.Item[4].Value).ToInt();		// 第幾卷
	BookVolnSutra = BookId + VolId + "n" + SutraId_;	// 內容是 T01n0001_

	// 由 c://xxx/xxx/xml/T/T01/T01n0001_001.xml
	// 找出這個主要目錄 c://xxx/xxx/


	SerialPath = regex->Replace(XMLFile, "[\\\\/][^\\\\/]+?[\\\\/][^\\\\/]+?[\\\\/][^\\\\/]+?[\\\\/][^\\\\/]+?$", "/");
}
// ---------------------------------------------------------------------------

