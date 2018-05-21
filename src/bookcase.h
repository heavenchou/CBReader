// ---------------------------------------------------------------------------

#ifndef BookcaseH
#define BookcaseH

#include <fmx.h>
#include <System.IOUtils.hpp>
#include <FMX.DialogService.hpp>
#include "series.h"

// ---------------------------------------------------------------------------

class CBookcase
{
private: // User declarations



public: // User declarations

	String BookcaseDir; // 書櫃的目錄
	TList * Books;      // 所有套書的指標

	CSeries * CBETA;    // 專指向 CBETA 的指標

	void __fastcall LoadBookcase(String sDir);  // 載入所有的書櫃
	int __fastcall Count();  // Books 的數量

	__fastcall CBookcase();
	__fastcall ~CBookcase();
};
// ---------------------------------------------------------------------------
#endif
