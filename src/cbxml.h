// ---------------------------------------------------------------------------

#ifdef _Windows
#include <Xml.Win.msxmldom.hpp>
#else
#include <Xml.omnixmldom.hpp>
#endif

#ifndef CBXMLH
#define CBXMLH
#include <fmx.h>
#include <Xml.XMLDoc.hpp>
#include <System.IOUtils.hpp>
#include "setting.h"

// ---------------------------------------------------------------------------
// �o�O�@��Ū��������󪺪���

class CCBXML
{
private: // User declarations

	String XMLFile; // XML �ɦW
	UnicodeString HTMLText;	// HTML �����G
	CSetting * Setting; // �e�{�Ϊ��]�w

	_di_IXMLDocument Document;

	void __fastcall MakeHTMLHead(); 	// ������ html �� head
	// --------------------------

	// �B�z�аO
	void __fastcall tag_a(_di_IXMLNode Node);
	void __fastcall tag_lb(_di_IXMLNode Node);
	void __fastcall tag_pb(_di_IXMLNode Node);
	void __fastcall tag_p(_di_IXMLNode Node);
	void __fastcall tag_div(_di_IXMLNode Node);
	void __fastcall tag_note(_di_IXMLNode Node);
	void __fastcall tag_default(_di_IXMLNode Node);

	// �B�z XML
	void __fastcall ParseXML(); // �ѪR XML , �x�s�� TreeRoot ��
	void __fastcall ParseNode(_di_IXMLNode Node); // �ѪR XML Node
	void __fastcall parseChild(_di_IXMLNode Node); // �ѪR XML Child

public: // User declarations

	void __fastcall SaveToHTML(String sFile);	// �x�s�� HTML

	// �ǤJ�ѼƬ� XML ��, �e�{���]�w
	__fastcall CCBXML(String sFile, CSetting * csSetting);
	__fastcall ~CCBXML();
};
// ---------------------------------------------------------------------------
#endif
