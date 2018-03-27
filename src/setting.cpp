// ---------------------------------------------------------------------------

#pragma hdrstop

#include "setting.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CSetting::CSetting(String sFile) // 建構函式
{
	SettingFile = sFile;            // 設定檔的位置

	// 設定預設值

	BookcaseDir = "Bookcase"; 		// 書櫃的目錄

    // 經文格式

	ShowLineFormat = false;         // 是否依大正藏切行
	ShowLineHead = true;			// 是否行首加上行首資訊
	//CorrSelect = 0;				// 勘誤選擇 0:修訂用字, 1:二者皆要 [底本>修訂], 2:底本用字
	//ShowCorrWarning = 0;          // 是否要秀出修訂選擇的注意事項, 預設是 0
	//ShowJKData = true;			// 顯示校勘資料
	VerticalMode = false;			// 垂直顯示
	ShowPunc = true;                // 呈現標點
	NoShowLgPunc = false;           // 不呈現偈頌的標點
	//LgType = 1;                   // 這是2016新的暫時功能, 設定偈頌呈現的方式, 0 為舊的方式用空格, 1 為非標準偈頌用 <p> 呈現編排

	CollationType = ctCBETACollation;      // 校勘格式 0:無, 1:原書, 2:CBETA

    // 文字的顏色與大小

	BgColor = TColor(14479837);				// 預設值是白色
    BgImage = "";
    LineHeight=20;                  // 行距
	JuanNumFontColor = TColor(32768);
    JuanNumFontSize = 16;
    JuanNumBold = false;
	JuanNameFontColor = TColor(16711680);
    JuanNameFontSize = 18;
    JuanNameBold = true;
	XuFontColor = TColor(10485760);
    XuFontSize = 16;
    XuBold = false;
	BodyFontColor = TColor(0);
    BodyFontSize = 16;
    BodyBold = false;
	WFontColor = TColor(10485760);
    WFontSize = 16;
    WBold = false;
	BylineFontColor = TColor(8421440);
    BylineFontSize = 16;
    BylineBold = false;
	HeadNameFontColor = TColor(10485760);
    HeadNameFontSize = 18;
    HeadNameBold = true;
	LineHeadFontColor = TColor(10485760);
    LineHeadFontSize = 14;
    LineHeadBold = false;
	LgFontColor = TColor(4227072);
    LgFontSize = 16;
    LgBold = false;
	DharaniFontColor = TColor(4227072);
    DharaniFontSize = 16;
    DharaniBold = false;
	CorrFontColor = TColor(255);
    CorrFontSize = 16;
    CorrBold = false;
	GaijiFontColor = TColor(16711680);
    GaijiFontSize = 16;
    GaijiBold = false;
	NoteFontColor = TColor(20639); // 6684774;
    NoteFontSize = 14;
    NoteBold = false;
	FootFontColor = TColor(8421631);
	FootFontSize = 16;
    FootBold = false;

    UseDharaniFontColor = false;	// 是否使用咒的顏色
    UseDharaniFontSize = false;	// 是否使用咒的文字大小
    UseCorrFontColor = true;	// 是否使用勘誤的顏色
    UseCorrFontSize = false;	// 是否使用勘誤的文字大小
    UseGaijiFontColor = false;	// 是否使用缺字的顏色
    UseGaijiFontSize = false;	// 是否使用缺字的文字大小
    UseNoteFontColor = true;	// 是否使用雙行小註的顏色
	UseNoteFontSize = true;		// 是否使用雙行小註的文字大小
	UseFootFontColor = false;	// 是否使用校勘的顏色
	UseFootFontSize = false;		// 是否使用校勘的文字大小

    UseCSSFile = false;     		// 使用 CSS 檔案
    CSSFileName = "cbreader.css";	// CSS 檔名

    // 缺字處理

	GaijiUseUniExt = true;    // 是否使用 Unicode Ext
	GaijiUseNormal = true;    // 是否使用通用字

	GaijiUniExtFirst = true;  // 優先使用 Unicode Ext
	GaijiNormalFirst = false;  // 優先使用 通用字

	GaijiDesFirst = true;     // 優先使用組字式
	GaijiImageFirst = false;   // 優先使用缺字圖檔

	ShowSiddamWay = 6;		// 悉曇字處理法 0:悉曇字型(siddam.ttf) 1:羅馬轉寫(unicode) 2:羅馬轉寫(純文字) 3:悉曇圖檔 4:自訂符號 5:CB碼 6:悉曇羅馬對照
	UserSiddamSign = "◇";	// 使用者自訂悉曇字符號
    ShowPaliWay = 0;		// 梵巴字處理法 0:Unicode 1:純文字 2:Ent 碼
    ShowUnicode30 = true;  // 呈現 Unicode 3.0 , 在 P5 XML , 這個可以預設為

	// 圖檔大小

	GaijiWidth  = 0;		// 缺字圖檔的寬, 0 為不設限
	GaijiHeight = 0;		// 缺字圖檔的高, 0 為不設限
	SDGifWidth  = 0;		// 缺字圖檔的寬, 0 為不設限
	SDGifHeight = 0;		// 缺字圖檔的高, 0 為不設限
	FigureRate  = 100;		// 圖檔的比例

    // 系統資訊

	CBETACDPath = "D:\\";		// CBETA 光碟的位置
    XMLSutraPath = "XML\\";	    // d:\\cbeta.src\\release\\xml\\";	// 經文的位置
	//BookmarkFile = fmMain->BookmarkFile;
	//SearchPath = fmMain->SearchPath;
	GaijiPath  = "gaiji-CB\\";
	SDGifPath  = "sd-gif\\";		// 悉曇字圖檔的位置
	RJGifPath  = "rj-gif\\";		// 蘭札字圖檔的位置
	FigurePath = "Figures\\";
	//UserDataPath = fmMain->UserDataPath;   // 個人資料目錄
    SaveLastTocFileName = true;		// 是否儲存最後開啟的書目
    LastTocFileName = "00All.toc";

	// 全文檢索

	NearNum = 30;				// 全文檢索 Near 的字距
	BeforeNum = 30;				// 全文檢索 Before 的字距
	MaxSearchNum = 500;			// 輸出最多的筆數, 0 表示不設限
    OverSearchWarn = true; 		// 超出全文檢索檔案數的限制時, 是否提出警告?
    RMPunctuationWarn = true;	// 移除使用者輸入檢索字串中的標點時，是否提出警告訊息？

	// 引用複製模式

	CopyMode = 1;			    // 1. 有校勘, 經名在前. 2.有校勘, 經名在後. 3. 無校勘, 經名在前. 4.無校勘, 經名在後.
    CBCopyWithJuanNum = true;   // 引用複製是否呈現卷數

    // 外部連結的資料

    CalloutCount = 0;               // 外部連結程式的數目
	//slCalloutTitle = new TTntStringList();      // 外部連結的標題
	//slCalloutProgram = new TTntStringList();    // 外部連結的程式

    // 不改的資訊 (或是很難改, 要會 ini)
    XMLJuanPosPath = "JuanPos\\";	// 每一卷經文移位的資料檔
	JuanLinePath = "JuanLine\\";	// 每一卷經文第一行行首的資訊

    LoadFromFile(SettingFile);  // 載入設定檔
}

// ---------------------------------------------------------------------------
__fastcall CSetting::~CSetting(void) // 解構函式
{
}
// ---------------------------------------------------------------------------
// 載入設定檔
void __fastcall CSetting::LoadFromFile()
{
	LoadFromFile(SettingFile);
}
// ---------------------------------------------------------------------------
// 載入設定檔
void __fastcall CSetting::LoadFromFile(String sFile)
{
	TIniFile *IniFile = new TIniFile(sFile);

	String Section;

	// Ini file 結構是
    // [section]
    // Ident = Value

    Section = "ShowFormat";

	ShowLineFormat = IniFile->ReadBool(Section, "ShowLineFormat", ShowLineFormat);
	ShowLineHead = IniFile->ReadBool(Section, "ShowLineHead", ShowLineHead);
	ShowPunc = IniFile->ReadBool(Section, "ShowPunc", ShowPunc);
	NoShowLgPunc = IniFile->ReadBool(Section, "NoShowLgPunc", NoShowLgPunc);
	VerticalMode = IniFile->ReadBool(Section, "VerticalMode", VerticalMode);

	CollationType = (ctCollationType)IniFile->ReadInteger(Section, "CollationType", CollationType);

	// 缺字處理

	Section = "Gaiji";

	GaijiUseUniExt = IniFile->ReadBool(Section, "GaijiUseUniExt" ,GaijiUseUniExt);    // 是否使用 Unicode Ext
	GaijiUseNormal = IniFile->ReadBool(Section, "GaijiUseNormal" ,GaijiUseNormal);      // 是否使用通用字

	GaijiUniExtFirst = IniFile->ReadBool(Section, "GaijiUniExtFirst" ,GaijiUniExtFirst);  // 優先使用 Unicode Ext
	GaijiNormalFirst = !GaijiUniExtFirst;  // 優先使用 通用字

	GaijiDesFirst = IniFile->ReadBool(Section, "GaijiDesFirst" ,GaijiDesFirst);     // 優先使用組字式
	GaijiImageFirst = !GaijiDesFirst;   // 優先使用缺字圖檔

	// 系統資訊

	Section = "SystemInfo";

	BookcaseDir = IniFile->ReadString(Section, "BookcaseDir", BookcaseDir);

	delete IniFile;
}
// ---------------------------------------------------------------------------
// 儲存設定
void __fastcall CSetting::SaveToFile()
{
	SaveToFile(SettingFile);
}
// ---------------------------------------------------------------------------
// 儲存設定
void __fastcall CSetting::SaveToFile(String sFile)
{
	TIniFile *IniFile = new TIniFile(sFile);

    AnsiString Section;

    // Ini file 結構是
    // [section]
    // Ident = Value

	Section = "Version";
    try
    {
		IniFile->WriteString(Section, "Version", "0.1");
    }
    catch(...)
    {
		delete IniFile;     // 先關掉再說

		return;
	}

    Section = "ShowFormat";

	IniFile->WriteBool(Section, "ShowLineFormat", ShowLineFormat);
	IniFile->WriteBool(Section, "ShowLineHead", ShowLineHead);
	IniFile->WriteBool(Section, "ShowPunc", ShowPunc);
	IniFile->WriteBool(Section, "NoShowLgPunc", NoShowLgPunc);
	IniFile->WriteBool(Section, "VerticalMode", VerticalMode);
	IniFile->WriteInteger(Section, "CollationType", CollationType);

	// 缺字處理

	Section = "Gaiji";

	IniFile->WriteBool(Section, "GaijiUseUniExt", GaijiUseUniExt);
	IniFile->WriteBool(Section, "GaijiUseNormal", GaijiUseNormal);
	IniFile->WriteBool(Section, "GaijiUniExtFirst", GaijiUniExtFirst);
	IniFile->WriteBool(Section, "GaijiDesFirst", GaijiDesFirst);

	// 系統資訊

	Section = "SystemInfo";

	IniFile->WriteString(Section, "BookcaseDir", BookcaseDir);

	delete IniFile;
}
// ---------------------------------------------------------------------------

