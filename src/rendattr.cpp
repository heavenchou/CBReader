//---------------------------------------------------------------------------

#pragma hdrstop

#include "rendattr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall CRendAttr::CRendAttr(String sStr)
{
	// 初值
	MarginLeft = 0;
	TextIndent = 0;
	Border = u"";
	IsInline = false;
	Rend = sStr;

	if(Rend != "")	Analysis();	// 進行分析
}
// ---------------------------------------------------------------------------
__fastcall CRendAttr::~CRendAttr(void) // 解構函式
{
}
// ---------------------------------------------------------------------------
void __fastcall CRendAttr::Analysis()
{
	String sRend = Rend;

	// 尾端補上 ;
	if(*sRend.LastChar() != u';') sRend += u";";
	sRend = StringReplace(sRend, u" ", "", TReplaceFlags() << rfReplaceAll);

	int iPos;
	String sMarginLeft = u"";
	String sTextIndent = u"";

	// 逐一取出 rend 裡面的內容, 可能是 "margin-left:2em;text-indent:1em;inline;"
	while(sRend != u"")
	{
		String sTmp = u"";

		if((iPos = sRend.Pos0(u";")) >= 0)
		{
			sTmp = sRend.SubString0(0,iPos);
			sRend.Delete0(0,iPos+1);
		}

		if(sTmp.SubString0(0,12) == u"margin-left:")
		{
			sMarginLeft = sTmp;
		}
		else if(sTmp.SubString0(0,12) == u"text-indent:")
		{
			sTextIndent = sTmp;
		}
		else if(sTmp.SubString0(0,7) == u"border:")
		{
			Border = sTmp;
		}
		else if(sTmp.SubString0(0,6) == u"inline")
		{
			IsInline = true;
		}
	}

	// 如果有 MarginLeft:
	if(sMarginLeft != u"")
	{
		// 支援 rend="margin-left:1em" 格式
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

	// 如果有 Border:
	if(Border != u"")
	{
		Border.Delete0(0,7);
	}

}
// ---------------------------------------------------------------------------