// ---------------------------------------------------------------------------


#ifndef SeriesH
#define SeriesH


#ifdef _Windows
#include <Xml.Win.msxmldom.hpp>
#else
#include <Xml.omnixmldom.hpp>
#endif

#include <Xml.XMLDoc.hpp>
#include <fmx.h>

#include "catalog.h"
#include "spine.h"
#include "bookdata.h"
#include "juanline.h"
#include <utility>  // for std::pair
// ---------------------------------------------------------------------------

class CSeries
{
private: // User declarations

public: // User declarations

	String Dir; 	// 本書的目錄

    String ID;          // ID 代碼, CBETA 的指定為 CBEAT
	String Title;       // 標題
	String Creator;     // 作者

	String NavFile;     // 導覽文件
	String CatalogFile;	// 目錄文件
	String SpineFile;   // 遍歷文件
	String BookDataFile;   // BookData 文件

	CCatalog * Catalog; // 目錄
	CSpine * Spine;		// 遍歷文件
	CJuanLine * JuanLine;	// 各卷與頁欄行的關係物件, CBETA 專用
	CBookData * BookData;   // 每本書的資訊, 例如 T , 大正藏, 2

	void __fastcall LoadMetaData(String sMeta);	// 載入後設文件

	// 由經卷去找經文
	String __fastcall CBGetFileNameBySutraNumJuan(String sBookID, String sSutraNum, String sJuan = "");
	// 由冊頁欄行找經文
	String __fastcall CBGetFileNameByVolPageFieldLine(String sBook, String sVol = "", String sPage = "", String sField = "", String sLine = "");
	__fastcall CSeries(String sDir);    // 傳入目錄, 進行初值化
	__fastcall ~CSeries();
};
// ---------------------------------------------------------------------------
#endif
