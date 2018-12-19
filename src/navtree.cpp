// ---------------------------------------------------------------------------

#pragma hdrstop

#include "navtree.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CNavTree::CNavTree(String sFile) // �غc�禡
{
	XMLFile = sFile;
	TreeRoot = new TList();
	NavXML = new CNavXML(XMLFile);
	NavXML->SaveToTree(TreeRoot); // �N XML ���J TreeRoot
}
// ---------------------------------------------------------------------------
__fastcall CNavTree::~CNavTree(void) // �Ѻc�禡
{
	if (TreeRoot)
	{
		// ���M���Ҧ��� Item
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
// �N��Ƨe�{�b�𪬥ؿ��W
void __fastcall CNavTree::SaveToTreeView(TTreeView * tvTreeView, TNotifyEvent fnLinkFunc)
{
	TreeView = tvTreeView;
	TreeView->Clear();
	TTreeViewItem * ParentItem = 0;  // �Ĥ@�� ParentItem �O TreeView
	int iPreLevel = -1;  // �e�@�� item ���h��, �w�]�� 0

	for(int i=0; i<TreeRoot->Count; i++)
	{
		SNavItem * nItem = (SNavItem *)TreeRoot->Items[i];
		int iThisLevel = nItem->Level;   // �n�إߪ��h��

		// �Y�O�U�@�h, �h�� Item �N�O�e�@�� Item ���l�h
		// �Y���O�U�@�h, �h�n���W��
		// ex . �e�@�h�O 4 , �o�h�]�O 4 , �ҥH�o�h�����h�N�O 3
		//    �Y�e�@�h�O 4 , �o�h�]�O 2 , �ҥH�o�h�����h�N�O 1

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
// �]�w Item ���e, �å[�� NavTreeView ����, �̫�Ǧ^�`�I��m
TTreeViewItem *  __fastcall CNavTree::TreeViewAddItem(TTreeViewItem * tvItem, SNavItem * nItem, TNotifyEvent fnLinkFunc)
{
	TTreeViewItem * newItem = new TTreeViewItem(TreeView);
	newItem->Text = nItem->Title;   // ���D
	newItem->TagString = nItem->URL;    // URL
	newItem->Tag = nItem->Type;     // Type
	newItem->OnDblClick = fnLinkFunc;    // Double Click �n�s����
	if(nItem->Type == nit_Title)
		newItem->ImageIndex = 0;    // �ؿ�
	else if(nItem->Type == nit_NavLink)
		newItem->ImageIndex = 2;    // �t�@�ӥؿ�
	else if(nItem->Type == nit_CBLink)
		newItem->ImageIndex = 3;    // CBETA �g��
	else if(nItem->Type == nit_NormalLink)
		newItem->ImageIndex = 4;    // �@��g��

	tvItem->AddObject(newItem);
	return newItem ;
}
// ---------------------------------------------------------------------------

