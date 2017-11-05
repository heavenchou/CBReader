// ---------------------------------------------------------------------------
#ifndef SpineH
#define SpineH

#include <fmx.h>
#include <vector>
#include "System.RegularExpressions.hpp"
#include "System.IOUtils.hpp"

// ---------------------------------------------------------------------------
using namespace std;

class CSpine
{
private: // User declarations

public: // User declarations

	TStringList * Files;    // 遍歷檔案
	// CBETA 特有的
	TStringList * Vol;		// 冊
	TStringList * Sutra;	// 經
	TStringList * Juan;		// 卷

	void __fastcall LoadSpineFile(String sFile);      	// 載入文件
	__fastcall CSpine();
	__fastcall ~CSpine();
};
// ---------------------------------------------------------------------------
#endif
