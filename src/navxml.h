// ---------------------------------------------------------------------------

#ifdef _Windows
#include <Xml.Win.msxmldom.hpp>
#else
#include <Xml.omnixmldom.hpp>
#endif

#ifndef NavXMLH
#define NavXMLH
#include <fmx.h>
#include <Xml.XMLDoc.hpp>
#include "navitem.h"
// ---------------------------------------------------------------------------
// �o�O�@��Ū��������󪺪���

class CNavXML
{
private: // User declarations

	String XMLFile; // XML �ɦW
	TList * TreeRoot;
	_di_IXMLDocument Document;

	// Parse �L�{���ݭn�Ȧs�����
	int ThisLevel;
	SNavItem ThisItem;
	// --------------------------

	// �B�z NavItem
	void __fastcall ClearThisItem(); // �M�� ThisItem
	void __fastcall AddThisItem(); // �N�ثe�� ThisItem �[�J Tree �òM��

	// �B�z�аO
	void __fastcall tag_nav(_di_IXMLNode Node);
	void __fastcall tag_ol(_di_IXMLNode Node);
	void __fastcall tag_li(_di_IXMLNode Node);
	void __fastcall tag_a(_di_IXMLNode Node);
	void __fastcall tag_cblink(_di_IXMLNode Node);
	void __fastcall tag_navlink(_di_IXMLNode Node);
	void __fastcall tag_default(_di_IXMLNode Node);

	// �B�z XML
	void __fastcall ParseXML(); // �ѪR XML , �x�s�� TreeRoot ��
	void __fastcall ParseNode(_di_IXMLNode Node); // �ѪR XML Node
	void __fastcall parseChild(_di_IXMLNode Node); // �ѪR XML Child

public: // User declarations

	void __fastcall SaveToTree(TList * TreeRoot);

	__fastcall CNavXML(String sFile);
	__fastcall ~CNavXML();
};
/*
	nav.xhtml �аO�P TreeRoot �����Y

	<nav> : ��ܬO�Ĥ@�h, �����W�@�h�����
	<ol> : ����U�@�h�F
	<li> : �����W�@�h�����, �[�J TreeRoot ��
	��r : �O����r

*/
// ---------------------------------------------------------------------------
#endif
