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

	TList * TreeRoot; // �𪬥ؿ�, ���e�Y�O NavItem
	CNavXML * NavXML;
	String XMLFile; // XML �ɦW

public: // User declarations

	TList * __fastcall GetTreeRoot(void);
	__fastcall CNavTree(String sFile);
	__fastcall ~CNavTree();
};
// ---------------------------------------------------------------------------
#endif
