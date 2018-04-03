//---------------------------------------------------------------------------

#ifndef BookDataH
#define BookDataH

#include <fmx.h>
#include <vector>
#include "System.RegularExpressions.hpp"
#include "System.IOUtils.hpp"

// ---------------------------------------------------------------------------
using namespace std;

class CBookData
{
private: // User declarations

public: // User declarations

	TStringList * ID;         // �øg�N�X
	TStringList * VolCount;       // �U�ƪ����
	TStringList * VerName;       // �����W��, �Ҧp�j���ìO�i�j�j
	TStringList * BookName;      // ����W��
	TStringList * BookEngName;        // �^��W��

	// �ǤJ�ɮ�, ��Ȥ�
	void __fastcall LoadBookDataFile(String sFile);
	// ���o���w�øg���U�Ʀ��, �Ҧp�j���ìO 2 ���
	int __fastcall GetVolLen(String sBook);
	// �ǤJ T, 1 , �Ǧ^ "T01" �o�ؼзǪ��U��
	String __fastcall GetFullVolString(String sBook, String sVol);
	// �ǤJ T, 1 , �Ǧ^ "01" �o�ؼзǪ��U��
	String __fastcall GetNormalVolNumString(String sBook, String sVol);
	// �ǤJ T, �Ǧ^ "�i�j�j" �����W�� , �䤣��Ǧ^�Ŧr��
	String __fastcall GetVerName(String sBook);
	// �ǤJ T, �Ǧ^��b TStringList �� Index , �S���h�Ǧ^ -1
	int __fastcall GetBookIndex(String sBook);

	__fastcall CBookData();
	__fastcall ~CBookData();
};
// ---------------------------------------------------------------------------
#endif
