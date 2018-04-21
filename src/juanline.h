//---------------------------------------------------------------------------

#ifndef JuanLineH
#define JuanLineH

#include "System.RegularExpressions.hpp"
#include "spine.h"
#include <map>
// ---------------------------------------------------------------------------
using namespace std;

class CJuanLine
{
private: // User declarations

	/* 資料結構

	Vol{"T01"} -> 指向 2 TStringList

	1. PageLine 記錄頁欄行 : 0001a01 , 0010b10 , .......
	2. SerialNo 記錄在 Spine 的第 n 行 (流水號)

	Spine 要記錄每一卷

	XML/T/T01/T01n0001_001.xml , T01, 0001, 001

	要查 T01 的 0100a01 在哪一經哪一卷? 查詢方式為:

	1. 知道是 T01
	2. 在 Vol{"T01"}->PageLine 裡面找到 0100a01 之前是哪一筆 (假設第 5 筆)
	3. 找 Vol{"T01"}->SerialNo[5] = xx (假設在 Spine 的第 120 筆)

	4. 找 Spine->Vol[120] = "T01" , 找到在 T01
	5. 找 Spine->Sutra[120] = "0002" , 找到在第 5 經
	6. 找 Spine->Juan[120] = "03" , 找到在第 3 卷

	*/

	struct SPageLineSerialNo {
		TStringList * PageLine;
		vector <int> SerialNo;
    };

	map <String, SPageLineSerialNo *> Vol;

public: // User declarations

	void __fastcall LoadFromSpine(CSpine * Spine);      	// 載入文件

	String __fastcall GetNormalPage(String sPage);  // 取得標準 4 位數的頁碼
	String __fastcall GetNormalField(String sField);  // 取得標準 1 位數的欄
	String __fastcall GetNormalLine(String sLine);  // 取得標準 2 位數的行數
	// 新的行首, 最前面 a-m 則在字首加 "1" , 其他則加 "2"
	String __fastcall GetNewPageLine(String sPageLine);

	// 由冊頁欄行找 Spine 的 Index
	int __fastcall CBGetSpineIndexByVolPageFieldLine(String sBook, String sVol = "", String sPage = "", String sField = "", String sLine = "");

	// 傳入檔名, 找出書,冊,經,卷
	void __fastcall GetBookVolSutraJuan(String sFile, String &sBook, String &sVol, String &sSutra, String &sJuan);

	__fastcall CJuanLine();
	__fastcall ~CJuanLine();
};
// ---------------------------------------------------------------------------
#endif
