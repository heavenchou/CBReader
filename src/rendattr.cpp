//---------------------------------------------------------------------------

#pragma hdrstop

#include "rendattr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall CRendAttr::CRendAttr(String sStr)
{
	// ���
	MarginLeft = 0;
	TextIndent = 0;
	Border = u"";
	IsInline = false;
	Rend = sStr;

	if(Rend != "")	Analysis();	// �i����R
}
// ---------------------------------------------------------------------------
__fastcall CRendAttr::~CRendAttr(void) // �Ѻc�禡
{
}
// ---------------------------------------------------------------------------
void __fastcall CRendAttr::Analysis()
{
	String sRend = Rend;

	// ���ݸɤW ;
	if(*sRend.LastChar() != u';') sRend += u";";
	sRend = StringReplace(sRend, u" ", "", TReplaceFlags() << rfReplaceAll);

	int iPos;
	String sMarginLeft = u"";
	String sTextIndent = u"";

	// �v�@���X rend �̭������e, �i��O "margin-left:2em;text-indent:1em;inline;"
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

	// �p�G�� MarginLeft:
	if(sMarginLeft != u"")
	{
		// �䴩 rend="margin-left:1em" �榡
		if((iPos = sMarginLeft.Pos0(u"em")) >= 0)
		{
			sMarginLeft = sMarginLeft.SubString0(12,iPos-12); // ���X�Ʀr
		}
		MarginLeft = sMarginLeft.ToIntDef(0);
	}

	// �p�G�� sTextIndent:
	if(sTextIndent != u"")
	{
		if((iPos = sTextIndent.Pos0(u"em")) >= 0)
		{
			sTextIndent = sTextIndent.SubString0(12,iPos-12);
		}
		TextIndent = sTextIndent.ToIntDef(0);
	}

	// �p�G�� Border:
	if(Border != u"")
	{
		Border.Delete0(0,7);
	}

}
// ---------------------------------------------------------------------------