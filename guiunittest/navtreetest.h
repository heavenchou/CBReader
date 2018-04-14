//---------------------------------------------------------------------------

#ifndef NavTreeTestH
#define NavTreeTestH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "../src/navtree.h"
//---------------------------------------------------------------------------
/* �u�@��z
	�D�{���ǤJ�G�� TStringList ������, �@�Ӧs����, �@�Ӧs���G
	�I�s RunAllTest();
	�N�v�@���յ{��,
	�óv�@�� "������r" �M ���L���G �s�J�G�� List ��.
	���\�Ǧ^ 1 , ���ѶǦ^ 0
*/

class CNavTreeTest
{
private:	// User declarations
public:		// User declarations

	// �ǤJ�n�x�s���G���r��C��
	TStringList * Titles;
	TStringList * Results;

	String MyFullPath;

	CNavTree * NavTree;

	void __fastcall RunAllTest();							// �����������
	void __fastcall LogTest(String sTitle, bool bResult);  // �x�s���յ��G

	// �n���ժ�����

	bool __fastcall NavTreeReadTitle();
	bool __fastcall NavTreeReadLink();
	bool __fastcall NavTreeReadNavLink();
	bool __fastcall NavTreeReadCBLink();

	__fastcall CNavTreeTest(TStringList * slTitle, TStringList * slResult);
};

//---------------------------------------------------------------------------
#endif
