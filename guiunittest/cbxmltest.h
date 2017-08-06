//---------------------------------------------------------------------------

#ifndef cbxmltestH
#define cbxmltestH
//---------------------------------------------------------------------------

#include "../src/cbxml.h"
class CCBXMLTest
{
private: // User declarations

	// �ǤJ�n�x�s���G���r��C��
	TStringList * Titles;
	TStringList * Results;

	CCBXML * CBXML;

public: // User declarations

	void __fastcall RunAllTest();							// �����������
	void __fastcall LogTest(String sTitle, bool bResult);  // �x�s���յ��G

	// �n���ժ�����

	bool __fastcall CBXMLSaveToHTML();

	__fastcall CCBXMLTest(TStringList * slTitle, TStringList * slResult);
};
//---------------------------------------------------------------------------
#endif
