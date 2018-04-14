//---------------------------------------------------------------------------

#ifndef SampleH
#define SampleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
/* 工作原理
	主程式傳入二個 TStringList 的指標, 一個存說明, 一個存結果
	呼叫 RunAllTest();
	就逐一測試程式,
	並逐一把 "說明文字" 和 布林結果 存入二個 List 中.
	成功傳回 1 , 失敗傳回 0
*/

class CSample
{
private:	// User declarations
public:		// User declarations

	// 傳入要儲存結果的字串列表
	TStringList * Titles;
	TStringList * Results;

	void __fastcall RunAllTest();							// 執行全部測試
	void __fastcall LogTest(String sTitle, bool bResult);  // 儲存測試結果

	// 要測試的項目
	bool __fastcall Test1();
	bool __fastcall Test2();

	__fastcall CSample(TStringList * slTitle, TStringList * slResult);
};

//---------------------------------------------------------------------------
#endif
