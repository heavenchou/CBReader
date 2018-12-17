//---------------------------------------------------------------------------

#pragma hdrstop

#include "styleattr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall CStyleAttr::CStyleAttr(String sStr)
{
	// 初值
	Style = sStr;
	NewStyle = "";    // 移除 段首空白與行首空白之後剩的 Style
	MarginLeft = 0;
	TextIndent = 0;

	if(Style != "")
	{
		// 尾端補上 ;
		if(*Style.LastChar() != u';') Style += u";";
		Analysis();	// 進行分析
	}
}
// ---------------------------------------------------------------------------
__fastcall CStyleAttr::~CStyleAttr(void) // 解構函式
{
}
// ---------------------------------------------------------------------------
void __fastcall CStyleAttr::Analysis()
{
	String sStyle = Style;

	//sStyle = StringReplace(sStyle, u" ", "", TReplaceFlags() << rfReplaceAll);

	int iPos;
	String sMarginLeft = u"";
	String sTextIndent = u"";

	// 逐一取出 style 裡面的內容, 可能是 "margin-left:2em;text-indent:1em;"
	while(sStyle != u"")
	{
		String sTmp = u"";

		if((iPos = sStyle.Pos0(u";")) >= 0)
		{
			sTmp = sStyle.SubString0(0,iPos+1);
			sStyle.Delete0(0,iPos+1);
		}

		if(sTmp.SubString0(0,12) == u"margin-left:")
		{
			sMarginLeft = sTmp;
		}
		else if(sTmp.SubString0(0,12) == u"text-indent:")
		{
			sTextIndent = sTmp;
		}
		else
		{
			NewStyle += sTmp;
        }
	}

	// 如果有 MarginLeft:
	if(sMarginLeft != u"")
	{
		// 支援 style="margin-left:1em" 格式
		if((iPos = sMarginLeft.Pos0(u"em")) >= 0)
		{
			sMarginLeft = sMarginLeft.SubString0(12,iPos-12); // 取出數字
		}
		MarginLeft = sMarginLeft.ToIntDef(0);
	}

	// 如果有 sTextIndent:
	if(sTextIndent != u"")
	{
		if((iPos = sTextIndent.Pos0(u"em")) >= 0)
		{
			sTextIndent = sTextIndent.SubString0(12,iPos-12);
		}
		TextIndent = sTextIndent.ToIntDef(0);
	}
}
// ---------------------------------------------------------------------------