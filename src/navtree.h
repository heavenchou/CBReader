// ---------------------------------------------------------------------------

#ifndef NavTreeH
#define NavTreeH
// ---------------------------------------------------------------------------
#include <fmx.h>
#include "navxml.h"
// ---------------------------------------------------------------------------
// 這是一個讀取導覽文件的物件

class CNavTree
{
private: // User declarations

	TList * TreeRoot; // 樹狀目錄, 內容即是 NavItem
	CNavXML * NavXML;
	String XMLFile; // XML 檔名

public: // User declarations

	TList * __fastcall GetTreeRoot(void);
	__fastcall CNavTree(String sFile);
	__fastcall ~CNavTree();
};
// ---------------------------------------------------------------------------
#endif
