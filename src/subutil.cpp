//---------------------------------------------------------------------------

#pragma hdrstop

#include "subutil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

String __fastcall CMyStrUtil::SubString(String s, int i)
{
	int iLen = s.Length();
	if(i>=0)
	{
		if(i >= iLen)   // 要求的位置超過原始字串長度
			return "";
		else
			return String (s.begin()+i);
	}
	else
	{
		if((i*-1)>iLen)
			return s;
		return
			String (s.end()+i);
	}
}
//---------------------------------------------------------------------------
// 取得子字串
String __fastcall CMyStrUtil::SubString(String s, int i, int j)
{
	int iLen = s.Length();
	if(iLen == 0) return "";
	if(i >= iLen) return "";	// 要求的位置超過原始字串長度
	if(j == 0) return "";  		// 要求的長度為 0

	if((i*-1)>iLen) i = iLen * -1;  // i 倒退太多, 回到起點就好

	if(i>=0 && j>0)
	{
		if(i+j > iLen) j = iLen - i;    // 要求的長度太長, 只取最大值
		return String (s.begin()+i, j);
	}
	else if(i<0 && j>0)
	{
		if(j > (i*-1)) j = i * -1;       // 要求的長度太長, 只取最大值
		return String (s.end()+i, j);
	}
	else if(i>=0 && j<0)
	{
		if((iLen-i ) <= (j*-1)) return "";  // j 倒退的長度超過起點了
		return String (s.begin()+i, iLen-i+j);
	}
	else if(i<0 && j<0)
	{
		if(j <= i) return "";           // j 倒退的長度超過起點了
		return String (s.end()+i, (i*-1)+j);
	}
    return "";
}
//---------------------------------------------------------------------------
// 傳入數字, 傳回 utf16 或 utf32 的字串
String __fastcall CMyStrUtil::LongToUnicode(unsigned long UTF32)
{
	if (UTF32 < 0x10000)
	{
		String s = (wchar_t) UTF32;
		return s;
    }
	unsigned long t = UTF32 - 0x10000;
	unsigned int h = (((t<<12)>>22) + 0xD800);
	unsigned int l = (((t<<22)>>22) + 0xDC00);
    unsigned int ret = ((h<<16) | ( l & 0x0000FFFF));

	String sh = (wchar_t) h;
	String sl = (wchar_t) l;
	return sh + sl;
}
//---------------------------------------------------------------------------
// 傳入字串, 傳回無符號長整數, 例 StrToULong("FFFF",16) => 65535;
unsigned long __fastcall CMyStrUtil::StrToULong(String s, int iBase)
{
	unsigned long ul = std::strtoul(AnsiString(s).c_str(),NULL,iBase);
	return ul;
}
//---------------------------------------------------------------------------
// 去除頭尾指定字元
String __fastcall CMyStrUtil::Trim(String sStr, wchar_t wChar)
{
	while((sStr.Length() > 0) && (*(sStr.begin()) == wChar))
	{
		sStr.Delete0(0,1);
	}

	while((sStr.Length() > 0) && (*(sStr.LastChar()) == wChar))
	{
		sStr.Delete0(sStr.Length()-1,1);
	}

	return sStr;
}
//---------------------------------------------------------------------------

