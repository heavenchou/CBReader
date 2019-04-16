// ---------------------------------------------------------------------------

#pragma hdrstop

#include "navxml.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CNavXML::CNavXML(String sFile) // �غc�禡
{
	// ���
	XMLFile = sFile;
	TreeRoot = 0;
	ThisLevel = 0;
	ClearThisItem();

	Document = interface_cast<Xmlintf::IXMLDocument>(new TXMLDocument(NULL));
	Document->FileName = XMLFile;
}

// ---------------------------------------------------------------------------
__fastcall CNavXML::~CNavXML(void) // �Ѻc�禡
{
	// ���U�� delete �b mac �|��, �n�䥿�W��k ???
	// if (Document) delete Document;
}

// ---------------------------------------------------------------------------
// �M�� ThisItem
void __fastcall CNavXML::ClearThisItem()
{
	ThisItem.Level = -1;
	ThisItem.Type = nit_Title;
	ThisItem.Title = "";
	ThisItem.URL = "";
}

// ---------------------------------------------------------------------------
// �N�ثe�� ThisItem �[�J Tree �òM��
void __fastcall CNavXML::AddThisItem()
{
	SNavItem * item = new SNavItem;
	if(*ThisItem.Title.FirstChar() == ' ')
	{
		ThisItem.Title.Delete0(0,1);
	}
	*item = ThisItem;
	TreeRoot->Add(item);
}

// ---------------------------------------------------------------------------
// �N XML Parse ��s�J TreeRoot ��
void __fastcall CNavXML::SaveToTree(TList * root)
{
	TreeRoot = root;
	ParseXML();
}

// ---------------------------------------------------------------------------
// �ѪR XML , �x�s�� TreeRoot ��
void __fastcall CNavXML::ParseXML()
{
	_di_IXMLNode Node;

	TreeRoot->Clear();
	Document->Active = true;

	// �M�� XML

	Node = Document->DocumentElement->ChildNodes->Nodes["body"];

	if (Node->ChildNodes->Count == 0)
		TDialogService::ShowMessage(u"���~�G�������䤣�� body �аO�C");
	else
	{
		ParseNode(Node);
		if (ThisItem.Level != -1)
			AddThisItem();
	}
}

// ---------------------------------------------------------------------------
// �ѪR XML Node
void __fastcall CNavXML::ParseNode(_di_IXMLNode Node)
{
#ifdef _Windows
	TNodeType nodetype = Node->NodeType;
#else
	Xml::Internal::Omnixml::TNodeType nodetype = Node->NodeType;
#endif
	/* Node Type
	1	ELEMENT_NODE
	2	ATTRIBUTE_NODE
	3	TEXT_NODE
	4	CDATA_SECTION_NODE
	5	ENTITY_REFERENCE_NODE
	6	ENTITY_NODE
	7	PROCESSING_INSTRUCTION_NODE
	8	COMMENT_NODE
	9	DOCUMENT_NODE
	10	DOCUMENT_TYPE_NODE
	11	DOCUMENT_FRAGMENT_NODE
	12	NOTATION_NODE
*/

	if (nodetype == 1)
	{
		// �@��`�I
		String sTagName = Node->NodeName;
		if (sTagName == "nav")
			tag_nav(Node);
		else if (sTagName == "ol")
			tag_ol(Node);
		else if (sTagName == "li")
			tag_li(Node);
		else if (sTagName == "a")
			tag_a(Node);
		else if (sTagName == "cblink")
			tag_cblink(Node);
		else if (sTagName == "navlink")
			tag_navlink(Node);
		else
			tag_default(Node);
	}
	else if (nodetype == 3)
	{
		// ��r�`�I
		ThisItem.Title += Node->Text;
	}
}

// ---------------------------------------------------------------------------
// �ѪR XML Child
void __fastcall CNavXML::parseChild(_di_IXMLNode Node)
{
	for (int i = 0; i < Node->ChildNodes->Count; i++)
	{
		_di_IXMLNode node = Node->ChildNodes->Get(i);
		ParseNode(node);
	}
}

// ---------------------------------------------------------------------------
// �ѪR XML �аO
void __fastcall CNavXML::tag_default(_di_IXMLNode Node)
{
	parseChild(Node); // �B�z���e
}

// ---------------------------------------------------------------------------
// �ѪR XML �аO
void __fastcall CNavXML::tag_nav(_di_IXMLNode Node)
{
	// �B�z�аO

	// �J�� nav �аO�n���x�s�e�@�����
	if (ThisItem.Level != -1)
		AddThisItem();

	ClearThisItem();
	ThisLevel = 0; // �^��̤W�h
	ThisItem.Level = 0; // �]�w�s�� Item

	parseChild(Node); // �B�z���e
	// �����аO
}

// ---------------------------------------------------------------------------
// �ѪR XML �аO
void __fastcall CNavXML::tag_ol(_di_IXMLNode Node)
{
	// �B�z�аO
	ThisLevel++; // �^��̤W�h
	parseChild(Node); // �B�z���e
	// �����аO
	ThisLevel--;
}

// ---------------------------------------------------------------------------
// �ѪR XML �аO
void __fastcall CNavXML::tag_li(_di_IXMLNode Node)
{
	// �J�� li �аO�n���x�s�e�@�����
	if (ThisItem.Level != -1)
		AddThisItem();

	ClearThisItem();
	ThisItem.Level = ThisLevel;
	parseChild(Node); // �B�z���e
}

// ---------------------------------------------------------------------------
// �ѪR XML �аO
void __fastcall CNavXML::tag_a(_di_IXMLNode Node)
{
	// Ū�� href �ݩ�
	String sAttr = Node->Attributes["href"];
	if (sAttr != "")
	{
		ThisItem.URL = sAttr;
		ThisItem.Type = nit_NormalLink; // �@�몺�s��
	}
	parseChild(Node); // �B�z���e
}

// ---------------------------------------------------------------------------
// �ѪR XML �аO
void __fastcall CNavXML::tag_navlink(_di_IXMLNode Node)
{
		// Ū�� href �ݩ�
	String sAttr = Node->Attributes["href"];
	if (sAttr != "")
	{
		ThisItem.URL = sAttr;
		ThisItem.Type = nit_NavLink; // �@�몺�s��
	}
	parseChild(Node); // �B�z���e
}

// ---------------------------------------------------------------------------
// �ѪR XML �аO
void __fastcall CNavXML::tag_cblink(_di_IXMLNode Node)
{
	// Ū�� href �ݩ�
	String sAttr = Node->Attributes["href"];
	if (sAttr != "")
	{
		ThisItem.URL = sAttr;
		ThisItem.Type = nit_CBLink; // �@�몺�s��
	}
	parseChild(Node); // �B�z���e
}
// ---------------------------------------------------------------------------
