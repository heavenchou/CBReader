//---------------------------------------------------------------------------

#ifndef BookDataH
#define BookDataH

#include <fmx.h>
#include <vector>
#include "System.RegularExpressions.hpp"
#include "System.IOUtils.hpp"

// ---------------------------------------------------------------------------
using namespace std;

class CBookData
{
private: // User declarations

public: // User declarations

	TStringList * ID;         // 藏經代碼
	TStringList * VolCount;       // 冊數的位數
	TStringList * VerName;       // 版本名稱, 例如大正藏是【大】
	TStringList * BookName;      // 中文名稱
	TStringList * BookEngName;        // 英文名稱

	// 傳入檔案, 初值化
	void __fastcall LoadBookDataFile(String sFile);
	// 取得指定藏經的冊數位數, 例如大正藏是 2 位數
	int __fastcall GetVolLen(String sBook);
	// 傳入 T, 1 , 傳回 "T01" 這種標準的冊數
	String __fastcall GetFullVolString(String sBook, String sVol);
	// 傳入 T, 1 , 傳回 "01" 這種標準的冊數
	String __fastcall GetNormalVolNumString(String sBook, String sVol);
	// 傳入 T, 傳回 "【大】" 版本名稱 , 找不到傳回空字串
	String __fastcall GetVerName(String sBook);
	// 傳入 T, 傳回其在 TStringList 的 Index , 沒有則傳回 -1
	int __fastcall GetBookIndex(String sBook);

	__fastcall CBookData();
	__fastcall ~CBookData();
};
// ---------------------------------------------------------------------------
#endif
