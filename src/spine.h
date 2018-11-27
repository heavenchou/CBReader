// ---------------------------------------------------------------------------
#ifndef SpineH
#define SpineH

#include <fmx.h>
#include <vector>
#include <System.RegularExpressions.hpp>
#include <System.IOUtils.hpp>
#include <FMX.DialogService.hpp>
#include <utility>  // for std::pair

// ---------------------------------------------------------------------------
using namespace std;

class CSpine
{
private: // User declarations

public: // User declarations

	TStringList * Files;    // 遍歷檔案
	// CBETA 特有的
	TStringList * BookID;       // 書, 例如 T
	TStringList * VolNum;     // 冊數, 例如 01
	TStringList * Vol;		// 冊
	TStringList * Sutra;	// 經
	TStringList * Juan;		// 卷

	void __fastcall LoadSpineFile(String sFile);      	// 載入文件

	// 傳回標準的經號格式, "0001" 或 "0001a"
	String __fastcall CBGetSutraNumFormat(String sSutraNum);

	// 由經卷去找 Spine 的 Index , 也可以不指定卷
	int __fastcall CBGetSpineIndexBySutraNumJuan(String sBookID,String sVol,String sSutraNum,String sJuan = "");
	// 由經卷去找 XML 檔名
	String __fastcall CBGetFileNameBySutraNumJuan(String sBookID,String sVol,String sSutraNum,String sJuan = "");
	// 由 Spine 的 Index 去找 XML 檔名
	String __fastcall CBGetFileNameBySpineIndex(int iIndex);


	__fastcall CSpine();
	__fastcall ~CSpine();
};
// ---------------------------------------------------------------------------
#endif
