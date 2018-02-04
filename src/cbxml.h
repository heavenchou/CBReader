// ---------------------------------------------------------------------------

#ifdef _Windows
#include <Xml.Win.msxmldom.hpp>
#else
#include <Xml.omnixmldom.hpp>
#endif

#ifndef CBXMLH
#define CBXMLH
#include <fmx.h>
#include <Xml.XMLDoc.hpp>
#include <System.IOUtils.hpp>
#include <System.SysUtils.hpp>
#include <map>
#include "setting.h"
#include "subutil.h"
#include "highlight.h"
#include "../../Monster/src/monster.h"

using namespace std;
// ---------------------------------------------------------------------------
// 這是一個讀取導覽文件的物件

class CCBXML
{
private: // User declarations

	String XMLFile; // XML 檔名
	String HTMLText;	// HTML 的結果
	String HTMLCollation;    // HTML 校註
	CSetting * Setting; // 呈現用的設定
	String JSFile;  	// javascript 的位置

	_di_IXMLDocument Document;

	String __fastcall MakeHTMLHead(); 	// 先產生 html 的 head

	// --------------------------

	// 處理標記
	String __fastcall tag_app(_di_IXMLNode Node);
	String __fastcall tag_div(_di_IXMLNode Node);
	String __fastcall tag_g(_di_IXMLNode Node);
	String __fastcall tag_lb(_di_IXMLNode Node);
	String __fastcall tag_lem(_di_IXMLNode Node);
	String __fastcall tag_mulu(_di_IXMLNode Node);
	String __fastcall tag_note(_di_IXMLNode Node);
	String __fastcall tag_p(_di_IXMLNode Node);
	String __fastcall tag_pb(_di_IXMLNode Node);
	String __fastcall tag_rdg(_di_IXMLNode Node);
	String __fastcall tag_t(_di_IXMLNode Node);
	String __fastcall tag_default(_di_IXMLNode Node);

	// 處理 XML
	String __fastcall ParseXML(); // 解析 XML , 儲存到 TreeRoot 中
	String __fastcall ParseNode(_di_IXMLNode Node); // 解析 XML Node
	String __fastcall parseChild(_di_IXMLNode Node); // 解析 XML Child

	// 通知 note orig , 此校勘有 mod 版
	// 就會把 orig note 中 class 的 note_mod 移除
	void __fastcall ThisNoteHasMod(String sIdNormal);
	// 原本的 orig 校勘還沒加入, 此時才要加入
	String __fastcall AddOrigNote(String HTMLText);

public: // User declarations

	// 缺字
	map<String, String> CB2des;     // 缺字 CB 碼查組字式
    map<String, String> CB2nor;     // 缺字 CB 碼查通用字
    map<String, String> CB2uni;     // 缺字 CB 碼查 unicode
    map<String, String> CB2nor_uni; // 缺字 CB 碼查通用 unicode

    map<String, String> SD2roma;    // 悉曇字 SD 碼查羅馬轉寫字 (純文字)
	map<String, String> SD2uni;     // 悉曇字 SD 碼查羅馬轉寫字 (Unicode)
    map<String, String> SD2char;    // 悉曇字 SD 碼查對映 TTF 字型的字
    map<String, String> SD2big5;    // 悉曇字 SD 碼查 Big5
	//map<String, String> RJ2roma;    // 蘭札字 RJ 碼查羅馬轉寫字 (純文字)
	//map<String, String> RJ2uni;     // 蘭札字 RJ 碼查羅馬轉寫字 (Unicode)
	//map<String, String> RJ2char;    // 蘭札字 RJ 碼查對映 TTF 字型的字
	//map<String, String> RJ2big5;    // 蘭札字 RJ 碼查 Big5

    map<String, String> mOrigNote;  // 暫存的 orig note

    bool ShowHighlight; 	// 是否塗色

	void __fastcall SaveToHTML(String sFile);	// 儲存至 HTML

	inline String __fastcall GetAttr(_di_IXMLNode Node, String sAttr); // 取得屬性
	String __fastcall NoteId2Num(String sId); // 把校勘ID 變成校勘數字 0001001 -> 1
	void __fastcall ReadGaiji(_di_IXMLNode xmlNodeGaijis); // 讀取缺字

	// 傳入參數為 XML 檔, 呈現的設定
	__fastcall CCBXML(String sFile, CSetting * csSetting, String sJSFile, bool bShowHighlight = false, TmyMonster * seSearchEngine = 0);
	__fastcall ~CCBXML();
};
// ---------------------------------------------------------------------------
#endif
