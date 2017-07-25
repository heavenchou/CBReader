// ---------------------------------------------------------------------------

#ifndef NavTreeH
#define NavTreeH
// ---------------------------------------------------------------------------
#include <fmx.h>
#include "navxml.h"
// ---------------------------------------------------------------------------
// �o�O�@��Ū��������󪺪���

class CNavTree
{
private: // User declarations

	TTreeView * TreeView;   // �ǤJ�� TreeView
	TList * TreeRoot; // �𪬥ؿ�, ���e�Y�O NavItem
	CNavXML * NavXML;
	String XMLFile; // XML �ɦW

public: // User declarations

	TList * __fastcall GetTreeRoot(void);

	// �N��Ƨe�{�b�𪬥ؿ��W, �ǤJ��έn�s�����禡
	void __fastcall SaveToTreeView(TTreeView * TreeView, TNotifyEvent fnLinkFunc);
	// �]�w Item ���e, �å[�� NavTreeView ����, �̫�Ǧ^�`�I��m
	TTreeViewItem *  __fastcall TreeViewAddItem(TTreeViewItem * tvItem, SNavItem * nItem, TNotifyEvent fnLinkFunc);

	__fastcall CNavTree(String sFile);
	__fastcall ~CNavTree();
};
// ---------------------------------------------------------------------------
#endif
