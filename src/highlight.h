//---------------------------------------------------------------------------

#ifndef HighlightH
#define HighlightH

#include <vector>
#include <map>
#include "../../Monster/src/Monster.h"
#include "../../Monster/src/PostfixStack.h"
#include "../../Monster/src/Int2List.h"

//---------------------------------------------------------------------------
// 塗顏色的物件
class CHighlight
{
private: // User declarations

public: // User declarations

	struct Smy_HighlightPos {
		int Begin;	// highlight 的起點
		int End;	// highlight 的終點
		int Num;	// 第幾個詞
		int Index;	// 該詞的第幾次出現
		int Next;	// 下一個詞的編號, 通常是 Index + 1 , 若是最後一筆, 則是 0
	} HighlightPos;

	String * HTMLSource;                     // 來源字串
	//vector< wchar_t > vText;            // 放置結果的

	TmyPostfixStack * PostfixStack;		// 運算用的

	// 搜尋字串中 "詞" 的數量, 例如 菩薩 * 羅漢 = 2 個詞
	int FoundPosCount;                  // FoundPos 的數量

	// 儲存找到的位置 (這是忽略標點的位置)
	// 每一個詞都要有一個記錄位置的空間, 這是一個二維陣列
	// 例 : 菩薩 + 羅漢
	// FoundPos[0] 是 "菩薩" 的位置, FoundPos[1] 是 "羅漢" 的位置.
	// FoundPos[0]->Int2s->Items[n] 表示是第n個菩薩的位置, 頭尾位置就是其 TPoint(x,y)
	TmyInt2List ** FoundPos;

	// 例如某字對應到真實的是組字 [金*本],
	// pair 內容就是 '['位置及字串長度 5
	map<int, pair<wchar_t *, int>> PosToReal;  // Pos 轉不忽略標點的真實位置


	// 這三個是要塗色用的重要資料, 記錄每一個字對應了哪些定位, 連結, 塗色
	// 定位, 後面接 vector , 三組一個單位, 分別是 第 x 組, 第 y 次出現, 下一次出現 (最後則回到 0)
	map <wchar_t *, vector<int>> mpWordAnchor;
	// 連結, 每個字只有一組,
	map <wchar_t *, pair<int,int>> mpWordLink;
	// 塗色, 每個字都可能好幾個顏色
	map <wchar_t *, vector<int>> mpWordClass;
	// 每一個字的長度, 例如組字式就會比較長
	map <wchar_t *, int> mpWordLength;

	/////////////////////////////////////

	// 每一個詞都有一個串列, 這是要記錄目前已畫線的串列是畫到什麼地方了.

	int * FoundPosIndex;
	TmyMonster * SearchEngine;   // 全文檢索引擎

	String __fastcall AddHighlight(String * sSource);  // 處理塗色

	void __fastcall GetAllFoundPos(void);		// 將所有的詞的位置先找到
	void __fastcall GetOneFoundPos(int iNum);	// 將某個詞的位置先找到
	void __fastcall FindTheRange(void);			// 將合乎要求的範圍找出來 (經過布林運算了), 傳回找到的組數
	void __fastcall KeepRangePos(void);			// 將每一個詞符合最終位置的留下來.
	void __fastcall GetEveryWordInfo(void);     // 把每一個字要定位、塗色與連結的資料都記錄下來, 包括重疊的字
	void __fastcall GetNextHighlightPos(bool bInitial = false);	// 取得下一個要變色的詞的相關位置, 任何傳入值都表示初值化
	int __fastcall Get1PattenLen(const wchar_t *);		// 找出此字串下一字的長度, 英文數字為 1 , [金*本] 為 7, &#x1234; 為 8
	String __fastcall MakeHighlight_old();  			// 實際塗色
	String __fastcall MakeHighlight();  			// 實際塗色

	// 找到某個要塗色的詞, 加上 name 的標記讓人連結, 指出是第 iNum 詞第 iTime 次出現
	// <a name="iNum_iTime"></a>
	void __fastcall AddSearchHeadName(vector<wchar_t> * vOutput, int iNum, int iTime);
	// 加上 <a href="#x_y" class="SearchWordx"> 標記
	void __fastcall AddSearchNameTag(vector<wchar_t> * vOutput, int iNum, int iTime);

	// 某個要塗色的詞, 加上定位 name 的標記讓人連結, 指出是第 iNum 詞第 iTime 次出現
	void __fastcall AddWordAnchor(vector<wchar_t> * vOutput, wchar_t * pPoint);

	// 加上連結及塗色的 class
	void __fastcall AddWordLink(vector<wchar_t> * vOutput, wchar_t * pPoint);

	__fastcall CHighlight(TmyMonster * seSearchEngine);
	__fastcall ~CHighlight();
};
// ---------------------------------------------------------------------------
#endif
