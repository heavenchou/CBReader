//---------------------------------------------------------------------------

#ifndef SubUtilH
#define SubUtilH

#include <fmx.h>
//---------------------------------------------------------------------------

// CBETA 相關函式
class CMyCBUtil
{

private:

public:

	// 將經名後面的 (第X卷-第X卷) or (第X卷) 移除
	static String __fastcall CutJuanBeforeSutraName(String sName);

};
//---------------------------------------------------------------------------

// 字串函式
class CMyStrUtil
{

private:

public:

	// 取出子字串, 由 i 開始(0 為基底), 若 i 是負數, 則表示由後面算起.
	// SubString("abcde",2) = "cde"
	// SubString("abcde",-3) = "cde"
	static String __fastcall SubString(String s, int i);

	// 取出子字串, 由 i 開始(0 為基底), 若 i 是負數, 則表示由後面算起.
	// 取出長度為 j, 若 j 為負數, 表示最後 -j 個字不用了.
	// SubString("abcde",2,2)   = "cd"
	// SubString("abcde",2,-1)  = "cd"
	// SubString("abcde",-3,2)  = "cd"
	// SubString("abcde",-3,-1) = "cd"
	static String __fastcall SubString(String s, int i, int j);

	// 傳入數字, 傳回 utf16 或 utf32 的字串
	// LongToUnicode(14201) => 㝹 (U+3779)
	// LongToUnicode(131363) => 𠄣 (U+20123)
	static String __fastcall LongToUnicode(unsigned long UTF32);

	// 傳入字串, 傳回無符號長整數, 例 StrToULong("FFFF",16) => 65535;
	static unsigned long __fastcall StrToULong(String s, int iBase=10);

	// 將傳入字串去除頭尾指定字元
	static String __fastcall Trim(String sStr, System::WideChar wChar);

	// 將傳入字串去除左邊指定字元
	static String __fastcall TrimLeft(String sStr, System::WideChar wChar);

	// 將傳入字串去除右邊指定字元
	static String __fastcall TrimRight(String sStr, System::WideChar wChar);

	// 比對指定長度的字串
	static bool __fastcall StrnCmp(System::WideChar * p1, System::WideChar * p2, int len);

	// 第一個字串是否包含第二個字串
	static bool __fastcall StrHas(System::WideChar * wp, String s);
};
//---------------------------------------------------------------------------
#endif
