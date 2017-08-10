//---------------------------------------------------------------------------

#ifndef cbxmltestH
#define cbxmltestH
//---------------------------------------------------------------------------

#include "../src/cbxml.h"
#include "../src/cbxmloption.h"
class CCBXMLTest
{
private: // User declarations

    String MyFullPath;
	// �ǤJ�n�x�s���G���r��C��
	TStringList * Titles;
	TStringList * Results;

	CCBXML * CBXML;
	CCBXMLOption * CBXMLOption; // �g��e�{�]�w��

public: // User declarations

	void __fastcall RunAllTest();							// �����������
	void __fastcall LogTest(String sTitle, bool bResult);  // �x�s���յ��G

	// �n���ժ�����

	bool __fastcall CBXMLSaveToHTML();

	__fastcall CCBXMLTest(TStringList * slTitle, TStringList * slResult);
};
//---------------------------------------------------------------------------
#endif
