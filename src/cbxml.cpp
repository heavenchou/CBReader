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
	IsDebug = false;
	// 初值
	XMLFile = sFile;
	Setting = cSetting;
	HTMLText = "";
	HTMLCollation = "";    // HTML 校註
	JSFile = sJSFile;
	ShowHighlight = bShowHighlight;    // 是否塗色
	mOrigNote.clear();

	SerialPath = u"";    // 系列(書)的主要位置
	BookVerName = u"";
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

	FuWenCount = 0;			// 用來判斷是否是附文, 因為有巢狀, 所以用 int
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
	NoteAddNum = 0;     // 自訂校註 <note type="add" 的流水號, 由 1 開始

	NextLine = new CNextLine;		// 用來處理隔行 <tt> 的物件

	// 處理 XML -----------------------------------------------------------

	Document = interface_cast<Xmlintf::IXMLDocument>(new TXMLDocument(NULL));
	Document->FileName = XMLFile;
	Document->ParseOptions = Document->ParseOptions + (TParseOptions() << poPreserveWhiteSpace);

	HTMLText += MakeHTMLHead(); // 先產生 html 的 head

	// 是否有塗色? 有就表示有檢索字串
	if(bShowHighlight)
	{
		// 加上搜尋字串的 <div>
		HTMLText += u"<div id=\"SearchHead\">檢索字串：" + seSearchEngine->SearchSentence + u"<hr></div>";
    }

	HTMLText += ParseXML();     		// 處理內文

	HTMLText = AddOrigNote(HTMLText);   // 原本的 orig 校勘還沒加入, 此時才要加入
	HTMLCollation = AddOrigNote(HTMLCollation);

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
		"var gotoline = \"#" + sLink + "\";"
		"</script>\n";
		//"location.href=\"#" + sLink + "\";\n"
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

	String sJqueryFile = StringReplace(JSFile, u"cbreader.js", "jquery.min.js", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
	String sHtml = u"<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
	"	<meta charset=\"utf-8\">\n"
	"	<title>";

	sHtml += BookId + SutraId + u" " + SutraName;

	sHtml += u"</title>\n";
	sHtml += u"	<script src=\"";
	sHtml += sJqueryFile;
	sHtml += u"\"></script>\n"
	"	<script src=\"";
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
	"		table {border-collapse: collapse;}\n";

	if(Setting->VerticalMode)
		sHtml += u"		body {writing-mode: tb-rl;}\n";

    // 行首格式
	if(Setting->ShowLineFormat)
				  // 原書
		sHtml += u"		div {display:inline;}\n"
				  "		p {display:inline;}\n"
                  "		br.lb_br {display:inline;}\n"
				  "		br.para_br {display:none;}\n"
				  "		p.juannum {display:inline; margin-left:0em;}\n"    // 經號
				  "		p.headname2 {display:inline; margin-left:0em;}\n"    // head 標題
				  "		p.headname3 {display:inline; margin-left:0em;}\n"    // head 標題
				  "		p.headname4 {display:inline; margin-left:0em;}\n"    // head 標題
				  "		p.byline {display:inline; margin-left:0em;}\n"    // byline
				  "		table {border-style: none;}\n"
				  "		td {padding: 0px;}\n"
				  "		span.line_space {display:inline;}\n"  // 行首空格
				  "		span.para_space {display:none;}\n";  // 行首空格
//				  "     table {display: inline; line-height:2px; border-style: none}\n"
//				  "     tbody {display: inline;}\n"
//				  "     tr {display: inline;}\n"
//				  "     td {display: inline; padding: 0px;}\n"
//				  "     ul {display:inline;padding-left:0;}\n"     	// <ul>
//				  "     li {display:inline;}\n";     // <li>
	else
				  // 段落格式
		sHtml += u"		div {display:block;}\n"
				  "		p {display:block;}\n"
				  "		br.lb_br {display:none;}\n"
				  "		br.para_br {display:inline;}\n"
				  "		p.juannum {display:block; margin-left:2em;}\n"  // 經號
				  "		p.headname2 {display:block; margin-left:2em;}\n"    // head 標題
				  "		p.headname3 {display:block; margin-left:3em;}\n"    // head 標題
				  "		p.headname4 {display:block; margin-left:4em;}\n"    // head 標題
				  "		p.byline {display:block; margin-left:4em;}\n"    // byline
				  "		table {border-style: solid;border-collapse: collapse;}\n"
				  "		td {padding: 10px;}\n"
				  "		span.line_space {display:none;}\n"     // 行首空格
				  "		span.para_space {display:inline;}\n";     // 行首空格
//				  "     table {display: table; line-height:20px; border-style: solid}\n"
//				  "     tbody {display: table-row-group;}\n"
//				  "     tr {display: table-row;}\n"
//				  "     td {display: table-cell; padding: 10px;}\n"
//				  "     ul {display:block;padding-left:40;}\n"     	// <ul>
//				  "     li {display:list-item;}\n";     // <li>

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
	if(Setting->ShowCollation == false)
		sHtml += u"		.note_orig {display:none;}\n"
				  "		.note_mod {display:none;}\n"
				  "		.note_add {display:none;}\n"
				  "		.note_star {display:none;}\n"
				  "		.note_star_removed {display:none;}\n";
	else if(Setting->CollationType == ctOrigCollation)
		sHtml += u"		.note_mod {display:none;}\n"
				  "		.note_orig {display:inline;}\n"
				  "		.note_add {display:none;}\n"
				  "		.note_star {display:inline;}\n"
				  "		.note_star_removed {display:inline;}\n";
	else if(Setting->CollationType == ctCBETACollation)
		sHtml += u"		.note_orig {display:none;}\n"
				  "		.note_mod {display:inline;}\n"
				  "		.note_add {display:inline;}\n"
				  "		.note_star {display:inline;}\n"
				  "		.note_star_removed {display:none;}\n";

	sHtml += u"	</style>\n</head>\n";
	sHtml += u"<body data-sutraname='" + SutraName
		+ u"' data-juan='" + JuanNum
		+ u"' data-totaljuan='" + TotalJuan + u"'";

	// data-notetype 用來判斷目前是呈現何種校註
	if(Setting->ShowCollation == false)
		sHtml += u" data-notetype='none'";
	else if(Setting->CollationType == ctOrigCollation)
		sHtml += u" data-notetype='orig'";
	else if(Setting->CollationType == ctCBETACollation)
		sHtml += u" data-notetype='cbeta'";

	sHtml += u">\n";
	return sHtml;
}
// ---------------------------------------------------------------------------
// 解析 XML , 儲存到 TreeRoot 中
String __fastcall CCBXML::ParseXML()
{
	_di_IXMLNode Node;
	Document->Active = true;
	String sHtml = u"";

	// 讀取缺字
	_di_IXMLNode NodeGaijis = Document->DocumentElement->ChildNodes->Nodes["teiHeader"]->ChildNodes->Nodes["encodingDesc"]->ChildNodes->Nodes["charDecl"];
	ReadGaiji(NodeGaijis); // 讀取缺字

	// 遍歷 XML

	Node = Document->DocumentElement->ChildNodes->Nodes["text"];

	if (Node->ChildNodes->Count == 0)
		TDialogService::ShowMessage(u"錯誤：找不到 text 標記。");
	else
		sHtml = ParseNode(Node);

	return sHtml;
}
// ---------------------------------------------------------------------------
// 解析 XML Node
String __fastcall CCBXML::ParseNode(_di_IXMLNode Node)
{
	String sHtml = u"";

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
		else if (sTagName == u"cb:div")		sHtml = tag_div(Node);
		else if (sTagName == u"cb:docNumber")	sHtml = tag_docNumber(Node);
		else if (sTagName == u"entry")		sHtml = tag_entry(Node);
		else if (sTagName == u"figDesc")	sHtml = tag_figdesc(Node);
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
		else if (sTagName == u"cb:tt")		sHtml = tag_tt(Node);
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
	String sHtml = u"";

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
// 校勘中的校勘 , 這個要處理 ????
// <app from="#beg0101004" to="#end0101004">
//    <lem wit="#wit1">得法眼淨。猶如淨潔白
//        <app n="0101005">
//            <lem wit="#wit1">疊</lem>
//            <rdg resp="#resp1" wit="#wit2 #wit3 #wit4"><g ref="#CB00626">?</g></rdg>
//        </app>
//    </lem>
//    <rdg resp="#resp1" wit="#wit5">諸法法眼生譬如新衣</rdg>
// </app>

// tt 中的校勘
// <cb:tt type="app" from="#beg0001026" to="#end0001026">
// 	<cb:t resp="#resp1" xml:lang="zh"><app n="0001026"><lem wit="#wit1">拘樓孫</lem><rdg resp="#resp1" wit="#wit2 #wit3 #wit4">拘留孫</rdg></app></cb:t>
// 	<cb:t resp="#resp1" xml:lang="pi" place="foot">Kakusandha.</cb:t>
// </cb:tt>

// 星號
// <app from="#beg_1" to="#end_1" corresp="#0001004"><lem wit="#wit1">辨</lem><rdg resp="#resp1" wit="#wit2">辯</rdg></app>

// ????? lem 裡面要有 note 及 lb 才行. note 是 app 包 app 用的

// 以上是舊版資料, 待研究...

/*
<note n="0001002" resp="Taisho" type="orig" place="foot text">〔長安〕－【宋】</note>
<note n="0001002" resp="CBETA" type="mod">長安【大】，〔－〕【宋】</note>
<app n="0001002">
	<lem wit="【大】">長安</lem>
	<rdg resp="Taisho" wit="【宋】"><space quantity="0"/></rdg>
</app>

【標準校註】
XML 原始標記  : <app n="0001002">
HTML 經文轉成 : <span id="note_app_0001002" class="note_app">
HTML 校註轉成 : <div id="txt_note_app_0001002"> ... </div>

【CBETA 自訂校註】
XML 原始標記  : <app n="0001b0201">
HTML 經文轉成 : <span id="note_app_A1" class="note_app">
HTML 校註轉成 : <div id="txt_note_app_A1"> ... </div>

【星號】
XML 原始標記  :
<app type="star" corresp="#0001004">
	<lem wit="【大】">辨</lem>
	<rdg resp="Taisho" wit="【宋】">辯</rdg>
</app>

HTML 經文轉成 :
<a id="note_star_0001004-1" class="note_star" href="" onclick="return false;">[＊]</a>
<span id="note_app_0001004-1" class="note_app">....</span>

HTML 校註轉成 :
<div id="txt_note_app_0001004-1">
	<div type="orig">辨</div>
	<div type="lem" data-wit="【大】">辨</div>
	<div type="rdg" data-wit="【宋】">辯</div>
</div>

【移除的星號】

XML 原始標記  : T01n0001_003.xml
<note n="0021b2101" resp="CBETA" type="add">琉璃【CB】【麗-CB】，瑠璃【大】，流離【聖】</note>
<app type="star_removed" corresp="#0021019">
	<lem resp="CBETA.say" wit="【CB】【麗-CB】">琉璃...</lem>
	<rdg wit="【大】">瑠璃</rdg>
	<rdg resp="Taisho" wit="【聖】">流離</rdg>
</app>

HTML 經文轉成 :
<a id="note_add_A1" class="note_add" href="" onclick="return false;">[A1]</a>
<a id="note_star_0021019-1" class="note_star_removed" href="" onclick="return false;">[＊]</a>
<span id="note_app_0021019-1" class="note_app">琉璃</span>

HTML 校註轉成 :
<div id="txt_note_add_A1">琉璃【CB】【麗-CB】，瑠璃【大】，流離【聖】</div>
<div id="txt_note_app_0021019-1">
	<div type="orig">【大正為趣】</div>
	<div type="lem" data-wit="【CB】">【CB為越】</span></div>
	<div type="rdg" data-wit="【大】">趣</div>
	<div type="rdg" data-wit="【宋】【元】【明】">越</div>
</div>
*/

String __fastcall CCBXML::tag_app(_di_IXMLNode Node)
{
	String sHtml = u"";

	String sId = GetAttr(Node, u"n");
	String sType = GetAttr(Node, u"type");

	if(sId != u"" && mpNoteAddNum.count(sId)>0)
	{
		// 表示是 <note type="add"> 的 CBETA 自訂 <app>
		// 取回流水號, 把 Id 改成 Axx
		String sIdNum = String(Get_Add_IdNum(sId));
		sId = u"A" + sIdNum;
	}

	if(sType == u"star")
	{
		// 星號

		String sId = GetAttr(Node, u"corresp");
		sId.Delete0(0,1);   // remove '#'
		int iStar = 1;
		if(mpNoteStarNum.count(sId)>0)
			iStar =  mpNoteStarNum[sId] + 1;    // 星號加 1
		else
			iStar =  1; 	// 第一個星號
		mpNoteStarNum[sId] = iStar;
		sId += u"-" + String(iStar);

		HTMLCollation += u"<div id=\"txt_note_app_" + sId + u"\">\n";
		sHtml += u"<a id=\"note_star_" + sId + u"\" class=\"note_star\" "
			   + u"href=\"\" onclick=\"return false;\">[＊]</a>";
		sHtml += u"<span id=\"note_app_" + sId + u"\" class=\"note_app\">"
				 + parseChild(Node) // 處理內容
				 + u"</span>";
		HTMLCollation += u"</div>\n";
	}
	else if(sType == u"star_removed")
	{
		// 移除的星號

		String sId = GetAttr(Node, u"corresp");
		sId.Delete0(0,1);   // remove '#'
		int iStar = 1;
		if(mpNoteStarNum.count(sId)>0)
			iStar =  mpNoteStarNum[sId] + 1;    // 星號加 1
		else
			iStar =  1; 	// 第一個星號
		mpNoteStarNum[sId] = iStar;
		sId += u"-" + String(iStar);

		HTMLCollation += u"<div id=\"txt_note_app_" + sId + "\">\n";
		sHtml += u"<a id=\"note_star_" + sId + u"\" class=\"note_star_removed\" "
			   + u"href=\"\" onclick=\"return false;\">[＊]</a>";
		sHtml += u"<span id=\"note_app_" + sId + u"\" class=\"note_app\">"
			  + parseChild(Node) // 處理內容
			  + u"</span>";
		HTMLCollation += u"</div>\n";
	}
	else
	{
    	// 這裡要注意順序
		// 1. 先處理校註區的 <div id="txt_note_app_xxxx">
		// 2. 進行 parseChild , 這裡面會處理校註區 lem ,rdg 的內容
		// 3. 再加上校註區的 </div>

		HTMLCollation += u"<div id=\"txt_note_app_" + sId + "\">\n";
		sHtml += u"<span id=\"note_app_" + sId + "\" class=\"note_app\">"
				 + parseChild(Node) // 處理內容
				 + u"</span>";
		HTMLCollation += u"</div>\n";
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
 // <byline cb:type="Translator">
String __fastcall CCBXML::tag_byline(_di_IXMLNode Node)
{
	String sHtml = u"";
	if(ListCount == 0) InByline = true;

	sHtml = parseChild(Node); // 處理內容

	if(ListCount)
		sHtml = u"<span class=\"byline\">　" + sHtml + u"</span>";
	else
	{
		InByline = false;
		sHtml = u"<span class=\"line_space\">　　　　</span>"
				u"<p class=\"byline\" data-tagname='p'>" + sHtml + u"</p>";
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_cell(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sCols = GetAttr(Node, u"cols");
	String sRows = GetAttr(Node, u"rows");

	CellNum++;        // cell 格式數量累加
	String sColspan = u"";
	String sRowspan = u"";

	int iColspan = 0;
	if(sCols != u"")
	{
		iColspan = sCols.ToIntDef(1)-1;
		sColspan = u" colspan = \"" + sCols + u"\"";
	}

	if(sRows != u"")
	{
		sRowspan = u" rowspan = \"" + sRows + u"\"";
	}

	sHtml += u"<td";
	sHtml += sColspan;
	sHtml += sRowspan;
	sHtml += u" data-tagname='td'>";

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
	sHtml += u"</td>";

	return sHtml;
}
// ---------------------------------------------------------------------------
// 附文: <cb:div type="w"><p xml:id="pT02p0029b2201"><anchor xml:id="nkr_note_orig_0029009" n="0029009"/>
// 序: <cb:div type="xu"><cb:mulu level="1" type="序">序</cb:mulu><anchor xml:id="nkr_note_orig_0001001" n="0001001"/><head><title>長阿含經</title>序</head>
String __fastcall CCBXML::tag_div(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sType = GetAttr(Node, u"type");

	DivCount++;

	if(sType != u"")
	{
		DivType[DivCount] = sType.LowerCase();    // 都變成小寫
	}

	if(DivType[DivCount] == u"w")		// 附文
	{
		FuWenCount++;
		sHtml += u"<div class='w' data-tagname='div'>";   // 要用 div , 才不會有 span 包 p 的困境

		if(FuWenCount == 1)
		{
			if(Setting->ShowLineFormat)
				sHtml += u"<div data-margin-left='1em' data-tagname='div'>";
			else
				sHtml += u"<div data-margin-left='1em' style='margin-left: 1em' data-tagname='div'>";
			sHtml += u"<span class='line_space'>　</span>";
		}
	}
	else if (DivType[DivCount] == u"xu")		// 序文
	{
		sHtml += u"<div class='xu' data-tagname='div'>";   // 要用 div , 才不會有 span 包 p 的困境
	}

	// ----------------------------------
	sHtml += parseChild(Node); // 處理內容
	// ----------------------------------

	if(DivType[DivCount] == u"w")		// 附文
	{
		FuWenCount--;
		if(FuWenCount == 0)	// 大正藏切行
		{
			sHtml += u"</div>";
		}
		sHtml += u"</div>";       // 不切行則要用 div , 才不會有 span 包 p 的困境

	}
	else if(DivType[DivCount] == "xu")		// 序
	{
		sHtml += u"</div>";
	}

	DivCount--;

	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_docNumber(_di_IXMLNode Node)
{
	String sHtml = u"";

	sHtml += u"<span class=\"line_space\">　　</span>";
	sHtml += u"<p class=\"juannum\" data-tagname='p'>";

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
		delete myRend;

		MarginLeft += String::StringOfChar(u'　',iMarginLeft);// 這是第二行之後要空的
		sTextIndentSpace += String::StringOfChar(u'　',iMarginLeft+iTextIndent);
	}

	// place="inline" 要加三個空格
	if(sPlace == u"inline")			// 行中段落加句點
	{
		sTextIndentSpace = u"　　　";
	}

	if(Setting->ShowLineFormat)
	{
		sHtml += u"<div class=\"entry\" style=\"text-indent: ";
		sHtml += String(iTextIndent);
		sHtml += u"em\" data-margin-left=\"";
		sHtml += String(iMarginLeft);
		sHtml += u"em\" data-tagname='div'>";
	}
	else
	{
		sHtml += u"<div class=\"entry\" style=\"text-indent: ";
		sHtml += String(iTextIndent);
		sHtml += u"em; margin-left: ";
		sHtml += String(iMarginLeft);
		sHtml += u"em\" data-margin-left=\"";
		sHtml += String(iMarginLeft);
		sHtml += u"em\" data-tagname='div'>";
	}

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
String __fastcall CCBXML::tag_figdesc(_di_IXMLNode Node)
{
	return u"";
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
	String sHtml = u"<p data-tagname='p'>";
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
	String sHtml = u"";
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
// 缺字 <g ref="#CB00166"/>
// 悉曇字 T20n1034 <g ref="#SD-A440"/>
// 蘭札字 T21n1419 <g ref="#RJ-C947"/>
String __fastcall CCBXML::tag_g(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sCBCode = GetAttr(Node, u"ref");
	sCBCode = sCBCode.Delete0(0,1);
	String sCBCodeType = sCBCode.SubString0(0,2);

	if(sCBCodeType == u"CB")
	{
		sHtml = u"<span class=\"gaiji\" data-id=\"" + sCBCode + u"\"";

		// 組字式
		String sDes = CB2des[sCBCode];
		if(!sDes.IsEmpty())
		{
			sHtml += u" data-des=\"" + sDes + u"\"";
		}
		// 通用字
		String sNor = CB2nor[sCBCode];
		if(!sNor.IsEmpty() && NoNormal == 0)    // 有 NoNormal 就不使用通用字
		{
			sHtml += u" data-nor=\"" + sNor + u"\"";
		}
		// Unicode
		String sUni = CB2uni[sCBCode];
		if(!sUni.IsEmpty())
		{
			sHtml += u" data-uni=\"" + sUni + u"\"";
		}
		// Normal Unicode
		String sNorUni = CB2nor_uni[sCBCode];
		if(!sNorUni.IsEmpty())
		{
			sHtml += u" data-noruni=\"" + sNorUni + u"\"";
		}
		// 圖檔

		String sGaijiFile = SerialPath + u"gaiji-CB/" + sCBCode.SubString0(2,2) + u"/"
							+ sCBCode + u".gif";
		sGaijiFile = ExpandFileName(sGaijiFile);
		sHtml += u" data-imgfile=\"" + sGaijiFile + u"\">";

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
				else if(TFile::Exists(sGaijiFile))
					sHtml += u"<img src=\"" + sGaijiFile + u"\"/>";
				else
					sHtml += u"<span title=\"此缺字無法呈現\">⍰</span>";
			}
			else
			{
				// 圖檔優先 , 暫時不用 ????
				if(TFile::Exists(sGaijiFile))
					sHtml += u"<img src=\"" + sGaijiFile + u"\"/>";
				else if(!sDes.IsEmpty()) sHtml += sDes;
				else
					sHtml += u"<span title=\"此缺字 " + sCBCode + u" 無法呈現\">⍰</span>";
            }

        }

		// 結束標記
		sHtml += "</span>";
	}
	// 悉曇字
	else if(sCBCodeType == u"SD" || sCBCodeType == u"RJ")
	{
		// 設定還沒寫 ????
		int iShowSiddamWay = Setting->ShowSiddamWay;

		// 特殊格式, 若格式是 4 自訂, 且文字是 S(R) , 就用特殊格式

		bool bSpecial = false;   // 200702 之後的加強版
		if(iShowSiddamWay == 6)  bSpecial = true;                                           // 200802 版

		// 悉曇字處理法 0:悉曇字型(siddam.ttf)
		if(iShowSiddamWay == 0 || bSpecial)
		{
			if(SD2char[sCBCode] != u"")
			{
				if(sCBCodeType == u"SD")  // 悉曇字型
					sHtml += u"<font face=\"siddam\">";
				else if(sCBCodeType == u"RJ")  // 悉曇字型
					sHtml += u"<font face=\"Ranjana\">";
				sHtml += SD2char[sCBCode];
				sHtml += u"</font>";
			}
			else
				iShowSiddamWay = 3;		// 沒有 sdchar 就先換圖檔

		}

		// 悉曇字處理法 1:羅馬轉寫(unicode)
		if(iShowSiddamWay == 1 || bSpecial)
		{
			String sUnicode;
			String sBig5;

			sUnicode = SD2uni[sCBCode];		// 悉曇
			sBig5 = SD2big5[sCBCode];

			if(sUnicode != u"")
			{
				sUnicode = u"<span class=\"nonhan\">" + sUnicode + u"</span>";
				if(bSpecial)    // 特殊格式, 給悉曇網用的, 200802 加入
					sUnicode = u"(" + sUnicode + u")";
				sHtml += sUnicode;
				sHtml += u" ";
			}
			else if(sBig5 != "")
			{
				// 有些悉曇字有 （　）　…　這些文字, 這些在悉曇字與轉寫字同時呈現時, 不用再呈現一次
				if(!bSpecial) sHtml += sBig5;
			}
			else if(!bSpecial)
				iShowSiddamWay = 3;		// 沒有 big5 就先換圖檔
		}
		// 悉曇字處理法 2:羅馬轉寫(純文字) -- 2018 新版不提供了

		// 悉曇字處理法 3:悉曇圖檔
		if(iShowSiddamWay == 3)
		{
			// sCBCode = SD-A440

			String sSDGifOrigFile;
			String sSDGifFile;

			if(sCBCodeType == u"SD")
			{
				sSDGifOrigFile = SerialPath + u"sd-gif/" + sCBCode.SubString0(2,2) + u"/"
							+ sCBCode + u".gif";
			}
			else if(sCBCodeType == u"RJ")
			{
				sSDGifOrigFile = SerialPath + u"rj-gif/" + sCBCode.SubString0(2,2) + u"/"
							+ sCBCode + u".gif";
			}
			sSDGifFile = ExpandFileName(sSDGifOrigFile);

			// 如果圖檔不存在, 就用 SD 碼
			if(TFile::Exists(sSDGifFile))
			{
				sHtml += u"<img src=\"";
				sHtml += sSDGifFile;
				sHtml += u"\"/>";
			}
			else
				iShowSiddamWay = 5;		// 沒有圖檔就換 SD 碼
		}
		// 悉曇字處理法 4:自訂符號 -- 2018 不提供了
		// 悉曇字處理法 5:CB碼     -- 2018 不提供了
		if(iShowSiddamWay == 5)
		{
			if(sCBCodeType == u"SD")
				sHtml += u"<span title=\"此悉曇字 " + sCBCode + u" 無法呈現\">⍰</span>";
			else if(sCBCodeType == u"RJ")
				sHtml += u"<span title=\"此蘭札字 " + sCBCode + u" 無法呈現\">⍰</span>";
        }
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
// 圖檔 <figure><graphic url="../figures/T/T18p0146_01.gif"></graphic></figure>
String __fastcall CCBXML::tag_graphic(_di_IXMLNode Node)
{
	String sHtml = u"";
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
	String sHtml = u"";
	// 處理標記

	IsFindLg = false;       // 因為 IsFindLg 是判斷 <l> 是不是第一個, 若是就不空格, 但有一種是 <lg><head>...</head><l> , 所以這也不算第一個 <l>

	String sType = GetAttr(Node, u"type");

	if(sType == u"add")
	{
		// 額外的, 不處理 ,
		// 只出現在 T06, T07 <head type="added">No. 220</head>,
		// T21 <head type="added">No. 1222</head>
		return u"";
    }

	if(sType == u"pin")     // 品名
	{
		InPinHead = true;
	}
	else if(sType == u"other")    // 附文標題
	{
		InOtherHead = true;
	}

	// head 是否在 list 中待處理
	_di_IXMLNode ParentNode = Node->ParentNode;
	String sParentNodeName = u"";
	if(ParentNode != NULL)
	{
		sParentNodeName = ParentNode->GetNodeName();
	}

	if(sParentNodeName == u"list")
	{
			// list 的規則參考 list 的項目
			sHtml += u"<span class=\"headname\">";
	}
	else
	{
		// 設定 head 待處理

		if(DivType[DivCount] == u"pin")          // 品的標題
			InPinHead = true;
		else if(DivType[DivCount] == u"w")		// 附文的標題
			InFuWenHead = true;
		else if(DivType[DivCount] == u"other")   // 其它的標題
			InOtherHead = true;
		else
			InHead = true;

		// Q1 ==> 空2格
		// Q2 ==> 空3格
		// Q3 ==> 空4格
		// Q4 ==> 空2格
		// Q5 ==> 空3格
		// Q6 ==> 空4格
		// Q7 ==> 空2格
		// Q8 ==> 空3格

		// 原本不應該有 0 , 但有時 head 不在 div 中, 就會有 0 了
		if(DivCount == 0 || DivCount % 3 == 1)
			sHtml += u"<span class=\"line_space\">　　</span>"
					  "<p class=\"headname2\" data-tagname='p'>";
		else if(DivCount % 3 == 2)
			sHtml += u"<span class=\"line_space\">　　　</span>"
					  "<p class=\"headname3\" data-tagname='p'>";
		else if(DivCount % 3 == 0)
			sHtml += u"<span class=\"line_space\">　　　　</span>"
					  "<p class=\"headname4\" data-tagname='p'>";
	}

	sHtml += parseChild(Node); // 處理內容
	// 結束標記

	if(sType == u"pin")     // 品名
	{
		InPinHead = false;
	}
	else if(sType == u"other")    // 附文標題
	{
		InOtherHead = false;
	}

	if(sParentNodeName == u"list")
	{
		// 詳細說明參考 list 的規則
		sHtml += u"</span>";
	}
	else
	{
		if(DivType[DivCount] == u"pin")          // 品的標題
			InPinHead = false;
		else if(DivType[DivCount] == u"w")		// 附文的標題
			InFuWenHead = false;
		else if(DivType[DivCount] == u"other")   // 其它的標題
			InOtherHead = false;
		else
			InHead = false;

		sHtml += u"</span>";
		sHtml += u"</p>";
	}

	if(sParentNodeName == u"list")
	{
		// 詳細說明參考 list 的規則
		sHtml += u"<ul data-tagname='ul'>";
	}

	return sHtml;
}
// ---------------------------------------------------------------------------
String __fastcall CCBXML::tag_item(_di_IXMLNode Node)
{
	String sHtml = u"";

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

        sHtml += u"<li data-tagname='li'>";
		//if(Setting->CutLine)	// 大正藏切行
		{
			String sItemId = GetAttr(Node, u"xml:id");

			// 第一個, 或在行首, 就要依 list 數量 * 2 來空格
			if(ItemNum[ListCount] == 1 || sPreNodeName == u"lb" || sPreChildNodeName == u"list")
            {
				//itemX63p0502b0319 是特例
				// 在切換校註呈現時如何處理? 待研究 ????
				//if(Setting->CorrSelect == 0 && sItemId == u"itemX63p0502b0319"){}
				//else
					sHtml += u"<span class=\"line_space\">" + String::StringOfChar(u'　',ListCount*2) + u"</span>";
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
	String sHtml = u"";

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
	String sHtml = u"";

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
		delete myRend;
		// l 標記應該不處理 LgMarginLeft 比較好吧
		//if(iMarginLeft)
		//	LgMarginLeft += String::StringOfChar(u'　',myRend->MarginLeft);  // lg 整段要空的格
	}
	else
	{
		//   type="normal" (預設): 第一個 <l> 不空格, 其餘 <l> 空二格, 有 rend 的 <l> 依 rend 處理.
		//   type="abnormal" : <l> 皆不空格, 有 rend 的 <l> 依 rend 處理.

		if(LgNormal)
		{
			if(LTagNum == 1)
			{
				iMarginLeft = 0;
				LMarginLeft = u"";
			}
			else
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
			
			// lg 之後的第一個 <l> , 不處理空格, 都由 lg 處理了
			// 之後每行第一個 <l> 都要先印出整段偈頌要空的空格
			if(IsFindLg)		
			{
				IsFindLg = false;
			}
			else
			{
				// 在 2016 之前, 標準或非標準的偈頌, 不依原書時, 第一個 <l> 都會折行.
				// 目前的新規則, 只要是不依原書, 非標準偈頌, 且有設定 Setting->LgType = 1 ,
				// 就不折行. 不過這只限在 GA 及 GB, 因為舊的經文還是折行較好

				// 也就是 【GA 或 GA 的非標準偈頌不折行】, 否則就折行
				if(!(LgNormal == false && (BookId == u"GA" || BookId == u"GB")))
				{
					sHtml += u"<br class=\"para_br\" data-tagname='br'/>";	// 偈頌折行
					//sHtml += "<span class=\"para_space\">" + LgMarginLeft + "</span>";
				}

				// 標準偈頌時, 整段的空格由 <l> 處理, 因為 <l> 會折行, 要折行後才能空
				// 非標準偈頌, 則由 <lb> 去空
				if(LgNormal) sHtml += LgMarginLeft;	// 整段偈頌要空的空格
			}
		}
	}
	
	sLTextIndent += String::StringOfChar(u'　',iMarginLeft + iTextIndent); 
	// l 本身要空的格
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
// 有二種情況要忽略
// 1. 卍續藏有二種 lb , ed 不是 "X" 的要忽略 : <lb ed="R150" n="0705a03"/>
// 2. 印順導師也有二種 lb , type="old" 要忽略 : <lb n="a003a06" ed="Y" type="old"/>

String __fastcall CCBXML::tag_lb(_di_IXMLNode Node)
{
	String sHtml = u"";

	// 取出屬性
	String sType = GetAttr(Node, u"type");
	String sEd = GetAttr(Node, u"ed");
	String sN = GetAttr(Node, "n");

	// Debug

	if(sN == u"0755c15")
	{
		int deb = 10;
		deb++;
		IsDebug = true;
	}



	// 印順導師著作有 type="old" 要忽略
	if(sType == u"old") return u"";

	// 如果 ed 屬性不是本書, 則忽略, 主要是卍續藏是 X, 但有 ed="Rxxx" 的情況
	if(sEd != BookId) return u"";

	// 因為換行了, 所以隔行對照一定要輸出, 因為有時還在 <t> 當中. 所以 NextLine->InNextLine要先清除, 例如: T18n0864A.xml
	// <lb n="0200a22"/><p type="dharani">....<tt><t lang="san-sd">&SD-E040;</t><t lang="chi"><yin><zi>南</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-DA42;</t><t lang="chi">唵<note place="inline">若有祈請所求一切事者
	// <lb n="0200a23"/>
	// <lb n="0200a24"/>於此應加孔雀王陀羅尼</note></t></tt><tt><t lang="san-sd">&SD-E0EF;</t><t lang="chi"><yin><zi>部嚕唵</zi><sg>三合</sg></yin></t></tt></p>
	// <lb n="0200a25"/>

	bool bTmp = NextLine->InNextLine;
	NextLine->InNextLine = false;
	NextLine->IsOutput = true;			// 表示所有的隔行對照都可以輸出了

	// 若有隔行對照, 則要將先將上行的印出來

	if(NextLine->ThisLine != u"")
	{
		sHtml += u"<br class=\"para_br\" data-tagname='br'/>";	// 原書切行
		sHtml += NextLine->ThisLine;
		NextLine->ThisLine = "";
	}

	LTagNum = 0;		// 還原 <l> 的數目, 要來判斷要不要折行或空格數目

	// 判斷要不要強迫切行
	// <lb ed="X" n="0070b01" type="honorific"/><lb ed="R150" n="0706a17"/>御製序文。闡揚宗淨合一之旨。
	// <lb ed="X" n="0070b02" type="honorific"/><lb ed="R150" n="0706a18"/>高宗純皇帝南巡。親詣雲棲。拈香禮佛。

	bool bForceCutLine = false;		// 強迫切行
	if(sType == u"honorific")	bForceCutLine = true;


	// 判斷此行是不是空白行
	bool bNoLineData = false;
	_di_IXMLNode NextSiblNode = GetNextSiblNode(Node);
	String sNextSiblNodeName = u"";
	if(NextSiblNode != NULL)
	{
		#ifdef _Windows
			TNodeType nodetype = NextSiblNode->NodeType;
		#else
			Xml::Internal::Omnixml::TNodeType nodetype = NextSiblNode->NodeType;
		#endif

		if(nodetype == 3) // 這是純 data
		{
			if(NextSiblNode->Text == u"\n")
			{
				// 可能空白行, 往下查是不是 lb 標記

				_di_IXMLNode NextSiblNode2 = GetNextSiblNode(NextSiblNode);  // 取得下一層

				if(NextSiblNode2 != NULL)
				{
					nodetype = NextSiblNode2->NodeType;
					if(nodetype == 1) // 這是純 element
					{
						String sNextSiblNode2Name = NextSiblNode2->GetNodeName();
						if(sNextSiblNode2Name == u"lb" || sNextSiblNode2Name == u"pb")
							bNoLineData = true;
					}
				}
			}
		}
		else if(nodetype == 1) // 這是純 element
		{
			sNextSiblNodeName = NextSiblNode->GetNodeName();
			if(sNextSiblNodeName == u"lb" || sNextSiblNodeName == u"pb")
				bNoLineData = true;
		}
	}

	// 原書格式會看到的行首空白
	String sSpace;
	if(LgNormal)
		sSpace = MarginLeft;	// + LgMarginLeft + LMarginLeft; 這些留在 <l> 才空格
	else
		sSpace = MarginLeft + LgMarginLeft + LMarginLeft;
	{
		if(InByline)				sSpace += u"　　　　";	// 譯者
		if(FuWenCount && !bNoLineData)	sSpace += u"　";			// 附文
		if(InSutraNum)				sSpace += u"　　";		// 經號

		if(InPinHead || InFuWenHead || InOtherHead || InHead)		// 品名, 附文標題, 其它標題
		{
			if(DivCount == 0 || DivCount % 3 == 1)
				sSpace += u"　　";
			else if(DivCount % 3 == 2 )
				sSpace += u"　　　";
			else if(DivCount % 3 == 0)
				sSpace += u"　　　　";
		}

		// 下一個不是 list 或 item 才要空 item 的空格
		if(ListCount > 0 && sNextSiblNodeName != u"list" && sNextSiblNodeName != u"item")
		{
			sSpace += String::StringOfChar(u'　',ListCount*2);	// Item 的空格
		}
	}

	// 產生行首資訊

	PageLine = sN;
	LineHead = BookVolnSutra + u"p" + PageLine + u"║";
	if(PreFormatCount || bForceCutLine) // 強迫指定依原書
		sHtml += u"<br/>";
	else if (NextLine->NextLine != u"") // 隔行對照, 所以要 <br>
		sHtml += u"<br/>";
	else
		sHtml += u"<br class=\"lb_br\" data-tagname='br'/>";
	sHtml += u"<a \nname=\"p" + PageLine + "\"></a><span class=\"linehead\">" + LineHead + "</span>";

	// 加入品資料, 引用複製會用到
	// 待處理: 引用複製在品的位置也要處理 ?????

	//if(MuluLabel != u"")
		// T21n1251_p0233a27 有品名有缺字造成的問題, 待處理 ????
		//sHtml += u"<a pin_name=\"" + MuluLabel + "\"></a>";

	// 印出行首空格

	if(PreFormatCount || bForceCutLine) // 強迫指定依原書就不輸 span 標記
		sHtml += sSpace;
	else
		sHtml += u"<span class=\"line_space\">" + sSpace + u"</span>";

	// 若有隔行對照, 則要將將下一行的印出來

	if(NextLine->NextLine != u"")
	{
		sHtml += NextLine->NextLine;
		NextLine->NextLine = u"";
	}

	NextLine->InNextLine = bTmp;

	// sHtml += parseChild(Node); // 處理內容

	return sHtml;
}
// ---------------------------------------------------------------------------
/*
<note n="0001002" resp="Taisho" type="orig" place="foot text">〔長安〕－【宋】</note>
<note n="0001002" resp="CBETA" type="mod">長安【大】，〔－〕【宋】</note>
<app n="0001002">
	<lem wit="【大】">長安</lem>
	<rdg resp="Taisho" wit="【宋】"><space quantity="0"/></rdg>
</app>

XML 原始標記  : <lem wit="【大】">長安</lem>
HTML 經文轉成 : 長安
HTML 校註轉成 : <div type="lem" data-wit="【大】">長安</div>

有原始版的 lem 或 rdg 還要轉出一行, 讓 javascript 好處理
				<div type="orig">XXX</div>

若選擇原書, lem 的 wit 有【大】(或其他藏經的原書版本), 則呈現 lem 的內容.
			lem 的 wit 沒有【大】(或其他藏經的原書版本), 則不呈現 lem 的內容, 等 rdg .
若選擇CBETA 版, 則呈現 lem 的內容, 因為若有 CB 版, 一定在 lem 中.

*/
String __fastcall CCBXML::tag_lem(_di_IXMLNode Node)
{
	String sHtml = u"";

	String sWit = GetAttr(Node, "wit");

	bool bShowLemText = true;
	bool bLemIsOrig = false;   // 判斷 lem 是否是原始校註

	if(sWit.Pos0(BookVerName) >= 0) bLemIsOrig = true;

	// 選擇原書校註, 但 lem 不是原始校註, 就不呈現文字
	if(Setting->CollationType == ctOrigCollation)
		if(bLemIsOrig == false) bShowLemText = false;

	String sLemText = parseChild(Node); // 處理內容

	if(bShowLemText) sHtml += sLemText;

	// 記錄到校註區

	// 有些有 type 屬性要處理 : T18n0848 : <rdg resp="Taisho" wit="【丙】" type="variantRemark">乘</rdg>
    // ???? (lem , rdg 都要處理, 但大概是呈現全部校註文字的表格才會用上吧)

	String sLemTag = u"";
	if(bLemIsOrig)
		// <div type="orig">大正原版</div>
		sLemTag += u"\t<div type=\"orig\">" + sLemText + u"</div>\n";

	sLemTag += u"\t<div type=\"lem\" data-wit=\"" +
					sWit + "\">" + sLemText + u"</div>\n";

	HTMLCollation += sLemTag;
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

/* 空格處理法

整個偈頌都要空的假設為Ｌ

目前的設計是 lb 之後不要空，等標準偈頌遇到第一個 <l> 才空，如下所示

<lb><l>ＬｘｘｘｘＬＬｘｘｘｘ

為什麼不在 <lb> 之後就空呢? 因為在段落模式，<lb> 沒有換行，而 <l> 才有換行效果，
所以空格要在 <l> 之後，才不會先空格再換行，就白空了。

至於非標準偈頌 , 又分成二種：

1. 原書格式 : lb 之後就要空出整段的 , 多行的偈頌才會整齊

<lb>ＬＬＬxxxxxxxx
<lb>ＬＬＬxxxxxxxx
<lb>ＬＬＬxxxxxxxx

2. 段落格式

整段的空格用 <p style="margin:3em"> 來控制

行首的Ｌ在段落模式會隱藏，<p> 在原書模式也會消失作用。

/* 2018註 : 底下這一段怪怪的, 不看好了
	// 2016 的新作法:
	// 原本整段的空格是用空格. 例如 : <lg rend="margin-left:1"> , 2014 之前的版本是每一行前面都加上一個空格. 不過折行就不會空格了.
	// 2016 就改成用段落 <p class="lg" style="margin-left:1em;"> , 因此每一行前面就不用加空格.
	// 不過因此 copy 再貼在純文字, 就會少了行首的空格.
	// 至於引用複製, 就要在原本行首的空格加上 <spane data-space="1"> 表示空一格, 到時再用引用複製來還原一個空格.
*/

String __fastcall CCBXML::tag_lg(_di_IXMLNode Node)
{
	String sHtml = u"";

	IsFindLg = true;			// 一遇到 <lg> 就 true, 第一個 <l> 就會處理並設為 false;
	LgCount++;                  // 判斷是不是在 <lg> 之中, 主要是用來處理偈頌中的標點要不要呈現.
	LgNormal = true;			// 預設值, 因為有些舊的 xml 沒有 <lg type=normal>
	LgInline = false;      		// lg 的 place 是不是 inline?
	LgMarginLeft = u"";			// lg 整段要空的格
	LTagNum = 0;		        // 還原 <l> 的數目, 要來判斷要不要折行或空格數目

	bool bHasRend = false;		// 先假設沒有 rend 屬性
	String sLgTextIndent = u"";	// lg 開頭要空的格
	bool bIsNote = false;       // 若 type 是 note1 or note2 , 則偈誦前後要加小括號

	// 先處理 type 屬性

	String sType = GetAttr(Node, u"type");

	if(sType == u"normal") LgNormal = true;		// lg 的 type 是 normal
	if(sType == u"abnormal") LgNormal = false;    // 因為舊版有 type=inline
	if(sType == u"note1" || sType == u"note2")	// 在偈誦前後要加小括號
	{
		LgNormal = true;
		bIsNote = true;
	}

	// 處理 place="inline"
	// V2.0 之後, 由 type=inline 改成 place=inline

	String sPlace = GetAttr(Node, "place");
	if(sPlace == "inline")			// 行中段落加句點
	{
		LgInline = true;
		LgNormal = false;
	}

	// 再處理 rend 屬性

	String sRend = GetAttr(Node, u"rend");

	int iMarginLeft = 0;
	int iTextIndent = 0;
	// 檢查移位 <lg rend="margin-left:1">
	// 舊版的會有 <lg type="inline">或<lg rend="inline''> , 要改成 type=abnormal
	if(sRend != u""
		&& !(!Setting->ShowPunc && LgNormal)    //若不秀標點且是標準格式, 就不依 rend
		&& !(Setting->NoShowLgPunc && LgNormal))     //若在偈頌中且偈頌不秀新標
	{
		bHasRend = true;

		CRendAttr * myRend = new CRendAttr(sRend);
		iMarginLeft = myRend->MarginLeft;
		iTextIndent = myRend->TextIndent;
		if(myRend->IsInline)
		{
			LgNormal = false;
			LgInline = true;
		}
        // lg 整段要空的格
		LgMarginLeft = String::StringOfChar(u'　',iMarginLeft);
		delete myRend;
	}
	else
	{
		if(LgNormal)
		{
			iMarginLeft = 1;
			LgMarginLeft = u"　";	// 非 normal 的可能是 inline, 不一定要空格
		}
	}

	// 開頭要空的格
	sLgTextIndent = String::StringOfChar(u'　',iMarginLeft + iTextIndent);

	if(LgInline && LgMarginLeft == "" && !bHasRend)	// 即在行中, 又沒有空白, 前一個字也不是空白時, 就加上空白
	{
		// 檢查前一個字是不是空格?
		// 先不管了, 一律加上空格 ????
		sHtml += u"<span class=\"line_space\">　</span>";
	}

	if(LgNormal)
	{
		// 標準偈頌, 完全利用空格來處理, 不由 <p> 來控制縮排, copy 才會好看
		sHtml += u"<p data-tagname='p'>";
		sHtml += sLgTextIndent;
	}
	else
	{
		// 如果是不依原書, 且不是 normal 偈頌, 且指定用段落的方式 (LgTYpe = 1), 則處理成 <p style="margin-left::2em;text-indent:xxem;"><lg class="lg"> 這種格式
		if(iMarginLeft !=0 || iTextIndent != 0)
		{
			if(Setting->ShowLineFormat)
			{
				sHtml += u"<p";
				if(iTextIndent != 0)
				{
					sHtml += u" style=\"text-indent:";
					sHtml += String(iTextIndent);
					sHtml += u"em\"";
				}
				if(iMarginLeft != 0)
				{
					sHtml += u" data-margin-left=\"";
					sHtml += String(iMarginLeft);
					sHtml += u"em\"";
				}
				sHtml += u" data-tagname='p'>";

				sHtml += u"<span class=\"line_space\">";
				sHtml += sLgTextIndent;
				sHtml += u"</span>";

			}
			else
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
				sHtml += u"\"";
				if(iMarginLeft != 0)
				{
					sHtml += u" data-margin-left=\"";
					sHtml += String(iMarginLeft);
					sHtml += u"em\"";
				}
				sHtml += u" data-tagname='p'>";

				sHtml += u"<span class=\"line_space\">";
				sHtml += sLgTextIndent;
				sHtml += u"</span>";
			}
		}
		else
		{
			sHtml += u"<p data-tagname='p'>";
		}
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
	String sHtml = u"";

	ListCount++;
	ItemNum[ListCount] = 0; // 歸零

	bool bHasHead = false;

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
			if(ListCount == 1)
			{
            	// 好像不需要了
				//sHtml += u"<br class=\"para_br\"/><br class=\"para_br\"/>";     // 第一層才要
			}
			bHasHead = true;
		}
	}

	// 如果是 <list rendition="simple"> 要轉成 <ul style="list-style:none;">
	String sRendition = GetAttr(Node, "rendition");

	if(!bHasHead)
	{
		if(sRendition == u"simple")
			sHtml += u"<ul style=\"list-style:none;\" data-tagname='ul'>";
		else
			sHtml += u"<ul data-tagname='ul'>";
    }

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
		// 目錄有缺字, 所以先不呈現 T21n1251_p0233a27 ????
		//sHtml += sMulu;

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

	String sHtml = u"";
	String sType = GetAttr(Node, u"type");
	String sPlace = GetAttr(Node, u"place");
	String sId = GetAttr(Node, u"n");
	String sIdNum = u"";	// 0001001a 取得 1a

	// 沒有 Type 的情況

	if(sType == u"")
	{
		if(sPlace == u"inline" || sPlace == u"inline2" || sPlace == u"interlinear")
		{
			sHtml += u"<span class=\"note\">(" + parseChild(Node) + u")</span>";
			return sHtml;
		}

		// 有些連 place 也沒有, 例如 :
		// <note resp="CBETA.Eva">為配合悉漢對照，故將p727a14中文與p727a15悉曇字前後對調</note>

		if(sPlace == u"")	return u"";
	}

	// 卍續藏特殊校注
	// X56n0922 <note n="0104k01" resp="Xuzangjing" place="foot text" type="orig_ke">
	// 【科01】 → xml：<note n="0001k01" resp=" Xuzangjing " place="foot text" type="orig_ke">
	// 【標01】 → xml：<note n="0001b01" resp=" Xuzangjing " place="foot text" type="orig_biao">
	// 【解01】 → xml：<note n="0001j01" resp="Xuzangjing" place="foot text" type="orig_jie">

	// 原書校勘
	if(sType.SubString0(0,4) == u"orig")
	{
		if(sId == u"") TDialogService::ShowMessage (u"錯誤 : 校勘沒有 n 屬性");
		else sIdNum = NoteId2Num(sId);	// 0001001a 取得 1a

		String sKBJ = u"";  // 科, 標, 解專用
		System::WideChar wcKBJ = *(sId.FirstChar()+4);
		if(wcKBJ == u'k') sKBJ = u"科";
		else if(wcKBJ == u'b') sKBJ = u"標";
		else if(wcKBJ == u'j') sKBJ = u"解";

		// <a id="note_orig_0001001" class="note_orig" href="" onclick="return false;">

		// note 要暫存起來, 要同時有 note_orig 和 mod
		// 等到真的遇到 mod , 再把 class 的 note_mod 移除

		String sTmp = u"<a id=\"note_orig_" + sId +
				 u"\" class=\"note_orig note_mod\" href=\"\" onclick=\"return false;\">[" +
				 sKBJ + sIdNum + u"]</a>";

		sHtml += u"<<tmp_note_orig_" + sId + u">>"; // 先做個記錄

		mOrigNote[sId] = sTmp;

		String sNoteText = parseChild(Node);
		// <div id="txt_note_orig_0001001">校勘內容</div>
		HTMLCollation += u"<div id=\"txt_note_orig_" + sId + u"\">" + sNoteText + u"</div>\n";
	}
	else if(sType.SubString0(0,3) == u"mod")
	{
		if(sId == u"") TDialogService::ShowMessage (u"錯誤 : 校勘沒有 n 屬性");
		else sIdNum = NoteId2Num(sId);	// 0001001a 取得 1a

		String sKBJ = u"";  // 科, 標, 解專用
		System::WideChar wcKBJ = *(sId.FirstChar()+4);
		if(wcKBJ == u'k') sKBJ = u"科";
		else if(wcKBJ == u'b') sKBJ = u"標";
		else if(wcKBJ == u'j') sKBJ = u"解";

		sHtml += u"<a id=\"note_mod_" + sId +
				 u"\" class=\"note_mod\" href=\"\" onclick=\"return false;\">[" +
				 sKBJ + sIdNum + u"]</a>";
		String sNoteText = parseChild(Node);
		HTMLCollation += u"<div id=\"txt_note_mod_" + sId + u"\">" + sNoteText + u"</div>\n";

		//String sIdNormal = sId.SubString0(0,7); // 取出標準的 ID, 因為有些有 abc...

		ThisNoteHasMod(sId);  // 通知 note orig , 此校勘有 mod 版
	}

	// 2016 新增加的版本 <note type="editor" ...
	else if(sType == u"editor" || sType == u"add")
	{
		if(sId == u"") TDialogService::ShowMessage (u"錯誤 : 校勘沒有 n 屬性");
		else sIdNum = String(Get_Add_IdNum(sId));	// 取得自訂校勘的流水號

		// <a id="note_orig_0001001" class="note_orig" href="" onclick="return false;">

		// note 要暫存起來, 要同時有 note_orig 和 mod
		// 等到真的遇到 mod , 再把 class 的 note_mod 移除

		sHtml += u"<a id=\"note_add_A" + sIdNum +
				 u"\" class=\"note_add\" href=\"\" onclick=\"return false;\">[A" +
				 sIdNum + u"]</a>";

		String sNoteText = parseChild(Node);
		// <div id="txt_note_orig_0001001">校勘內容</div>
		HTMLCollation += u"<div id=\"txt_note_add_A" + sIdNum + u"\">" + sNoteText + u"</div>\n";
	}
	// 2018 新增加的版本 <note type="authorial" ...
	// Y13n0013 }<lb n="0303a11" ed="Y"/>
	// ...〈書<note type="authorial">（菊池寬）</note>復讎以後〉...
	else if(sType == u"authorial")
	{
		sHtml += parseChild(Node);
	}
	// N19n0007.xml
	// <note type="star" corresp="#0211020"></note>
	else if(sType == u"star")
	{
		String sCorresp = GetAttr(Node, u"corresp");
		sCorresp = String(sCorresp.begin()+1);
		String sTmp = u"<a id=\"note_star_" + sCorresp +
				 u"\" class=\"note_orig note_mod\" href=\"\" onclick=\"return false;\">[＊]</a>";
		sHtml += sTmp;
	}

	// parseChild(Node); // 處理內容

	return sHtml;
}

// ---------------------------------------------------------------------------
// 注意 : 原本若 XML P4 的 <p> 有 place="inline" , 則 inline 會加至 "rend" 屬性中,
//        其他 place 內容會變成 cb:type 屬性. 例如:
//        P4 <p place="inline"> => P5 <p rend="inline">
//        P4 <p place="head2"> => P5 <p cb:type="head2">

// 段落 : <p>
// 咒語 : <p cb:type="dharani">唵緊至囉
// 比照 head 處理 : <p xml:id="pX82p0160b2201" cb:type="head1">臨濟宗</p>
// 卍續特殊校注有 X14n0288  <p cb:type="各家會釋" rend="margin-left:1em"> , <p cb:type="訂解總論" ... >

String __fastcall CCBXML::tag_p(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sRend = GetAttr(Node, u"rend");
	String sType = GetAttr(Node, u"cb:type");
    #ifdef _Windows
		TNodeType nodetype;
	#else
		Xml::Internal::Omnixml::TNodeType nodetype;
	#endif

	String sTextIndentSpace = u"";
	String sOldMarginLeft = MarginLeft; // 先存起舊的
	int iMarginLeft = 0;
	int iTextIndent = 0;
	bool bHasRend = false;	    // 若有 rend 就不使用預設的空格
	bool bHasInline = false;    // 有沒有 inline
	bool bAddSpace = false;		// 要不要加空格, 有 inline 不一定加空格, 若前面有空格就不加
	bool bHasLg = false;	    // 用來判斷前面是不是 lg, 若是就要空二格了
	int  iSpecialType = 0;      // 0 : 一般情況 , 1 : cb:type ="各家會釋" , 2 : cb:type="訂解總論"

	// 處理 rend
	// 檢查移位 <p xml:id="pX78p0420a0401" rend="margin-left:2em">
	//          <p xml:id="pX78p0802a1901" rend="margin-left:1em;text-indent:-1em">
	//          <p xml:id="xxxxxxxxxxxxxx" rend="margin-left:1em;text-indent:-1em;inline">

	if(sRend != u"")
	{
		bHasRend = true;

		CRendAttr * myRend = new CRendAttr(sRend);
		iMarginLeft = myRend->MarginLeft;
		iTextIndent = myRend->TextIndent;

		// 處理 inline
		if(myRend->IsInline)
		{
			bHasInline = true;
			bAddSpace = true;		// 要不要加空格

			// 新的用法, 若 <p rend="inline"> 前無空格, 則依原書切行時都加上空格, 否則一律不加

			_di_IXMLNode PreSiblNode = Node->PreviousSibling();  	// 取得上一層
			if(PreSiblNode != NULL)
			{
				nodetype = PreSiblNode->NodeType;
				if(nodetype == 3) // 這是純 data
				{
					String sData = PreSiblNode->Text;

					if(*(sData.LastChar()) == u'　')
						bAddSpace = false;
				}
				else if(nodetype == 1) // 這是純 element
				{
					if(PreSiblNode->NodeName == u"lg") bHasLg = true;
				}
			}
		}
		delete myRend;
	}

	// 處理 cb:type
	// 這二種不理它 : cb:type="xx" , cb:type="interlinear"

	// 先處理卍續藏可能特有的 cb:type ="各家會釋" or cb:type="訂解總論"

	if(sType != u"")
	{
		if(sType == u"各家會釋")	// 行中段落加句點
		{
			iSpecialType = 1;		// 0 : 一般情況 , 1 : cb:type ="各家會釋" , 2 : cb:type="訂解總論"
		}
		if(sType == u"訂解總論")	// 行中段落加句點
		{
			iSpecialType = 2;		// 0 : 一般情況 , 1 : cb:type ="各家會釋" , 2 : cb:type="訂解總論"
		}
	}

	// ------------------------------------------------------
	// 順序 : <p><span...>　(空格) [pxxxxaxx]  字
	// ------------------------------------------------------

	// 處理 <p....>
	if(iSpecialType > 0)
	{
		// iSpecialType > 0 只會出現在校註
		sHtml += u"<p style=\"text-indent: ";
		sHtml += String(iTextIndent);
		sHtml += u"em; margin-left: ";
		sHtml += String(iMarginLeft);
		sHtml += u"em; margin-top: 5px; margin-bottom: 0em;\" data-tagname='p'>";

		if(iSpecialType == 1)
			sHtml += u"<font color=#800000>";
		else if(iSpecialType == 2)
			sHtml += u"<font color=blue>";
	}
	else
	{
		if(Setting->ShowLineFormat)
		{
			sHtml += u"<p style='text-indent: ";
			sHtml += String(iTextIndent);
			sHtml += u"em' data-margin-left='";
			sHtml += String(iMarginLeft);
			sHtml += u"em' data-tagname='p'>";
		}
		else
		{
			sHtml += u"<p style='text-indent: ";
			sHtml += String(iTextIndent);
			sHtml += u"em; margin-left: ";
			sHtml += String(iMarginLeft);
			sHtml += u"em' data-margin-left='";
			sHtml += String(iMarginLeft);
			sHtml += u"em' data-tagname='p'>";
		}
	}

	// 處理 <span ....> --------------------------------------------------

	if(sType == u"dharani")		// 咒
	{
		sHtml += u"<span class=\"dharani\">";
	}
	else if(sType == u"pre")	// 依原書格式
	{
		PreFormatCount++;	   // 判斷是否是要依據原始經文格式切行, 要累加的, 因為可能有巢狀的 pre
	}
	else if(sType.SubString0(0,4) == u"head")	// 比照 head 處理
	{
		//IsOtherHead = true;
		// Q1 ==> 空2格
		// Q2 ==> 空3格
		// Q3 ==> 空4格
		// Q4 ==> 空2格
		// Q5 ==> 空3格
		// Q6 ==> 空4格
		// Q7 ==> 空2格
		// Q8 ==> 空3格

		if(sType == u"head1" || sType == u"head4" || sType == u"head7")
		{
			iMarginLeft += 2;
			bAddSpace = false;
		}
		else if(sType == u"head2" || sType == u"head5" || sType == u"head8")
		{
			iMarginLeft += 3;
			bAddSpace = false;
		}
		else if(sType == u"head3" || sType == u"head6" || sType == u"head9")
		{
			iMarginLeft += 4;
			bAddSpace = false;
		}

		sHtml += u"<span class=\"headname\">";
	}


	// 卍續藏校注就不用段首了, 在校勘中也不用(藏外第一經第一卷就有例子)
	if(iSpecialType == 0)
	{
		// 處理空格及段首
		if(sTextIndentSpace == u"" && bAddSpace)
		{
			iTextIndent++;
			if(bHasLg) iTextIndent++;
		}
		MarginLeft += String::StringOfChar(u'　', iMarginLeft);
		String sSpace = String::StringOfChar(u'　', iMarginLeft + iTextIndent);

		sHtml += u"<span class=\"line_space\">";
		sHtml += sSpace;
		sHtml += u"</span>";

		// 處理段首
		if(sType.SubString0(0,4) != "head")	// 當成 head 就不要加行首資訊了
		{
			sHtml += u"<span class=\"parahead\">[" + PageLine + "] </span>";
		}
	}

	// -----------------------------------
	sHtml += parseChild(Node); // 處理內容
	// -----------------------------------

	//sHtml += "</p>";

	MarginLeft = sOldMarginLeft;


	if(sType == u"dharani")
	{
		sHtml += u"</span>";
	}
	else if(sType == u"pre")			// 依原書格式
	{
		PreFormatCount--;		// 判斷是否是要依據原始經文格式切行, 要累加的, 因為可能有巢狀的 pre
	}

	if(iSpecialType > 0) sHtml += u"</font>";
	sHtml += u"</p>";

	return sHtml;
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
String __fastcall CCBXML::tag_pb(_di_IXMLNode Node)
{
	String sHtml = u"";
	sHtml = parseChild(Node); // 處理內容

	return sHtml;
}

// ---------------------------------------------------------------------------
/*
<note n="0001002" resp="Taisho" type="orig" place="foot text">〔長安〕－【宋】</note>
<note n="0001002" resp="CBETA" type="mod">長安【大】，〔－〕【宋】</note>
<app n="0001002">
	<lem wit="【大】">長安</lem>
	<rdg resp="Taisho" wit="【宋】"><space quantity="0"/></rdg>
</app>

XML 原始標記  : <rdg resp="Taisho" wit="【宋】"><space quantity="0"/></rdg>
HTML 經文轉成 : (空的, 沒有字)
HTML 校註轉成 : <div type="rdg" data-wit="【宋】">長安</div>

有原始版的 lem 或 rdg 還要轉出一行, 讓 javascript 好處理
				<div type="orig">XXX</div>

若選擇原書, rdg 的 wit 有【大】(或其他藏經的原書版本), 則呈現 lem 的內容.
			rdg 的 wit 沒有【大】(或其他藏經的原書版本), 則不呈現 lem 的內容, 等 rdg .
若選擇CBETA 版, 則不呈現 rdg 的內容, 因為若有 CB 版, 一定在 lem 中.

*/
String __fastcall CCBXML::tag_rdg(_di_IXMLNode Node)
{
String sHtml = u"";

	String sWit = GetAttr(Node, "wit");

	bool bShowRdgText = false;
	bool bRdgIsOrig = false;   // 判斷 lem 是否是原始校註

	if(sWit.Pos0(BookVerName) >= 0) bRdgIsOrig = true;

	// 選擇原書校註, 且 rdg 是原始校註, 才呈現文字
	if(Setting->CollationType == ctOrigCollation)
		if(bRdgIsOrig == true) bShowRdgText = true;

	String sRdgText = parseChild(Node); // 處理內容

	if(bShowRdgText) sHtml += sRdgText;

	// 記錄到校註區


	String sRdgTag = u"";
	if(bRdgIsOrig)
		// <div type="orig">大正原版</div>
		sRdgTag += u"\t<div type=\"orig\">" + sRdgText + u"</div>\n";

	sRdgTag += u"\t<div type=\"rdg\" data-wit=\"" +
					sWit + "\">" + sRdgText + u"</div>\n";

	HTMLCollation += sRdgTag;
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
			sTarget.Delete0(0,1);
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
String __fastcall CCBXML::tag_row(_di_IXMLNode Node)
{
	String sHtml = u"";
	//String sXXX = GetAttr(Node, "xxx");
    CellNum = 0;        // cell 格式數量歸 0
	OtherColspan = 0;   // 因本 cell 佔 n 格以上, 所以和後面的 cell 要空 (n-1)*3 的空格, 此即記錄 n-1 的數字

	sHtml += u"<tr data-tagname='tr'>";
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
		if(true) // if(NextLine->InNextLine) // ???? 隔行對照待處理
			sHtml += u"　";
		else
			sHtml += u"";
	}

	//sHtml = parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
// 在校勘的巴利文不要印出來 <cb:t lang="pli" resp="Taisho" place="foot">S&amacron;vatth&imacron;.</cb:t>
// <cb:tt type="tr"><cb:t lang="chi">&lac;</cb:t><cb:t lang="san-sd">&SD-A5A9;</cb:t></cb:tt><cb:tt type="tr"><cb:t lang="chi">&lac;</cb:t><cb:t lang="san-sd">&SD-A5F0;</cb:t></cb:tt><cb:tt type="tr"><cb:t lang="chi">如</cb:t><cb:t lang="san-sd">&SD-CFCF;</cb:t></cb:tt>
// <cb:tt><cb:t lang="san-sd">&SD-A5A9;</cb:t><cb:t lang="chi">曩</cb:t></cb:tt>

/* 2014 年的類型
<cb:t cert="?" resp="#respx" xml:lang="pi" place="foot">
<cb:t cert="?" resp="#respx" xml:lang="sa" place="foot">
<cb:t resp="#respx" xml:lang="pi" place="foot">
<cb:t resp="#respx" xml:lang="sa" place="foot">
<cb:t resp="#respx" xml:lang="x-unknown" place="foot">
<cb:t resp="#respx" xml:lang="zh" place="foot">
<cb:t resp="#respx" xml:lang="zh">
<cb:t xml:lang="sa-Sidd" rend="margin-left:1em">
<cb:t xml:lang="sa-Sidd">
<cb:t xml:lang="sa-x-rj">
<cb:t xml:lang="zh">
<cb:t xml:lang="zh-x-yy" rend="margin-left:1em">
*/
String __fastcall CCBXML::tag_t(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sPlace = GetAttr(Node, u"place");
	String sRend = GetAttr(Node, u"rend");
	if(sPlace == u"foot") return u"";

	// 如果是隔行對照, 就要累加 <t> 的計數器
	if(NextLine->InNextLine)	NextLine->TCount = NextLine->TCount+1;

	if(sRend != u"")
	{
		CRendAttr * myRend = new CRendAttr(sRend);
		int iMarginLeft = myRend->MarginLeft;

		MarginLeft += String::StringOfChar(u'　',iMarginLeft);
		sHtml += MarginLeft;
		delete myRend;
	}
	// "<add_sp>" 是故意的, 在 TmyNextLineOfTT 物件處理
	else if(NextLine->InNextLine)
		// 在隔行對照時, 除非 rend = "" , 否則一律加上全型空格
		sHtml += u"<add_sp>";

	if(InTTNormal)
	{
		sHtml += u"<br class=\"para_br\" data-tagname='br'/>";
	}

	sHtml += parseChild(Node); // 處理內容

    // 判斷是不是在隔行對照
	if(NextLine->InNextLine || !NextLine->IsOutput)
	{
		NextLine->Add(sHtml);
		return u"";
	}
	else
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
	String sHtml = u"";
	String sRend = GetAttr(Node, u"rend");
	String sBorder = u"1";      // 預設表格線為 1

	CRendAttr * myRend = new CRendAttr(sRend);
	if(myRend->Border != "") sBorder = myRend->Border;
	delete myRend;

	sHtml += u"<table data-tagname=\"table\" border=\"";
	sHtml += sBorder;
	sHtml += u"\"><tbody data-tagname=\"tbody\">";

	sHtml += parseChild(Node); // 處理內容

	sHtml += u"</tbody></table>";

	//sHtml = mv_data_between_tr(sHtml);  // 把 <tr/>..<tr><td> 中間的資料移到 <td> 裡面

	return sHtml;
}
// ---------------------------------------------------------------------------
// 用來判斷是否可用通用字 , 大於 0 就不用通用字,
// 這二種就不用通用字 <text rend="no_nor"> 及 <term rend="no_nor">
String __fastcall CCBXML::tag_term(_di_IXMLNode Node)
{
	String sHtml = u"";
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
	String sHtml = u"";
	sHtml = u"<p data-tagname='p'>";
	sHtml += parseChild(Node); // 處理內容
    sHtml += u"</p>";
	return sHtml;
}
// ---------------------------------------------------------------------------
/* 處理 tt

	校勘產生的 tt		# <cb:tt type="app">	: 在 back 區中校勘中 <app> 裡面的 tt
    					# <cb:tt type="app" from="#begxxxxxx" to="#endxxxxxx"> : 在 back 區獨立出來的
    					# <cb:tt word-count="xx" type="app" from="#begxxxxxx" to="#endxxxxxx">

	同一行的 tt						# <cb:tt rend="inline">
	已經自動將漢字和悉曇分開的 tt	# <cb:tt rend="normal">

	這是要隔行對照的 tt 			# <cb:tt>
	這是要隔行對照的 tt 			# <cb:tt type="tr"> : 只出現在 T20n1168B


P5 校勘有點複雜, 尤其是有 <tt> 的
底下有三種, 一種是純粹 tt 的.
第二種是 back 區中的 tt 中還有 app , 但 app 中則無 tt , 這是因為在 P4 及 P5a 是 tt 包 app
第三種是 back 區中的 app 中還有 tt , 但 tt 中則無 app , 這是因為在 P4 及 p5a 是 app 包 tt
我想不管是哪一種, 原則上應該是只去找 app 中的 xml:id , back 中的 tt 就不管了.
除非是 app 中還有 tt , 則只要處理 tt 中 t 不是 place=foot 的部份

T01n0001.xml

<anchor xml:id="nkr_note_orig_0011012" n="0011012"/>
<anchor xml:id="beg0011012" n="0011012"/>禹舍
<anchor xml:id="end0011012"/>

<note n="0011012" resp="#resp1" type="orig" place="foot text" target="#nkr_note_orig_0011012">禹舍～Vassak?ra.</note>
<cb:tt type="app" from="#beg0011012" to="#end0011012">
	<cb:t resp="#resp1" xml:lang="zh">禹舍</cb:t>
	<cb:t resp="#resp1" xml:lang="pi" place="foot">Vassak?ra.</cb:t>
</cb:tt>

========================

T01n0001.xml

<anchor xml:id="nkr_note_orig_0039004" n="0039004"/>
<anchor xml:id="nkr_note_mod_0039004" n="0039004"/>
<anchor xml:id="beg0039004" n="0039004"/>摩羅醯搜
<anchor xml:id="end0039004"/>

<note n="0039004" resp="#resp1" type="orig" place="foot text" target="#nkr_note_orig_0039004">摩羅醯搜＝摩醯樓【宋】，摩羅醯樓【元】【明】～M?tul?.</note>
<note n="0039004" resp="#resp2" type="mod" target="#nkr_note_mod_0039004">摩羅醯搜～M?tul?.，＝摩醯樓【宋】，＝摩羅醯樓【元】【明】</note>

<app from="#beg0039004" to="#end0039004">
	<lem wit="#wit1">摩羅醯搜</lem>
	<rdg resp="#resp1" wit="#wit2">摩醯樓</rdg>
	<rdg resp="#resp1" wit="#wit3 #wit4">摩羅醯樓</rdg></app>

<cb:tt type="app" from="#beg0039004" to="#end0039004">
	<cb:t resp="#resp1" xml:lang="zh">
		<app n="0039004">
			<lem wit="#wit1">摩羅醯搜</lem>
			<rdg resp="#resp1" wit="#wit2">摩醯樓</rdg>
			<rdg resp="#resp1" wit="#wit3 #wit4">摩羅醯樓</rdg></app></cb:t>
	<cb:t resp="#resp1" xml:lang="pi" place="foot">M?tul?.</cb:t>
</cb:tt>

======================

T01n0026.xml

<anchor xml:id="nkr_note_orig_0680020" n="0680020"/>
<anchor xml:id="nkr_note_mod_0680020" n="0680020"/>
<anchor xml:id="beg0680020" n="0680020"/>婆羅婆
<anchor xml:id="end0680020"/>

<note n="0680020" resp="#resp1" type="orig" place="foot text" target="#nkr_note_orig_0680020">～Bh?radv?ja. 婆＝娑【聖】</note>
<note n="0680020" resp="#resp2" type="mod" target="#nkr_note_mod_0680020"><choice><corr>婆羅婆</corr><sic><space quantity="0"/></sic></choice>～Bh?radv?ja.，婆＝娑【聖】</note>

<app from="#beg0680020" to="#end0680020">
	<lem wit="#wit1">
		<cb:tt type="app">
			<cb:t resp="#resp2" xml:lang="zh">婆羅婆</cb:t>
			<cb:t resp="#resp1" xml:lang="pi" place="foot">Bh?radv?ja.</cb:t></cb:tt></lem>
<rdg resp="#resp1" wit="#wit12">娑羅婆</rdg></app>

<cb:tt type="app" from="#beg0680020" to="#end0680020">
	<cb:t resp="#resp2" xml:lang="zh">婆羅婆</cb:t>
	<cb:t resp="#resp1" xml:lang="pi" place="foot">Bh?radv?ja.</cb:t>
</cb:tt>

*/
String __fastcall CCBXML::tag_tt(_di_IXMLNode Node)
{
	String sHtml = u"";
	String sRend = GetAttr(Node, u"rend");
	String sType = GetAttr(Node, u"type");
	String sPlace = GetAttr(Node, u"place");

	if((sRend == u"" && sType == u"") || (sRend == u"" && sType == u"tr"))
		NextLine->FindNextLine();	// 這是隔行對照

    // 在 <cb:tt rend="normal"> 中, 這時每一個 <t> 都要換行 ,
	// T54n2133A : <lb n="1194c17"/><p><cb:tt rend="normal"><cb:t lang="san-sd">
	if(sRend == u"normal")	InTTNormal = true;

	sHtml = parseChild(Node); // 處理內容

	NextLine->FindNextLineEnd();
	InTTNormal = false;
	// 在 <tt rend="normal"> 中, 這時每一個 <t> 都要換行 ,
	// T54n2133A : <lb n="1194c17"/><p><tt rend="normal"><t lang="san-sd">

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
		return u"";

}
// ---------------------------------------------------------------------------
// 把校勘ID 變成校勘數字 0001001 -> 1 , 0001001a -> 1a
// 科標解的為 0001k01 -> 1
// 0004001-n01 -> 1 (B06n0003_p0004b18)
// 0561001-1 -> 1 (B13n0080_p0561a14)
String __fastcall CCBXML::NoteId2Num(String sId)
{
	if(sId == "") return "";

	//int iLen = sId.Length();

	int iStdLen;
	String::iterator it = sId.begin();
	String::iterator itend = sId.end();

	// 處理 - 號
	int iPos = sId.Pos0(u"-");
	if(iPos >= 0)
	{
		itend = it + iPos;
	}

	it += 4; // 跳過頁碼

	// 處理科標解的校勘

	if(*(it) == u'k' || *(it) == u'b' || *(it) == u'j')
		it++;

	while(*(it) == '0' && it != itend-1)
	{
		it++;
	}

	return String(it,itend-it);
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
void __fastcall CCBXML::ThisNoteHasMod(String sId)
{
	// 在 T27n1545 有 id = 0003002A 的校勘
	// 所以要同時處理 0003002A 與 0003002
	// 因為有時 mod 會有 a, b 還是要還原回 7 個字
	// 若有 xxxxxxxA , 就處理. 否則就處理 xxxxxxx 7 個字的版本.
	if(mOrigNote.find(sId) != mOrigNote.end())
	{
		mOrigNote[sId] = StringReplace(mOrigNote[sId],
		u"class=\"note_orig note_mod\"", u"class=\"note_orig\"", TReplaceFlags());
	}
	else if(sId.Length() > 7)
	{
		String sIdNormal = sId.SubString0(0,7);
		if(mOrigNote.find(sIdNormal) != mOrigNote.end())
		{
			mOrigNote[sIdNormal] = StringReplace(mOrigNote[sIdNormal],
			u"class=\"note_orig note_mod\"", u"class=\"note_orig\"", TReplaceFlags());
		}
    }
}
// ---------------------------------------------------------------------------
// 原本的 orig 校勘還沒加入, 此時才要加入
String __fastcall CCBXML::AddOrigNote(String HTMLText)
{
    if(HTMLText == u"") return u"";
	vector<System::WideChar> vOut;

	System::WideChar * pPoint = HTMLText.FirstChar();

	// 標記用的 <<tmp_note_orig_xxxxxxx>>
	while(*pPoint)
	{
		if(*pPoint != u'<' || *(pPoint+1) != u'<')
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
				String sId;

				int iShift = 23;

				while(*(pPoint+iShift) != u'>')
				{
					iShift++;
                }

				sId = String(pPoint+16, iShift-16);
				pPoint = pPoint + iShift + 2;

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
		TDialogService::ShowMessage(u"檔名格式不正確, 無法分析");
		return;
	}

	mygrps = mycoll.Item[0].Groups;

	BookId = mygrps.Item[1].Value;		// 內容是 'T'(大正藏), 'X'(卍續藏)
	VolId = mygrps.Item[2].Value;      // 內容是 01
	SutraId = mygrps.Item[3].Value;		// 內容是 "0001" or "0143a"
	SutraId_ = SutraId;
	if(SutraId.Length() == 4) SutraId_ += u"_";
	JuanNum = String(mygrps.Item[4].Value).ToInt();		// 第幾卷
	BookVolnSutra = BookId + VolId + u"n" + SutraId_;	// 內容是 T01n0001_

	int iIndex = fmMain->Bookcase->CBETA->Catalog->FindIndexBySutraNum(BookId,SutraId);
	SutraName = fmMain->Bookcase->CBETA->Catalog->SutraName->Strings[iIndex];
	TotalJuan = fmMain->Bookcase->CBETA->Catalog->JuanNum->Strings[iIndex].ToIntDef(0);

	// 由 c://xxx/xxx/xml/T/T01/T01n0001_001.xml
	// 找出這個主要目錄 c://xxx/xxx/


	SerialPath = regex->Replace(XMLFile, u"[\\\\/][^\\\\/]+?[\\\\/][^\\\\/]+?[\\\\/][^\\\\/]+?[\\\\/][^\\\\/]+?$", "/");

	// 取得版本, 例如 'T' 的版本是 【大】
	BookVerName = fmMain->Bookcase->CBETA->BookData->GetVerName(BookId);
	if(BookVerName == u"")   // 失敗
	{
		TDialogService::ShowMessage(u"版本名稱找不到");
		return;
	}
}
// ---------------------------------------------------------------------------
// 傳入 note 標記的 id , 傳回流水號, 若沒有就自動 + 1 並傳回
int __fastcall CCBXML::Get_Add_IdNum(String sId)
{
	map<String, int>::iterator it;
	it = mpNoteAddNum.find(sId);
	if(it != mpNoteAddNum.end())
	{
		// 找到了, 傳回流水號
		return it->second;
	}
	else
	{
		// 沒找到, 產生一個
		NoteAddNum++;
		mpNoteAddNum[sId] = NoteAddNum;
		return NoteAddNum;
    }
}
// ---------------------------------------------------------------------------
// 取得下一個 note , 但因為有一些是 <lb type=old> , <pb type=old> <lb id=Rxx> 要忽略
_di_IXMLNode __fastcall CCBXML::GetNextSiblNode(_di_IXMLNode Node)
{
	_di_IXMLNode NextSiblNode = Node->NextSibling();
	String sNextSiblNodeName = u"";
	if(NextSiblNode == NULL) return NextSiblNode;

	#ifdef _Windows
		TNodeType nodetype = NextSiblNode->NodeType;
	#else
		Xml::Internal::Omnixml::TNodeType nodetype = NextSiblNode->NodeType;
	#endif

	// 不是純 element , 傳回
	if(nodetype != 1) return NextSiblNode;

	// 這是純 element

	sNextSiblNodeName = NextSiblNode->GetNodeName();
	if(sNextSiblNodeName != u"lb" && sNextSiblNodeName != u"pb") return NextSiblNode;


	// 取出屬性
	String sType = GetAttr(NextSiblNode, u"type");
	String sEd = GetAttr(NextSiblNode, u"ed");
	String sN = GetAttr(NextSiblNode, "n");

	// 印順導師著作有 type="old" 要忽略
	if(sType == u"old")
	{
		NextSiblNode = GetNextSiblNode(NextSiblNode);
	}
	// 如果 ed 屬性不是本書, 則忽略, 主要是卍續藏是 X, 但有 ed="Rxxx" 的情況
	else if(sEd != BookId)
	{
		NextSiblNode = GetNextSiblNode(NextSiblNode);
	}

	return NextSiblNode;
}
// ---------------------------------------------------------------------------
// 把 <tr/>..<tr><td> 中間的資料移到 <td> 裡面
String __fastcall CCBXML::mv_data_between_tr(String sHtml)
{
    TRegEx *regex = new TRegEx();
	String sPattern = u"(<\\/tr>)(.*?)(<tr [^>]*>(<td [^>]*>))";
	if(IsDebug) ShowMessage(sHtml);
	String Out = regex->Replace(sHtml, sPattern, &TableTrReplace, TRegExOptions() << roSingleLine);
	if(IsDebug) ShowMessage(Out);
	return Out;
}
// ---------------------------------------------------------------------------
// 取代正規式 : 把 <tr/>..<tr><td> 中間的資料移到 <td> 裡面
String __fastcall CCBXML::TableTrReplace (const TMatch &Match)
{
	TMatch reMatch = Match; // 這行一定要, 不然會錯
	String a = reMatch.Groups.Item[1].Value;
	String b = reMatch.Groups.Item[2].Value;
	String c = reMatch.Groups.Item[3].Value;
	// 把二個括號交換
	if(IsDebug) ShowMessage(a);
	if(IsDebug) ShowMessage(b);
	if(IsDebug) ShowMessage(c);
	return a+c+b;
}
