// ---------------------------------------------------------------------------
#ifndef CatalogH
#define CatalogH

#include <fmx.h>
#include <vector>
#include "System.RegularExpressions.hpp"
#include "System.IOUtils.hpp"

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

	void __fastcall LoadCatalog(String sFile);      	// �ǤJ�ɮ�, ��Ȥ�
	__fastcall CCatalog();
	__fastcall ~CCatalog();
};
// ---------------------------------------------------------------------------
#endif
