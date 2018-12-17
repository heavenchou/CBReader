//---------------------------------------------------------------------------

#ifndef StyleAttrH
#define StyleAttrH

#include <fmx.h>
//---------------------------------------------------------------------------
// 傳入這類字串
// margin-left:1em;text-indent:1em;inline;border:1px
// 找出 margin-left , text-indent 數字及是否有 inline
// border 後面取出的是字串, 不是數字

class CStyleAttr
{

private:

public:

	String Style;
    String NewStyle;    // 移除 段首空白與行首空白之後剩的 Style
	int MarginLeft;
	int TextIndent;

    void __fastcall Analysis();
	__fastcall CStyleAttr(String sStr);
	__fastcall ~CStyleAttr();
};
//---------------------------------------------------------------------------
#endif
