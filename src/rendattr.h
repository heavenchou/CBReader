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
	int MarginLeft;
	int TextIndent;
    String Border;
	bool IsInline;

    void __fastcall Analysis();
	__fastcall CRendAttr(String sStr);
	__fastcall ~CRendAttr();
};
//---------------------------------------------------------------------------
#endif
