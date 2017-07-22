// ---------------------------------------------------------------------------

#pragma hdrstop

#include "navtree.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CNavTree::CNavTree(String sFile) // 建構函式
{
	XMLFile = sFile;
	TreeRoot = new TList();
	NavXML = new CNavXML(XMLFile);
	NavXML->SaveToTree(TreeRoot); // 將 XML 載入 TreeRoot
	/*
	// 檢查結果
	String s = "";
	for (int i = 0; i < TreeRoot->Count; i++) {
	SNavItem * it = (SNavItem*)TreeRoot->Items[i];
	s = s + it->Level + "," + it->Type + "," + it->Title + "," + it->URL;
	s += "\n";
	}
	ShowMessage(s);
	*/
}

// ---------------------------------------------------------------------------
__fastcall CNavTree::~CNavTree(void) // 解構函式
{
	if (TreeRoot)
	{
		// 先清除所有的 Item
		for (int i = 0; i < TreeRoot->Count; i++)
			delete TreeRoot->Items[i];
		TreeRoot->Clear();
		delete TreeRoot;
	}
	if (NavXML)	delete NavXML;
}

// ---------------------------------------------------------------------------
TList * __fastcall CNavTree::GetTreeRoot(void)
{
	return TreeRoot;
}
// ---------------------------------------------------------------------------
