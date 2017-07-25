//---------------------------------------------------------------------------

#ifndef SampleH
#define SampleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
/* �u�@��z
	�D�{���ǤJ�G�� TStringList ������, �@�Ӧs����, �@�Ӧs���G
	�I�s RunAllTest();
	�N�v�@���յ{��,
	�óv�@�� "������r" �M ���L���G �s�J�G�� List ��.
	���\�Ǧ^ 1 , ���ѶǦ^ 0
*/

class CSample
{
private:	// User declarations
public:		// User declarations

	// �ǤJ�n�x�s���G���r��C��
	TStringList * Titles;
	TStringList * Results;

	void __fastcall RunAllTest();							// �����������
	void __fastcall LogTest(String sTitle, bool bResult);  // �x�s���յ��G

	// �n���ժ�����
	bool __fastcall Test1();
	bool __fastcall Test2();

	__fastcall CSample(TStringList * slTitle, TStringList * slResult);
};

//---------------------------------------------------------------------------
#endif
