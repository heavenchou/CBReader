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
// ---------------------------------------------------------------------------

class CSeries
{
private: // User declarations

public: // User declarations

	String Dir; 	// 本書的目錄

	String Title;       // 標題
	String Creator;     // 作者

	String NavFile;     // 導覽文件
	String TocFile;     // 目錄文件
	String SpineFile;   // 遍歷文件

	void __fastcall LoadMetaData(String sMeta);	// 載入後設文件
	__fastcall CSeries(String sDir);    // 傳入目錄, 進行初值化
	__fastcall ~CSeries();
};
// ---------------------------------------------------------------------------
#endif
