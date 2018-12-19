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
}
// ---------------------------------------------------------------------------
__fastcall CNavTree::~CNavTree(void) // 解構函式
{
	if (TreeRoot)
	{
		// 先清除所有的 Item
		for (int i = 0; i < TreeRoot->Count; i++)
			delete (SNavItem *)TreeRoot->Items[i];
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
// 將資料呈現在樹狀目錄上
void __fastcall CNavTree::SaveToTreeView(TTreeView * tvTreeView, TNotifyEvent fnLinkFunc)
{
	TreeView = tvTreeView;
	TreeView->Clear();
	TTreeViewItem * ParentItem = 0;  // 第一個 ParentItem 是 TreeView
	int iPreLevel = -1;  // 前一個 item 的層次, 預設為 0

	for(int i=0; i<TreeRoot->Count; i++)
	{
		SNavItem * nItem = (SNavItem *)TreeRoot->Items[i];
		int iThisLevel = nItem->Level;   // 要建立的層次

		// 若是下一層, 則此 Item 就是前一個 Item 的子層
		// 若不是下一層, 則要往上找
		// ex . 前一層是 4 , 這層也是 4 , 所以這層的父層就是 3
		//    若前一層是 4 , 這層也是 2 , 所以這層的父層就是 1

		if(iThisLevel != iPreLevel + 1)
		{
			for(int j=0; j< iPreLevel - iThisLevel+1; j++)
			{
				ParentItem = ParentItem->ParentItem();
			}
		}

		TTreeViewItem * tvItem;

		if(iThisLevel == 0)
			tvItem = TreeViewAddItem((TTreeViewItem *)TreeView, nItem, fnLinkFunc);
		else
			tvItem = TreeViewAddItem(ParentItem, nItem, fnLinkFunc);

		ParentItem = tvItem;
		iPreLevel = iThisLevel;
	}
}
// ---------------------------------------------------------------------------
// 設定 Item 內容, 並加到 NavTreeView 之中, 最後傳回節點位置
TTreeViewItem *  __fastcall CNavTree::TreeViewAddItem(TTreeViewItem * tvItem, SNavItem * nItem, TNotifyEvent fnLinkFunc)
{
	TTreeViewItem * newItem = new TTreeViewItem(TreeView);
	newItem->Text = nItem->Title;   // 標題
	newItem->TagString = nItem->URL;    // URL
	newItem->Tag = nItem->Type;     // Type
	newItem->OnDblClick = fnLinkFunc;    // Double Click 要連結的
	if(nItem->Type == nit_Title)
		newItem->ImageIndex = 0;    // 目錄
	else if(nItem->Type == nit_NavLink)
		newItem->ImageIndex = 2;    // 另一個目錄
	else if(nItem->Type == nit_CBLink)
		newItem->ImageIndex = 3;    // CBETA 經文
	else if(nItem->Type == nit_NormalLink)
		newItem->ImageIndex = 4;    // 一般經文

	tvItem->AddObject(newItem);
	return newItem ;
}
// ---------------------------------------------------------------------------

