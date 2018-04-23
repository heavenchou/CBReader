// ---------------------------------------------------------------------------

#ifndef SettingH
#define SettingH

#include "cbxmloption.h"
#include <fmx.h>
#include <System.IniFiles.hpp>
// ---------------------------------------------------------------------------
enum ctCollationType {ctOrigCollation, ctCBETACollation};
class CSetting
{
private: // User declarations

public: // User declarations

	String SettingFile;     // 設定檔的位置

	//CCBXMLOption * CBXMLOption; // CBETA 經文的格式

	// 預設 Windows 會在主程式的子目錄 Bookcase
	// 預設 Mac 會在 /Library/CBETA/Bookcase
	// 若找不到, 會詢問使用者, 然後存在 BookcaseFullDir
    // BookcaseFullDir 是第一搜尋目標
	String BookcasePath; 	// 書櫃的目錄
	String BookcaseFullPath; 		// 書櫃的完整目錄

    // 經文格式

	bool ShowLineFormat;	// 是否依大正藏切行
	bool ShowLineHead;		// 是否行首加上行首資訊
    int  CorrSelect;		// 修訂選擇 0:修訂用字, 1:二者皆要 [底本>修訂], 2:底本用字
    int ShowCorrWarning;    // 是否要秀出修訂選擇的注意事項, 預設是 0
    bool ShowCollation;		// 顯示校勘資料
	int  CopyMode;			// 1. 有校勘, 經名在前. 2.有校勘, 經名在後. 3. 無校勘, 經名在前. 4.無校勘, 經名在後.
    bool CBCopyWithJuanNum; // 引用複製是否呈現卷數

	bool VerticalMode;		// 垂直顯示
    bool ShowPunc;          // 呈現標點與段落
    bool NoShowLgPunc;      // 不呈現偈頌的標點
    int LgType;             // 這是2016新的暫時功能, 設定偈頌呈現的方式, 0 為舊的方式用空格, 1 為非標準偈頌用 <p> 呈現編排

	// 校勘格式

	ctCollationType CollationType;      // 校勘格式 0:原書, 1:CBETA

	// 經文呈現的顏色, 背景

    TColor BgColor;     			// 畫面的背景色
    String BgImage;				// 背景圖
    int    LineHeight;              // 行距
    TColor JuanNumFontColor;		// 經號的顏色
    int    JuanNumFontSize;			// 經號的文字大小
    bool   JuanNumBold;			    // 經號的文字是否粗體
    TColor JuanNameFontColor;		// 卷名的顏色
    int    JuanNameFontSize;		// 卷名的文字大小
    bool   JuanNameBold;			// 卷名的文字是否粗體
    TColor XuFontColor;				// 序的顏色
    int    XuFontSize;				// 序的文字大小
    bool   XuBold;			        // 序的文字是否粗體
    TColor BodyFontColor;			// 正文的顏色
    int    BodyFontSize;			// 正文的文字大小
    bool   BodyBold;			    // 正文的文字是否粗體
    TColor WFontColor;				// 附文的顏色
    int    WFontSize;				// 附文的文字大小
    bool   WBold;			        // 附文的文字是否粗體
    TColor BylineFontColor;			// 作譯者的顏色
    int    BylineFontSize;			// 作譯者的文字大小
    bool   BylineBold;			    // 作譯者的文字是否粗體
    TColor HeadNameFontColor;		// 標題的顏色
    int    HeadNameFontSize;		// 標題的文字大小
    bool   HeadNameBold;			// 標題的文字是否粗體
    TColor LineHeadFontColor;		// 行首的顏色
    int    LineHeadFontSize;		// 行首的文字大小
    bool   LineHeadBold;			// 行首的文字是否粗體
    TColor LgFontColor;				// 偈頌的顏色
    int    LgFontSize;				// 偈頌的文字大小
    bool   LgBold;			        // 偈頌的文字是否粗體

    TColor DharaniFontColor;		// 咒的顏色
    int    DharaniFontSize;			// 咒的文字大小
    bool   UseDharaniFontColor;		// 是否使用咒的顏色
    bool   UseDharaniFontSize;		// 是否使用咒的文字大小
    bool   DharaniBold;			    // 咒的文字是否粗體

    TColor CorrFontColor;		// 勘誤的顏色
    int    CorrFontSize;		// 勘誤的文字大小
    bool   UseCorrFontColor;	// 是否使用勘誤的顏色
    bool   UseCorrFontSize;		// 是否使用勘誤的文字大小
    bool   CorrBold;			// 勘誤的文字是否粗體

    TColor GaijiFontColor;		// 缺字的顏色
    int    GaijiFontSize;		// 缺字的文字大小
    bool   UseGaijiFontColor;	// 是否使用缺字的顏色
    bool   UseGaijiFontSize;	// 是否使用缺字的文字大小
    bool   GaijiBold;			// 缺字的文字是否粗體

    TColor NoteFontColor;		// 雙行小註的顏色
    int    NoteFontSize;		// 雙行小註的文字大小
    bool   UseNoteFontColor;	// 是否使用雙行小註的顏色
    bool   UseNoteFontSize;		// 是否使用雙行小註的文字大小
    bool   NoteBold;			// 雙行小註的文字是否粗體

	TColor FootFontColor;		// 校勘的顏色
	int    FootFontSize;		// 校勘的文字大小
	bool   UseFootFontColor;	// 是否使用校勘的顏色
	bool   UseFootFontSize;		// 是否使用校勘的文字大小
    bool   FootBold;			// 校勘的文字是否粗體

	bool   UseCSSFile;     		// 使用 CSS 檔案
	String CSSFileName;

	// 缺字處理

	bool GaijiUseUniExt;    // 是否使用 Unicode Ext
	bool GaijiUseNormal;    // 是否使用通用字

	bool GaijiUniExtFirst;  // 優先使用 Unicode Ext
	bool GaijiNormalFirst;  // 優先使用 通用字

	bool GaijiDesFirst;     // 優先使用組字式
	bool GaijiImageFirst;   // 優先使用缺字圖檔

	//int GaijiID[4];			// 4種處理缺字的方法, 分別是"通用字","組字式","Unicode", "圖型"
	//bool GaijiUse[4];		// 4種缺字是否使用

	int ShowSiddamWay;		// 悉曇字處理法 0:悉曇字型(siddam.ttf) 1:羅馬轉寫(unicode) 2:羅馬轉寫(純文字) 3:悉曇圖檔 4:自訂符號 5:CB碼
	int ShowPaliWay;		// 梵巴字處理法 0:Unicode 1:純文字 2:Ent 碼

	String UserSiddamSign;		// 使用者自訂悉曇字符號
    bool ShowUnicode30;     // 呈現 Unicode 3.1

	// 圖檔大小

	int GaijiWidth;			// 缺字圖檔的寬, 0 為不設限
	int GaijiHeight;		// 缺字圖檔的高
	int SDGifWidth;			// 缺字圖檔的寬, 0 為不設限
	int SDGifHeight;		// 缺字圖檔的高
	int FigureRate;			// 圖檔的比例

    // 系統資訊

    String CBETACDPath;		// CD 經文的位置
    String XMLSutraPath;	// 經文的位置
	String BookmarkFile;	// Bookmark 的位置
	String SearchPath;		// 全文檢索的位置
	String GaijiPath;		// 缺字圖檔的位置
	String SDGifPath;		// 悉曇字圖檔的位置
	String RJGifPath;		// 蘭札字圖檔的位置
	String FigurePath;		// 圖檔的位置
    String UserDataPath;    // 個人資料目錄
    bool SaveLastTocFileName;	// 是否儲存最後開啟的書目
    String LastTocFileName;	// 最後開啟的書目

	// 全文檢索

	int NearNum;				// 全文檢索 Near 的字距
	int BeforeNum;				// 全文檢索 Before 的字距
	int MaxSearchNum;			// 輸出最多的筆數, 0 表示不設限
    bool OverSearchWarn; 		// 超出全文檢索檔案數的限制時, 是否提出警告?
    bool RMPunctuationWarn; 	// 移除使用者輸入檢索字串中的標點時，是否提出警告訊息？

    // 外部連結的資料

    int CalloutCount;                   // 外部連結程式的數目
	//TTntStringList * slCalloutTitle;       // 外部連結的標題
	//TTntStringList * slCalloutProgram;     // 外部連結的程式

	// 還沒處理 (或不處理?)

    String XMLJuanPosPath;		// 每一卷經文移位的資料檔
	String JuanLinePath;		// 每一卷經文第一行行首的資訊

	void __fastcall LoadFromFile();
	void __fastcall SaveToFile();
	void __fastcall LoadFromFile(String sFile);
	void __fastcall SaveToFile(String sFile);

	__fastcall CSetting(String sFile);
	__fastcall ~CSetting();
};
// ---------------------------------------------------------------------------
#endif
