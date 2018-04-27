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
#include "rendattr.h"
#include "nextline.h"
#include "main.h"
#include "System.RegularExpressions.hpp"
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

	// --------------------------

	String DivType[30];		// 最多 20 層
	int DivCount;   		// Div 的層次

	bool InByline;  		// 用來判斷是否是作譯者
	int  FuWenCount;		// 用來判斷是否是附文, 因為有巢狀, 所以用 int
	bool InSutraNum;	   	// 用來判斷本行是否是經號
	bool InPinHead;			// 用來判斷本行是否是品名
	bool InFuWenHead;		// 用來判斷本行是否是附文標題
	bool InOtherHead;		// 用來判斷本行是否是其它標題
	bool InHead;			// 用來判斷本行是否是標題
	int  NoNormal;			// 用來判斷是否可用通用字 , 大於 0 就不用通用字, 這二種就不用通用字 <text rend="no_nor"> 及 <term rend="no_nor">

	// 偈頌相關
	bool IsFindLg;			// 一遇到 <lg> 就 true, 第一個 <l> 就會處理並設為 false;
	int  LgCount;              // 判斷是不是在 <lg> 之中, 主要是用來處理偈頌中的標點要不要呈現.
	bool LgNormal;		    // lg 的 type 是不是 normal? 有 normal 及 abnormal 二種
	bool LgInline;          // lg 的 place 是不是 inline?
	String LgMarginLeft;	// lg 整段要空的格
	// L
	int LTagNum;		    // <l> 出現的數字, 用來判斷要在普及版寫幾個空格
	String LMarginLeft;	    // L的空格

	bool InTTNormal;		// 在 <tt rend="normal"> 中, 這時每一個 <t> 都要換行 , T54n2133A : <lb n="1194c17"/><p><tt rend="normal"><t lang="san-sd">
	int  PreFormatCount;	// 判斷是否是要依據原始經文格式切行, 要累加的, 因為可能有巢狀的 pre
	String MarginLeft;		// 移位
	//String NormalWords; 	// 通用詞處理法, 若是 orig , 就是呈現 <orig> 中的字, 若是 "reg" 就是呈現 <reg> 中的字, 這是在 choice 標記中判斷

	int ListCount;			// 計算 list 的數目, 有一些地方需要用到
	int ItemNum[100];		// 用來判斷 item 出現的次數, 每一層 list 都有不同的內容

	int CellNum;            // 計算第幾個 Cell, 用來判斷要在普及版寫幾個空格
	int OtherColspan;       // 因本 cell 佔 n 格以上, 所以和後面的 cell 要空 (n-1)*3 的空格, 此即記錄 n-1 的數字

	// 要判斷品的範圍, 若出現品的 mulu, 則記錄 level, 等到 level 數字再次大於或等於時, 此品才結束
	//<mulu level="3" label="3 轉輪聖王品" type="品"/>
	int MuluLevel;          // 目錄的層次
	String MuluLabel;		// 目錄的名稱
    bool InMulu;			// 在 <cb:mulu>...</cb:mulu> 的範圍內, 文字則不呈現,
	bool InMuluPin;			// 在 <cb:mulu>...</cb:mulu> 的範圍內, 而且是 "品" , 則文字不呈現, 但要記錄至 MuluLabel

	int NoteAddNum;			// 自訂校註 <note type="add" 的流水號, 由 1 開始
	map<String, int> mpNoteAddNum;	// 由 id 找出 流水號, 沒有就設定一個
	map<String, int> mpNoteStarNum; // 記錄每一個 id 有多少個星號了

	// --------------------------

	CNextLine * NextLine;		// 用來處理隔行 <tt> 的物件

	// --------------------------


	// 處理標記
	String __fastcall tag_anchor(_di_IXMLNode Node);
	String __fastcall tag_app(_di_IXMLNode Node);
	String __fastcall tag_byline(_di_IXMLNode Node);
	String __fastcall tag_cell(_di_IXMLNode Node);
	String __fastcall tag_div(_di_IXMLNode Node);
	String __fastcall tag_docNumber(_di_IXMLNode Node);
	String __fastcall tag_entry(_di_IXMLNode Node);
	String __fastcall tag_figdesc(_di_IXMLNode Node);
	String __fastcall tag_foreign(_di_IXMLNode Node);
	String __fastcall tag_form(_di_IXMLNode Node);
	String __fastcall tag_formula(_di_IXMLNode Node);
	String __fastcall tag_g(_di_IXMLNode Node);
	String __fastcall tag_graphic(_di_IXMLNode Node);
	String __fastcall tag_head(_di_IXMLNode Node);
	String __fastcall tag_item(_di_IXMLNode Node);
	String __fastcall tag_juan(_di_IXMLNode Node);
	String __fastcall tag_l(_di_IXMLNode Node);
	String __fastcall tag_lb(_di_IXMLNode Node);
	String __fastcall tag_lem(_di_IXMLNode Node);
	String __fastcall tag_lg(_di_IXMLNode Node);
	String __fastcall tag_list(_di_IXMLNode Node);
	String __fastcall tag_mulu(_di_IXMLNode Node);
	String __fastcall tag_note(_di_IXMLNode Node);
	String __fastcall tag_p(_di_IXMLNode Node);
	String __fastcall tag_pb(_di_IXMLNode Node);
	String __fastcall tag_rdg(_di_IXMLNode Node);
	String __fastcall tag_ref(_di_IXMLNode Node);
	String __fastcall tag_row(_di_IXMLNode Node);
	String __fastcall tag_sg(_di_IXMLNode Node);
	String __fastcall tag_space(_di_IXMLNode Node);
	String __fastcall tag_t(_di_IXMLNode Node);
	String __fastcall tag_table(_di_IXMLNode Node);
	String __fastcall tag_term(_di_IXMLNode Node);
	String __fastcall tag_trailer(_di_IXMLNode Node);
	String __fastcall tag_tt(_di_IXMLNode Node);
	String __fastcall tag_unclear(_di_IXMLNode Node);
	String __fastcall tag_default(_di_IXMLNode Node);

	// 處理 XML
	String __fastcall ParseXML(); // 解析 XML , 儲存到 TreeRoot 中
	String __fastcall ParseNode(_di_IXMLNode Node); // 解析 XML Node
	String __fastcall parseChild(_di_IXMLNode Node); // 解析 XML Child

	String __fastcall MakeHTMLHead(); 	// 先產生 html 的 head

	// 通知 note orig , 此校勘有 mod 版
	// 就會把 orig note 中 class 的 note_mod 移除
	void __fastcall ThisNoteHasMod(String sIdNormal);
	// 原本的 orig 校勘還沒加入, 此時才要加入
	String __fastcall AddOrigNote(String HTMLText);

	// 傳入 note 標記的 id , 傳回流水號, 若沒有就自動 + 1 並傳回
	int __fastcall Get_Add_IdNum(String sId);

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

	// 處理 XML 過程需要的變數

	String BookId;		// 內容是 'T'(大正藏), 'X'(卍續藏)
	String VolId;      	// 內容是 01
	String SutraId;		// 內容是 "0001" or "0143a"
	String SutraId_;	// 內容是 "0001_" or "0143a"
	String SutraName;	// 經名
	int    JuanNum;		// 第幾卷
    int    TotalJuan;   // 共幾卷, 用來判斷是不是只有一卷

	String GotoPageLine;	// 本網頁要跳到的地點 (因為不一定是卷首)

	String BookVolnSutra;	// 內容是 T01n0001_
	String PageLine;		// 內容是 0001a01
	String LineHead;		// 內容是 T01n0001_p0001a01║

	bool ShowHighlight; 	// 是否塗色

	String BookVerName;     // 例如大正藏是 【大】,這是要由其他資料找出來的

	String SerialPath;      // 主要目錄, 要找圖檔位置用的

	void __fastcall GetInitialFromFileName();   // 由經名取得一切相關資訊

	void __fastcall SaveToHTML(String sFile);	// 儲存至 HTML

	inline String __fastcall GetAttr(_di_IXMLNode Node, String sAttr); // 取得屬性
	String __fastcall NoteId2Num(String sId); // 把校勘ID 變成校勘數字 0001001 -> 1
	void __fastcall ReadGaiji(_di_IXMLNode xmlNodeGaijis); // 讀取缺字

	// 取得下一個 note , 但因為有一些是 <lb type=old> , <pb type=old> <lb id=Rxx> 要忽略
	_di_IXMLNode __fastcall GetNextSiblNode(_di_IXMLNode Node);

	// 把 <tr/>..<tr><td> 中間的資料移到 <td> 裡面
	String __fastcall mv_data_between_tr(String sHtml);
	// 取代正規式 : 把 <tr/>..<tr><td> 中間的資料移到 <td> 裡面
	String __fastcall TableTrReplace (const TMatch &Match);

	// 傳入參數為 XML 檔, 呈現的設定
	__fastcall CCBXML(String sFile, String sLink, CSetting * csSetting, String sJSFile, bool bShowHighlight = false, TmyMonster * seSearchEngine = 0);
	__fastcall ~CCBXML();
};
// ---------------------------------------------------------------------------
#endif
