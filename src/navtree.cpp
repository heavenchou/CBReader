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
	/*
	// �ˬd���G
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
__fastcall CNavTree::~CNavTree(void) // �Ѻc�禡
{
	if (TreeRoot)
	{
		// ���M���Ҧ��� Item
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
