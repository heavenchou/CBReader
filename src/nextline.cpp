//---------------------------------------------------------------------------
#pragma hdrstop

#include "nextline.h"
//---------------------------------------------------------------------------
__fastcall CNextLine::CNextLine()		// �غc�禡
{
	InNextLine = false;					// �P�_�O�_�b�j�檺 <tt> ��
    IsOutput = true;
	ThisLine = u"";							// �o�@�檺�F��
	NextLine = u"";                          // �U�@�檺�F��
}
//---------------------------------------------------------------------------
__fastcall CNextLine::~CNextLine(void)	// �Ѻc�禡
{

}
//---------------------------------------------------------------------------
// �J��j���Ӫ� tt �аO
void __fastcall CNextLine::FindNextLine(void)
{
	InNextLine = true;
    IsOutput = false;
	TCount = 0;				// �p�ƾ����s�}�l
}
//---------------------------------------------------------------------------
// �J��j���Ӫ� tt �аO
void __fastcall CNextLine::FindNextLineEnd(void)
{
	InNextLine = false;
}
//---------------------------------------------------------------------------
// �B�z�j���Ӫ����
void __fastcall CNextLine::Add(String sData)
{
	if(TCount == 1)
	{
		if(sData == u"<add_sp>")
		{
			if(ThisLine == u"") sData = u"";
			else sData = u"�@";
		}
		ThisLine = ThisLine + sData;
	}
	else if(TCount == 2)
	{
    	if(sData == u"<add_sp>")
		{
			if(NextLine == u"") sData = u"";
			else sData = u"�@";
		}
		NextLine = NextLine + sData;
	}
}
//---------------------------------------------------------------------------
