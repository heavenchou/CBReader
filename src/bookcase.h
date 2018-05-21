// ---------------------------------------------------------------------------

#ifndef BookcaseH
#define BookcaseH

#include <fmx.h>
#include <System.IOUtils.hpp>
#include <FMX.DialogService.hpp>
#include "series.h"

// ---------------------------------------------------------------------------

class CBookcase
{
private: // User declarations



public: // User declarations

	String BookcaseDir; // ���d���ؿ�
	TList * Books;      // �Ҧ��M�Ѫ�����

	CSeries * CBETA;    // �M���V CBETA ������

	void __fastcall LoadBookcase(String sDir);  // ���J�Ҧ������d
	int __fastcall Count();  // Books ���ƶq

	__fastcall CBookcase();
	__fastcall ~CBookcase();
};
// ---------------------------------------------------------------------------
#endif
