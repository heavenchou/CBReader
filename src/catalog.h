// ---------------------------------------------------------------------------
#ifndef CatalogH
#define CatalogH

#include <fmx.h>
#include <vector>
#include <System.RegularExpressions.hpp>
#include <System.IOUtils.hpp>
#include <FMX.DialogService.hpp>

// ---------------------------------------------------------------------------
using namespace std;

class CCatalog
{
private: // User declarations

public: // User declarations

	vector <TStringList *> Field;

	TStringList * ID;         // 藏經代碼
	TStringList * Bulei;      // 部類
	TStringList * Part;       // 部別
	TStringList * Vol;        // 冊數
	TStringList * SutraNum;   // 經號
	TStringList * JuanNum;    // 卷數
	TStringList * SutraName;  // 經名
	TStringList * Byline;     // 作譯者

	int __fastcall FindIndexBySutraNum(String sBook, String sVol, String sSutraNum);	// 取得 Catalog 的編號
	int __fastcall FindIndexBySutraNum(String sBook, int iVol, String sSutraNum);	// 取得 Catalog 的編號
	//int __fastcall FindIndexBySutraNum(String sBook, String sSutraNum);	// 取得 Catalog 的編號
	void __fastcall LoadCatalog(String sFile);      	// 傳入檔案, 初值化
	__fastcall CCatalog();
	__fastcall ~CCatalog();
};
// ---------------------------------------------------------------------------
#endif
