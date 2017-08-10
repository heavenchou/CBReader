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

	CCBXMLOption * CBXMLOption; // CBETA 經文的格式

	string BookcaseDir; // 書櫃的目錄

	__fastcall CSetting();
	__fastcall ~CSetting();
};
// ---------------------------------------------------------------------------
#endif
