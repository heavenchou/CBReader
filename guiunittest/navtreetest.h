//---------------------------------------------------------------------------

#ifndef NavTreeTestH
#define NavTreeTestH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "../src/navtree.h"
//---------------------------------------------------------------------------
/* 工作原理
	主程式傳入二個 TStringList 的指標, 一個存說明, 一個存結果
	呼叫 RunAllTest();
	就逐一測試程式,
	並逐一把 "說明文字" 和 布林結果 存入二個 List 中.
	成功傳回 1 , 失敗傳回 0
*/

class CNavTreeTest
{
private:	// User declarations
public:		// User declarations

	// 傳入要儲存結果的字串列表
	TStringList * Titles;
	TStringList * Results;

	String MyFullPath;

	CNavTree * NavTree;

	void __fastcall RunAllTest();							// 執行全部測試
	void __fastcall LogTest(String sTitle, bool bResult);  // 儲存測試結果

	// 要測試的項目

	bool __fastcall NavTreeReadTitle();
	bool __fastcall NavTreeReadLink();
	bool __fastcall NavTreeReadNavLink();
	bool __fastcall NavTreeReadCBLink();

	__fastcall CNavTreeTest(TStringList * slTitle, TStringList * slResult);
};

//---------------------------------------------------------------------------
#endif
