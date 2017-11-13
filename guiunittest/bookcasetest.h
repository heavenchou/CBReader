//---------------------------------------------------------------------------
#include <fmx.h>
#ifndef BookcaseTestH
#define BookcaseTestH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "../src/bookcase.h"
#include "../src/series.h"
//---------------------------------------------------------------------------
/* �u�@��z
	�D�{���ǤJ�G�� TStringList ������, �@�Ӧs����, �@�Ӧs���G
	�I�s RunAllTest();
	�N�v�@���յ{��,
	�óv�@�� "������r" �M ���L���G �s�J�G�� List ��.
	���\�Ǧ^ 1 , ���ѶǦ^ 0
*/

class CBookcaseTest
{
private:	// User declarations
public:		// User declarations

	// �ǤJ�n�x�s���G���r��C��
	TStringList * Titles;
	TStringList * Results;

	String MyFullPath;

	CBookcase * Bookcase;
	CSeries * Series;
	
	void __fastcall RunAllTest();							// �����������
	void __fastcall LogTest(String sTitle, bool bResult);  // �x�s���յ��G

	// �n���ժ�����

	bool __fastcall BookcaseFindDir();
	bool __fastcall BookcaseCount();
	bool __fastcall ReadSeriesTitle();
	bool __fastcall ReadSeriesNav();
	bool __fastcall FindXMLBySutraJuan(String sBookID, String sSutra, String sJuan, String sFile);
	bool __fastcall FindXMLByVolPageFieldLine(String sBookID, String sVol, String sPage, String sField, String sLine, String sFile);
	bool __fastcall ChkNormalVol(String sBookID, String sVol, String sVolResult);
	bool __fastcall ChkNormalSutra(String sSutra, String sResult);
	bool __fastcall ChkNormalPage(String sPage, String sResult);
	bool __fastcall ChkNormalLine(String sLine, String sResult);
	__fastcall CBookcaseTest(TStringList * slTitle, TStringList * slResult);
};

//---------------------------------------------------------------------------
#endif
