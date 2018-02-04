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
#include <System.SysUtils.hpp>
#include <map>
#include "setting.h"
#include "subutil.h"
#include "highlight.h"
#include "../../Monster/src/monster.h"

using namespace std;
// ---------------------------------------------------------------------------
// �o�O�@��Ū��������󪺪���

class CCBXML
{
private: // User declarations

	String XMLFile; // XML �ɦW
	String HTMLText;	// HTML �����G
	String HTMLCollation;    // HTML �յ�
	CSetting * Setting; // �e�{�Ϊ��]�w
	String JSFile;  	// javascript ����m

	_di_IXMLDocument Document;

	String __fastcall MakeHTMLHead(); 	// ������ html �� head

	// --------------------------

	// �B�z�аO
	String __fastcall tag_app(_di_IXMLNode Node);
	String __fastcall tag_div(_di_IXMLNode Node);
	String __fastcall tag_g(_di_IXMLNode Node);
	String __fastcall tag_lb(_di_IXMLNode Node);
	String __fastcall tag_lem(_di_IXMLNode Node);
	String __fastcall tag_mulu(_di_IXMLNode Node);
	String __fastcall tag_note(_di_IXMLNode Node);
	String __fastcall tag_p(_di_IXMLNode Node);
	String __fastcall tag_pb(_di_IXMLNode Node);
	String __fastcall tag_rdg(_di_IXMLNode Node);
	String __fastcall tag_t(_di_IXMLNode Node);
	String __fastcall tag_default(_di_IXMLNode Node);

	// �B�z XML
	String __fastcall ParseXML(); // �ѪR XML , �x�s�� TreeRoot ��
	String __fastcall ParseNode(_di_IXMLNode Node); // �ѪR XML Node
	String __fastcall parseChild(_di_IXMLNode Node); // �ѪR XML Child

	// �q�� note orig , ���հɦ� mod ��
	// �N�|�� orig note �� class �� note_mod ����
	void __fastcall ThisNoteHasMod(String sIdNormal);
	// �쥻�� orig �հ��٨S�[�J, ���ɤ~�n�[�J
	String __fastcall AddOrigNote(String HTMLText);

public: // User declarations

	// �ʦr
	map<String, String> CB2des;     // �ʦr CB �X�d�զr��
    map<String, String> CB2nor;     // �ʦr CB �X�d�q�Φr
    map<String, String> CB2uni;     // �ʦr CB �X�d unicode
    map<String, String> CB2nor_uni; // �ʦr CB �X�d�q�� unicode

    map<String, String> SD2roma;    // �x��r SD �X�dù����g�r (�¤�r)
	map<String, String> SD2uni;     // �x��r SD �X�dù����g�r (Unicode)
    map<String, String> SD2char;    // �x��r SD �X�d��M TTF �r�����r
    map<String, String> SD2big5;    // �x��r SD �X�d Big5
	//map<String, String> RJ2roma;    // �����r RJ �X�dù����g�r (�¤�r)
	//map<String, String> RJ2uni;     // �����r RJ �X�dù����g�r (Unicode)
	//map<String, String> RJ2char;    // �����r RJ �X�d��M TTF �r�����r
	//map<String, String> RJ2big5;    // �����r RJ �X�d Big5

    map<String, String> mOrigNote;  // �Ȧs�� orig note

    bool ShowHighlight; 	// �O�_���

	void __fastcall SaveToHTML(String sFile);	// �x�s�� HTML

	inline String __fastcall GetAttr(_di_IXMLNode Node, String sAttr); // ���o�ݩ�
	String __fastcall NoteId2Num(String sId); // ��հ�ID �ܦ��հɼƦr 0001001 -> 1
	void __fastcall ReadGaiji(_di_IXMLNode xmlNodeGaijis); // Ū���ʦr

	// �ǤJ�ѼƬ� XML ��, �e�{���]�w
	__fastcall CCBXML(String sFile, CSetting * csSetting, String sJSFile, bool bShowHighlight = false, TmyMonster * seSearchEngine = 0);
	__fastcall ~CCBXML();
};
// ---------------------------------------------------------------------------
#endif
