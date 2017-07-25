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

	TTreeView * TreeView;   // 傳入的 TreeView
	TList * TreeRoot; // 樹狀目錄, 內容即是 NavItem
	CNavXML * NavXML;
	String XMLFile; // XML 檔名

public: // User declarations

	TList * __fastcall GetTreeRoot(void);

	// 將資料呈現在樹狀目錄上, 傳入樹及要連結的函式
	void __fastcall SaveToTreeView(TTreeView * TreeView, TNotifyEvent fnLinkFunc);
	// 設定 Item 內容, 並加到 NavTreeView 之中, 最後傳回節點位置
	TTreeViewItem *  __fastcall TreeViewAddItem(TTreeViewItem * tvItem, SNavItem * nItem, TNotifyEvent fnLinkFunc);

	__fastcall CNavTree(String sFile);
	__fastcall ~CNavTree();
};
// ---------------------------------------------------------------------------
#endif
