//---------------------------------------------------------------------------
#include <fmx.h>
#ifndef BookcaseTestH
#define BookcaseTestH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "../src/bookcase.h"
#include "../src/series.h"
//---------------------------------------------------------------------------
/* 工作原理
	主程式傳入二個 TStringList 的指標, 一個存說明, 一個存結果
	呼叫 RunAllTest();
	就逐一測試程式,
	並逐一把 "說明文字" 和 布林結果 存入二個 List 中.
	成功傳回 1 , 失敗傳回 0
*/

class CBookcaseTest
{
private:	// User declarations
public:		// User declarations

	// 傳入要儲存結果的字串列表
	TStringList * Titles;
	TStringList * Results;

	String MyFullPath;

	CBookcase * Bookcase;
	CSeries * Series;
	
	void __fastcall RunAllTest();							// 執行全部測試
	void __fastcall LogTest(String sTitle, bool bResult);  // 儲存測試結果

	// 要測試的項目

	bool __fastcall BookcaseFindDir();
	bool __fastcall BookcaseCount();
	bool __fastcall ReadSeriesTitle();
	bool __fastcall ReadSeriesNav();
	bool __fastcall FindXMLBySutraJuan(String sBookID, String sSutra, String sJuan, String sFile);
	bool __fastcall FindXMLByVolPageFieldLine(String sBookID, String sVol, String sPage, String sField, String sLine, String sFile);
	bool __fastcall ChkNormalVol(String sBookID, String sVol, String sVolResult);
	bool __fastcall ChkNormalSutra(String sSutra, String sResult);
	bool __fastcall ChkNormalPage(String sPage, String sResult);
	bool __fastcall ChkNormalLine(String sLine, String sResult);
	__fastcall CBookcaseTest(TStringList * slTitle, TStringList * slResult);
};

//---------------------------------------------------------------------------
#endif
