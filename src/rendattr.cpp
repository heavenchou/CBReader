//---------------------------------------------------------------------------

#pragma hdrstop

#include "rendattr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall CRendAttr::CRendAttr(String sStr)
{
	// ���
	Rend = sStr;
	NewStyle = u"";
	RendList = new TStringList;

	MarginLeft = 0;
	TextIndent = 0;
	Border = "";
	IsInline = false;

	// �����̫e�����Ů�
	while(Rend != u"" && *Rend.FirstChar() == u' ') Rend.Delete0(0,1);
	if(Rend != "")
	{
		// ���ݸɤW�Ů�
		if(*Rend.LastChar() != u' ') Rend += u" ";
		Analysis();	// �i����R
		CreateStyle();  // ���ͬ۹����� Style
	}
}
// ---------------------------------------------------------------------------
__fastcall CRendAttr::~CRendAttr(void) // �Ѻc�禡
{
	if(RendList) delete RendList;
	RendList = 0;
}
// ---------------------------------------------------------------------------
void __fastcall CRendAttr::Analysis()
{
	String sRend = Rend;
	int iPos;

	// �v�@���X rend �̭������e, �i��O "no-marker border"
	while(sRend != u"")
	{
		String sTmp = u"";

		int iPos = sRend.Pos0(u" ");
		sTmp = sRend.SubString0(0,iPos);
		RendList->Add(sTmp);

		sRend.Delete0(0,iPos+1);
		// �����i��h�X���Ů�
		while(sRend != u"" && *sRend.FirstChar() == u' ') sRend.Delete0(0,1);
	}
}
// ---------------------------------------------------------------------------
// �b RendList ���Y�r��
bool __fastcall CRendAttr::Find(String sStr)
{
	int i;
    return RendList->Find(sStr,i);
}
// ---------------------------------------------------------------------------
// ���ͬ۹����� Style
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
			// SimSun/NSimSun ²�駺��
			// Songti TC Mac ����
			NewStyle += u"font-family:MingLiU,�ө���,PMingLiU,�s�ө���,SimSun,NSimSun,\"Songti TC\";";
		else if(sStr == u"kaiti")
			// STKaiti �O²�鷢��
			// Kaiti TC Mac ����
			NewStyle += u"font-family:DFKai-SB,�з���,STKaiti,\"Kaiti TC\";";
		else if(sStr == u"heiti")
			// simhei ²�����, Microsoft YaHei �L�n����
			// Heiti TC Mac ����
			NewStyle += u"font-family:\"Microsoft JhengHei\",�L�n������,\"Microsoft YaHei\",simhei,\"Heiti TC\";";
    }
}
// ---------------------------------------------------------------------------
