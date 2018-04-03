//---------------------------------------------------------------------------
#pragma hdrstop

#include "nextline.h"
//---------------------------------------------------------------------------
__fastcall CNextLine::CNextLine()		// 建構函式
{
	InNextLine = false;					// 判斷是否在隔行的 <tt> 中
    IsOutput = true;
	ThisLine = u"";							// 這一行的東西
	NextLine = u"";                          // 下一行的東西
}
//---------------------------------------------------------------------------
__fastcall CNextLine::~CNextLine(void)	// 解構函式
{

}
//---------------------------------------------------------------------------
// 遇到隔行對照的 tt 標記
void __fastcall CNextLine::FindNextLine(void)
{
	InNextLine = true;
    IsOutput = false;
	TCount = 0;				// 計數器重新開始
}
//---------------------------------------------------------------------------
// 遇到隔行對照的 tt 標記
void __fastcall CNextLine::FindNextLineEnd(void)
{
	InNextLine = false;
}
//---------------------------------------------------------------------------
// 處理隔行對照的資料
void __fastcall CNextLine::Add(String sData)
{
	if(TCount == 1)
	{
		if(sData == u"<add_sp>")
		{
			if(ThisLine == u"") sData = u"";
			else sData = u"　";
		}
		ThisLine = ThisLine + sData;
	}
	else if(TCount == 2)
	{
    	if(sData == u"<add_sp>")
		{
			if(NextLine == u"") sData = u"";
			else sData = u"　";
		}
		NextLine = NextLine + sData;
	}
}
//---------------------------------------------------------------------------
