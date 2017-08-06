//---------------------------------------------------------------------------

#ifndef cbxmltestH
#define cbxmltestH
//---------------------------------------------------------------------------

#include "../src/cbxml.h"
class CCBXMLTest
{
private: // User declarations

	// 傳入要儲存結果的字串列表
	TStringList * Titles;
	TStringList * Results;

	CCBXML * CBXML;

public: // User declarations

	void __fastcall RunAllTest();							// 執行全部測試
	void __fastcall LogTest(String sTitle, bool bResult);  // 儲存測試結果

	// 要測試的項目

	bool __fastcall CBXMLSaveToHTML();

	__fastcall CCBXMLTest(TStringList * slTitle, TStringList * slResult);
};
//---------------------------------------------------------------------------
#endif
