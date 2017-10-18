// ---------------------------------------------------------------------------

#ifndef SettingH
#define SettingH

#include "cbxmloption.h"
#include <fmx.h>
// ---------------------------------------------------------------------------

class CSetting
{
private: // User declarations

public: // User declarations

	CCBXMLOption * CBXMLOption; // CBETA 經文的格式

	String BookcaseDir; // 書櫃的目錄

	__fastcall CSetting();
	__fastcall ~CSetting();
};
// ---------------------------------------------------------------------------
#endif
