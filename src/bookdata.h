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
	TStringList * BookName;      // ����W��
	TStringList * BookEngName;        // �^��W��

	void __fastcall LoadBookDataFile(String sFile);      	// �ǤJ�ɮ�, ��Ȥ�
	// ���o���w�øg���U�Ʀ��, �Ҧp�j���ìO 2 ���
	int __fastcall GetVolLen(String sBook);
	// �ǤJ T, 1 , �Ǧ^ "T01" �o�ؼзǪ��U��
	String __fastcall GetFullVolString(String sBook, String sVol);
	__fastcall CBookData();
	__fastcall ~CBookData();
};
// ---------------------------------------------------------------------------
#endif
