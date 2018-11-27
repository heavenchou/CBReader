// ---------------------------------------------------------------------------
#ifndef CatalogH
#define CatalogH

#include <fmx.h>
#include <vector>
#include <System.RegularExpressions.hpp>
#include <System.IOUtils.hpp>
#include <FMX.DialogService.hpp>

// ---------------------------------------------------------------------------
using namespace std;

class CCatalog
{
private: // User declarations

public: // User declarations

	vector <TStringList *> Field;

	TStringList * ID;         // �øg�N�X
	TStringList * Bulei;      // ����
	TStringList * Part;       // ���O
	TStringList * Vol;        // �U��
	TStringList * SutraNum;   // �g��
	TStringList * JuanNum;    // ����
	TStringList * SutraName;  // �g�W
	TStringList * Byline;     // �@Ķ��

	int __fastcall FindIndexBySutraNum(String sBook, String sVol, String sSutraNum);	// ���o Catalog ���s��
	int __fastcall FindIndexBySutraNum(String sBook, int iVol, String sSutraNum);	// ���o Catalog ���s��
	//int __fastcall FindIndexBySutraNum(String sBook, String sSutraNum);	// ���o Catalog ���s��
	void __fastcall LoadCatalog(String sFile);      	// �ǤJ�ɮ�, ��Ȥ�
	__fastcall CCatalog();
	__fastcall ~CCatalog();
};
// ---------------------------------------------------------------------------
#endif
