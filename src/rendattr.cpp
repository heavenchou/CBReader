//---------------------------------------------------------------------------

#pragma hdrstop

#include "rendattr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall CRendAttr::CRendAttr(String sStr)
{
	// 初值
	Rend = sStr;
	NewStyle = u"";
	RendList = new TStringList;

	MarginLeft = 0;
	TextIndent = 0;
	Border = "";
	IsInline = false;

	// 移除最前面的空格
	while(Rend != u"" && *Rend.FirstChar() == u' ') Rend.Delete0(0,1);
	if(Rend != "")
	{
		// 尾端補上空格
		if(*Rend.LastChar() != u' ') Rend += u" ";
		Analysis();	// 進行分析
		CreateStyle();  // 產生相對應的 Style
	}
}
// ---------------------------------------------------------------------------
__fastcall CRendAttr::~CRendAttr(void) // 解構函式
{
	if(RendList) delete RendList;
	RendList = 0;
}
// ---------------------------------------------------------------------------
void __fastcall CRendAttr::Analysis()
{
	String sRend = Rend;
	int iPos;

	// 逐一取出 rend 裡面的內容, 可能是 "no-marker border"
	while(sRend != u"")
	{
		String sTmp = u"";

		int iPos = sRend.Pos0(u" ");
		sTmp = sRend.SubString0(0,iPos);
		RendList->Add(sTmp);

		sRend.Delete0(0,iPos+1);
		// 移除可能多出的空格
		while(sRend != u"" && *sRend.FirstChar() == u' ') sRend.Delete0(0,1);
	}
}
// ---------------------------------------------------------------------------
// 在 RendList 找到某字串
bool __fastcall CRendAttr::Find(String sStr)
{
	int i;
    return RendList->Find(sStr,i);
}
// ---------------------------------------------------------------------------
// 產生相對應的 Style
void __fastcall CRendAttr::CreateStyle()
{
	for(int i=0; i<RendList->Count; i++)
	{
		String sStr = RendList->Strings[i];

		if(sStr == u"border")
			NewStyle += u"border:1px black solid;";
		else if(sStr == u"no-border")
			NewStyle += u"border:0;";
		else if(sStr == u"no-marker")
			NewStyle += u"list-style:none;";
		else if(sStr == u"bold")
			NewStyle += u"font-weight:bold;";
		else if(sStr == u"italic")
			NewStyle += u"font-style:italic;";
		else if(sStr == u"circle-above")
			NewStyle += u"text-emphasize:circle-above;";
		else if(sStr == u"mingti" || sStr == u"songti")
			// SimSun/NSimSun 簡體宋體
			// Songti TC Mac 宋體
			NewStyle += u"font-family:MingLiU,細明體,PMingLiU,新細明體,SimSun,NSimSun,\"Songti TC\";";
		else if(sStr == u"kaiti")
			// STKaiti 是簡體楷體
			// Kaiti TC Mac 楷體
			NewStyle += u"font-family:DFKai-SB,標楷體,STKaiti,\"Kaiti TC\";";
		else if(sStr == u"heiti")
			// simhei 簡體黑體, Microsoft YaHei 微軟雅黑
			// Heiti TC Mac 黑體
			NewStyle += u"font-family:\"Microsoft JhengHei\",微軟正黑體,\"Microsoft YaHei\",simhei,\"Heiti TC\";";
    }
}
// ---------------------------------------------------------------------------
