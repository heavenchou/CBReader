//---------------------------------------------------------------------------

#ifndef RendAttrH
#define RendAttrH

#include <fmx.h>
//---------------------------------------------------------------------------
// 傳入這類字串
// "no-marker border bold large"
// 產生相對應的 style 內容

class CRendAttr
{

private:

public:

	String Rend;
	String NewStyle;   // 由 Rend 的內容自動產生相對應的 Style
	TStringList * RendList; // 存放每一組 Rend
	//int MarginLeft;
	//int TextIndent;
	//String Border;
	//bool IsInline;

	void __fastcall Analysis();
	void __fastcall CreateStyle();		// 產生相對應的 Style
	bool __fastcall Find(String sStr);  // 在 RendList 找到某字串
	__fastcall CRendAttr(String sStr);
	__fastcall ~CRendAttr();
};
//---------------------------------------------------------------------------
#endif
