//---------------------------------------------------------------------------

#ifndef RendAttrH
#define RendAttrH

#include <fmx.h>
//---------------------------------------------------------------------------
// 傳入這類字串
// margin-left:1em;text-indent:1em;inline;border:1px
// 找出 margin-left , text-indent 數字及是否有 inline
// border 後面取出的是字串, 不是數字

class CRendAttr
{

private:

public:

	String Rend;
	String NewStyle;   // 由 Rend 的內容自動產生相對應的 Style
	TStringList * RendList; // 存放每一組 Rend
	int MarginLeft;
	int TextIndent;
	String Border;
	bool IsInline;

	void __fastcall Analysis();
	void __fastcall CreateStyle();		// 產生相對應的 Style
	bool __fastcall Find(String sStr);  // 在 RendList 找到某字串
	__fastcall CRendAttr(String sStr);
	__fastcall ~CRendAttr();
};
//---------------------------------------------------------------------------
#endif
