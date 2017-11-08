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
	__fastcall CBookData();
	__fastcall ~CBookData();
};
// ---------------------------------------------------------------------------
#endif
