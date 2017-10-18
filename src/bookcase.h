// ---------------------------------------------------------------------------

#ifndef BookcaseH
#define BookcaseH

#include "series.h"
#include <fmx.h>
#include <System.IOUtils.hpp>

// ---------------------------------------------------------------------------

class CBookcase
{
private: // User declarations



public: // User declarations

	String BookcaseDir; // ���d���ؿ�
    TList * Books;      // �Ҧ��M�Ѫ�����

	void __fastcall LoadBookcase(String sDir);  // ���J�Ҧ������d
	int __fastcall Count();  // Books ���ƶq

	__fastcall CBookcase();
	__fastcall ~CBookcase();
};
// ---------------------------------------------------------------------------
#endif
