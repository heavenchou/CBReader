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
#include "rendattr.h"
#include "nextline.h"
#include "main.h"
#include "System.RegularExpressions.hpp"
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

	// --------------------------

	String DivType[30];		// �̦h 20 �h
	int DivCount;   		// Div ���h��

	bool InByline;  		// �ΨӧP�_�O�_�O�@Ķ��
	int  FuWenCount;		// �ΨӧP�_�O�_�O����, �]�����_��, �ҥH�� int
	bool InSutraNum;	   	// �ΨӧP�_����O�_�O�g��
	bool InPinHead;			// �ΨӧP�_����O�_�O�~�W
	bool InFuWenHead;		// �ΨӧP�_����O�_�O������D
	bool InOtherHead;		// �ΨӧP�_����O�_�O�䥦���D
	bool InHead;			// �ΨӧP�_����O�_�O���D
	int  NoNormal;			// �ΨӧP�_�O�_�i�γq�Φr , �j�� 0 �N���γq�Φr, �o�G�شN���γq�Φr <text rend="no_nor"> �� <term rend="no_nor">

	// �U�|����
	bool IsFindLg;			// �@�J�� <lg> �N true, �Ĥ@�� <l> �N�|�B�z�ó]�� false;
	int  LgCount;              // �P�_�O���O�b <lg> ����, �D�n�O�ΨӳB�z�U�|�������I�n���n�e�{.
	bool LgNormal;		    // lg �� type �O���O normal? �� normal �� abnormal �G��
	bool LgInline;          // lg �� place �O���O inline?
	String LgMarginLeft;	// lg ��q�n�Ū���
	// L
	int LTagNum;		    // <l> �X�{���Ʀr, �ΨӧP�_�n�b���Ϊ��g�X�ӪŮ�
	String LMarginLeft;	    // L���Ů�

	bool InTTNormal;		// �b <tt rend="normal"> ��, �o�ɨC�@�� <t> ���n���� , T54n2133A : <lb n="1194c17"/><p><tt rend="normal"><t lang="san-sd">
	int  PreFormatCount;	// �P�_�O�_�O�n�̾ڭ�l�g��榡����, �n�֥[��, �]���i�঳�_���� pre
	String MarginLeft;		// ����
	//String NormalWords; 	// �q�ε��B�z�k, �Y�O orig , �N�O�e�{ <orig> �����r, �Y�O "reg" �N�O�e�{ <reg> �����r, �o�O�b choice �аO���P�_

	int ListCount;			// �p�� list ���ƥ�, ���@�Ǧa��ݭn�Ψ�
	int ItemNum[100];		// �ΨӧP�_ item �X�{������, �C�@�h list �������P�����e

	int CellNum;            // �p��ĴX�� Cell, �ΨӧP�_�n�b���Ϊ��g�X�ӪŮ�
	int OtherColspan;       // �]�� cell �� n ��H�W, �ҥH�M�᭱�� cell �n�� (n-1)*3 ���Ů�, ���Y�O�� n-1 ���Ʀr

	// �n�P�_�~���d��, �Y�X�{�~�� mulu, �h�O�� level, ���� level �Ʀr�A���j��ε����, ���~�~����
	//<mulu level="3" label="3 ����t���~" type="�~"/>
	int MuluLevel;          // �ؿ����h��
	String MuluLabel;		// �ؿ����W��
    bool InMulu;			// �b <cb:mulu>...</cb:mulu> ���d��, ��r�h���e�{,
	bool InMuluPin;			// �b <cb:mulu>...</cb:mulu> ���d��, �ӥB�O "�~" , �h��r���e�{, ���n�O���� MuluLabel

	int NoteAddNum;			// �ۭq�յ� <note type="add" ���y����, �� 1 �}�l
	map<String, int> mpNoteAddNum;	// �� id ��X �y����, �S���N�]�w�@��
	map<String, int> mpNoteStarNum; // �O���C�@�� id ���h�֭ӬP���F

	// --------------------------

	CNextLine * NextLine;		// �ΨӳB�z�j�� <tt> ������

	// --------------------------


	// �B�z�аO
	String __fastcall tag_anchor(_di_IXMLNode Node);
	String __fastcall tag_app(_di_IXMLNode Node);
	String __fastcall tag_byline(_di_IXMLNode Node);
	String __fastcall tag_cell(_di_IXMLNode Node);
	String __fastcall tag_div(_di_IXMLNode Node);
	String __fastcall tag_docNumber(_di_IXMLNode Node);
	String __fastcall tag_entry(_di_IXMLNode Node);
	String __fastcall tag_figdesc(_di_IXMLNode Node);
	String __fastcall tag_foreign(_di_IXMLNode Node);
	String __fastcall tag_form(_di_IXMLNode Node);
	String __fastcall tag_formula(_di_IXMLNode Node);
	String __fastcall tag_g(_di_IXMLNode Node);
	String __fastcall tag_graphic(_di_IXMLNode Node);
	String __fastcall tag_head(_di_IXMLNode Node);
	String __fastcall tag_item(_di_IXMLNode Node);
	String __fastcall tag_juan(_di_IXMLNode Node);
	String __fastcall tag_l(_di_IXMLNode Node);
	String __fastcall tag_lb(_di_IXMLNode Node);
	String __fastcall tag_lem(_di_IXMLNode Node);
	String __fastcall tag_lg(_di_IXMLNode Node);
	String __fastcall tag_list(_di_IXMLNode Node);
	String __fastcall tag_mulu(_di_IXMLNode Node);
	String __fastcall tag_note(_di_IXMLNode Node);
	String __fastcall tag_p(_di_IXMLNode Node);
	String __fastcall tag_pb(_di_IXMLNode Node);
	String __fastcall tag_rdg(_di_IXMLNode Node);
	String __fastcall tag_ref(_di_IXMLNode Node);
	String __fastcall tag_row(_di_IXMLNode Node);
	String __fastcall tag_sg(_di_IXMLNode Node);
	String __fastcall tag_space(_di_IXMLNode Node);
	String __fastcall tag_t(_di_IXMLNode Node);
	String __fastcall tag_table(_di_IXMLNode Node);
	String __fastcall tag_term(_di_IXMLNode Node);
	String __fastcall tag_trailer(_di_IXMLNode Node);
	String __fastcall tag_tt(_di_IXMLNode Node);
	String __fastcall tag_unclear(_di_IXMLNode Node);
	String __fastcall tag_default(_di_IXMLNode Node);

	// �B�z XML
	String __fastcall ParseXML(); // �ѪR XML , �x�s�� TreeRoot ��
	String __fastcall ParseNode(_di_IXMLNode Node); // �ѪR XML Node
	String __fastcall parseChild(_di_IXMLNode Node); // �ѪR XML Child

	String __fastcall MakeHTMLHead(); 	// ������ html �� head

	// �q�� note orig , ���հɦ� mod ��
	// �N�|�� orig note �� class �� note_mod ����
	void __fastcall ThisNoteHasMod(String sIdNormal);
	// �쥻�� orig �հ��٨S�[�J, ���ɤ~�n�[�J
	String __fastcall AddOrigNote(String HTMLText);

	// �ǤJ note �аO�� id , �Ǧ^�y����, �Y�S���N�۰� + 1 �öǦ^
	int __fastcall Get_Add_IdNum(String sId);

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

	// �B�z XML �L�{�ݭn���ܼ�

	String BookId;		// ���e�O 'T'(�j����), 'X'(������)
	String VolId;      	// ���e�O 01
	String SutraId;		// ���e�O "0001" or "0143a"
	String SutraId_;	// ���e�O "0001_" or "0143a"
	String SutraName;	// �g�W
	int    JuanNum;		// �ĴX��
    int    TotalJuan;   // �@�X��, �ΨӧP�_�O���O�u���@��

	String GotoPageLine;	// �������n���쪺�a�I (�]�����@�w�O����)

	String BookVolnSutra;	// ���e�O T01n0001_
	String PageLine;		// ���e�O 0001a01
	String LineHead;		// ���e�O T01n0001_p0001a01��

	bool ShowHighlight; 	// �O�_���

	String BookVerName;     // �Ҧp�j���ìO �i�j�j,�o�O�n�Ѩ�L��Ƨ�X�Ӫ�

	String SerialPath;      // �D�n�ؿ�, �n����ɦ�m�Ϊ�

	void __fastcall GetInitialFromFileName();   // �Ѹg�W���o�@��������T

	void __fastcall SaveToHTML(String sFile);	// �x�s�� HTML

	inline String __fastcall GetAttr(_di_IXMLNode Node, String sAttr); // ���o�ݩ�
	String __fastcall NoteId2Num(String sId); // ��հ�ID �ܦ��հɼƦr 0001001 -> 1
	void __fastcall ReadGaiji(_di_IXMLNode xmlNodeGaijis); // Ū���ʦr

	// ���o�U�@�� note , ���]�����@�ǬO <lb type=old> , <pb type=old> <lb id=Rxx> �n����
	_di_IXMLNode __fastcall GetNextSiblNode(_di_IXMLNode Node);

	// �� <tr/>..<tr><td> ��������Ʋ��� <td> �̭�
	String __fastcall mv_data_between_tr(String sHtml);
	// ���N���W�� : �� <tr/>..<tr><td> ��������Ʋ��� <td> �̭�
	String __fastcall TableTrReplace (const TMatch &Match);

	// �ǤJ�ѼƬ� XML ��, �e�{���]�w
	__fastcall CCBXML(String sFile, String sLink, CSetting * csSetting, String sJSFile, bool bShowHighlight = false, TmyMonster * seSearchEngine = 0);
	__fastcall ~CCBXML();
};
// ---------------------------------------------------------------------------
#endif
