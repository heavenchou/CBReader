// ---------------------------------------------------------------------------

#pragma hdrstop

#include "cbxml.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 建構函式
// 傳入參數為 XML 檔, 呈現的設定
__fastcall CCBXML::CCBXML(String sFile, CSetting * cSetting, String sJSFile , bool bShowHighlight, TmyMonster * seSearchEngine)
{
	// 初值
	XMLFile = sFile;
	Setting = cSetting;
	HTMLText = "";
	HTMLCollation = "";    // HTML 校註
	JSFile = sJSFile;
	ShowHighlight = bShowHighlight;    // 是否塗色
	mOrigNote.clear();

	GetInitialFromFileName();   // 由經名取得一切相關資訊

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

	if(true)
	{
		HTMLText += "<script>\n"
		"location.href=\"#heaven\";"
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

		if (sTagName == "app")		sHtml = tag_app(Node);
		else if (sTagName == "div")		sHtml = tag_div(Node);
		else if (sTagName == "g")		sHtml = tag_g(Node);
		else if (sTagName == "lb")		sHtml = tag_lb(Node);
		else if (sTagName == "lem")		sHtml = tag_lem(Node);
		else if (sTagName == "cb:mulu")	sHtml = tag_mulu(Node);
		else if (sTagName == "note")	sHtml = tag_note(Node);
		else if (sTagName == "p")		sHtml = tag_p(Node);
		else if (sTagName == "pb")		sHtml = tag_pb(Node);
		else if (sTagName == "rdg")		sHtml = tag_rdg(Node);
		else if (sTagName == "cb:t")	sHtml = tag_t(Node);
		else                      		sHtml = tag_default(Node);
	}
	else if (nodetype == 3)
	{
		// 文字節點
		sHtml = StringReplace(Node->Text, "\n", "", TReplaceFlags() << rfReplaceAll);
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
// 解析 XML 標記
String __fastcall CCBXML::tag_default(_di_IXMLNode Node)
{
	String sHtml = "";
	sHtml = parseChild(Node); // 處理內容
	return sHtml;
}
// ---------------------------------------------------------------------------
// 解析 XML 標記
String __fastcall CCBXML::tag_app(_di_IXMLNode Node)
{
	String sHtml = "";
	// 處理標記
	sHtml = parseChild(Node); // 處理內容
	// 結束標記

	return sHtml;
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
String __fastcall CCBXML::tag_div(_di_IXMLNode Node)
{
	String sHtml = "";
	// 讀取 href 屬性
	//String sAttr = Node->Attributes["href"];
	//if (sAttr != "")
	{
	}
	sHtml = parseChild(Node); // 處理內容

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
		if(!sNor.IsEmpty())
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
			else if(Setting->GaijiUseUniExt && Setting->GaijiUseNormal && !sNorUni.IsEmpty())
				// 通用 unicode 次之
				sHtml += sNorUni;
			else if(Setting->GaijiUseNormal && !sNor.IsEmpty())
				// 通用字最後
				sHtml += sNor;
			else bHasGaiji = false;
		}
		else
		{
			if(Setting->GaijiUseNormal && !sNor.IsEmpty())
				// 通用字優先
				sHtml += sNor;
			else if(Setting->GaijiUseUniExt && !sUni.IsEmpty())
				// unicode 次之
				sHtml += sUni;
			else if(Setting->GaijiUseUniExt && Setting->GaijiUseNormal && !sNorUni.IsEmpty())
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
// <lb>
// <lb n="0150b09" ed="T"/>
String __fastcall CCBXML::tag_lb(_di_IXMLNode Node)
{
	String sHtml = "";
	// 處理標記
	sHtml = "<br class=\"lb_br\"/>";

	String sn = GetAttr(Node, "n");
	PageLine = sn;
	LineHead = BookVolnSutra + "p" + PageLine + "║";
	sHtml += "<a\nname=\"" + PageLine + "\"></a><span class=\"linehead\">" + LineHead + "</span>";
	// sHtml += parseChild(Node); // 處理內容
	// 結束標記

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
// 目錄
String __fastcall CCBXML::tag_mulu(_di_IXMLNode Node)
{
	String sHtml = "";
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
// 解析 XML 標記
String __fastcall CCBXML::tag_t(_di_IXMLNode Node)
{
	String sHtml = "";
	String sAttrPlace = GetAttr(Node, "place");
	if(sAttrPlace == u"foot") return "";

	sHtml = parseChild(Node); // 處理內容

	return sHtml;
}

// ---------------------------------------------------------------------------
// 解析 XML 標記
String __fastcall CCBXML::tag_rdg(_di_IXMLNode Node)
{
	String sHtml = "";
	//sHtml = parseChild(Node); // 處理內容

	return sHtml;
}

// ---------------------------------------------------------------------------
// 儲存至 HTML
void __fastcall CCBXML::SaveToHTML(String sFile)
{
	TFile::WriteAllText(sFile, HTMLText, TEncoding::UTF8);
}
// ---------------------------------------------------------------------------
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
	"		.linehead {color:#0000A0; font-weight: normal; font-size:14pt;}\n"
	"		.parahead {color:#0000A0; font-weight: normal; font-size:14pt;}\n"
	"		.lg {color:#008040; font-weight: normal; font-size:16pt;}\n"
	"		.corr {color:#FF0000; font-weight: normal; }\n"
	"		.note {color:#9F5000; font-weight: normal; font-size:14pt;}\n";

    // 行首格式
	if(Setting->ShowLineFormat)
		sHtml += u"		p {display:inline;}\n";
	else
		sHtml += u"		p {display:block;}\n"
				  "		br.lb_br {display:none;}\n";

	if(Setting->ShowLineHead)
	{
		if(Setting->ShowLineFormat)
			sHtml += u"		.linehead {display:inline;}\n"
					  "		.parahead {display:none;}\n";
		else
			sHtml += u"		.linehead {display:none;}\n"
					  "		.parahead {display:inline;}\n";
	}
	else
	{
		sHtml += u"		.linehead {display:none;}\n"
				  "		.parahead {display:none;}\n";
    }


	// 校勘呈現
	if(Setting->CollationType == ctNoCollation)
		sHtml += u"		.note_orig {display:none;}\n"
				  "		.note_mod {display:none;}\n";
	else if(Setting->CollationType == ctOrigCollation)
		sHtml += u"		.note_mod {display:none;}\n"
				  "		.note_orig {display:inline;}\n";
	else if(Setting->CollationType == ctCBETACollation)
		sHtml += u"		.note_orig {display:none;}\n"
				  "		.note_mod {display:inline;}\n";

	sHtml += u"	</style>\n"
	"</head>\n"
	"<body>\n";

	return sHtml;
}
// ---------------------------------------------------------------------------
// 取得屬性
inline String __fastcall CCBXML::GetAttr(_di_IXMLNode Node, String sAttr)
{
	return Node->HasAttribute(sAttr) ? Node->Attributes[sAttr] : "";
}
// ---------------------------------------------------------------------------
// 把校勘ID 變成校勘數字 0001001 -> 1 , 0001001a -> 1a
String __fastcall CCBXML::NoteId2Num(String sId)
{
	if(sId == "") return "";

	int iLen = sId.Length();

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
	vector<wchar_t> vOut;

	wchar_t * pPoint = HTMLText.FirstChar();

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

				wchar_t * pNote = sNote.FirstChar();
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
}
// ---------------------------------------------------------------------------

