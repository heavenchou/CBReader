// ---------------------------------------------------------------------------

#ifndef SettingH
#define SettingH

#include <string>
#include "cbxmloption.h"
// ---------------------------------------------------------------------------
using namespace std;

class CSetting
{
private: // User declarations

public: // User declarations

	CCBXMLOption * CBXMLOption; // CBETA �g�媺�榡

	string BookcaseDir; // ���d���ؿ�

	__fastcall CSetting();
	__fastcall ~CSetting();
};
// ---------------------------------------------------------------------------
#endif
