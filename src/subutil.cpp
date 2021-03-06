//---------------------------------------------------------------------------

#pragma hdrstop

#include "subutil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// 將經名後面的 (第X卷-第X卷) or (第X卷) 移除
String __fastcall CMyCBUtil::CutJuanAfterSutraName(String sName)
{
	if(sName.Pos0(u"(第") >= 0)
	{
		int iPos = sName.Pos0(u"(第");
		int iPos2 = sName.Pos0(u"卷)");
		// (第 x 卷) 必須在最後
		if(iPos2 == sName.Length()-2)
		{
			sName = sName.SubString0(0,iPos);
		}
	}
	return sName;
}
//---------------------------------------------------------------------------
// 將經名後面的 （上中下一二三......十）移除
String __fastcall CMyCBUtil::CutNumberAfterSutraName(String sName)
{
	if(sName.Pos0(u"（") >= 0)
	{
		// 華雨集（一）... 是不同經
		if(sName.Pos0(u"華雨集（") >= 0) return sName;
		// 日記（八）（含墨蹟、函札、法語、編後贅語）
		if(sName == u"日記（八）（含墨蹟、函札、法語、編後贅語）") return u"日記";

		TRegEx *regex = new TRegEx();
		sName = regex->Replace(sName, u"（[上中下一二三四五六七八九十、]+）$", "");
	}
	return sName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
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
		String s = (System::WideChar) UTF32;
		return s;
    }
	unsigned long t = UTF32 - 0x10000;
	unsigned int h = (((t<<12)>>22) + 0xD800);
	unsigned int l = (((t<<22)>>22) + 0xDC00);
    unsigned int ret = ((h<<16) | ( l & 0x0000FFFF));

	String sh = (System::WideChar) h;
	String sl = (System::WideChar) l;

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
String __fastcall CMyStrUtil::Trim(String sStr, System::WideChar wChar)
{
	sStr = TrimLeft(sStr, wChar);
	sStr = TrimRight(sStr, wChar);
	return sStr;
}
//---------------------------------------------------------------------------
// 去除左邊指定字元
String __fastcall CMyStrUtil::TrimLeft(String sStr, System::WideChar wChar)
{
	while((sStr.Length() > 0) && (*(sStr.begin()) == wChar))
	{
		sStr.Delete0(0,1);
	}

	return sStr;
}
//---------------------------------------------------------------------------
// 去除右邊指定字元
String __fastcall CMyStrUtil::TrimRight(String sStr, System::WideChar wChar)
{
	while((sStr.Length() > 0) && (*(sStr.LastChar()) == wChar))
	{
		sStr.Delete0(sStr.Length()-1,1);
	}

	return sStr;
}
//---------------------------------------------------------------------------
// 比對指定長度的字串
bool __fastcall CMyStrUtil::StrnCmp(System::WideChar * p1, System::WideChar * p2, int len)
{
	String s1 = String(p1,len);
	String s2 = String(p2,len);
	if(s1 == s2) return true;
	else return false;
}
//---------------------------------------------------------------------------
// 第一個字串是否包含第二個字串
bool __fastcall CMyStrUtil::StrHas(System::WideChar * wp, String s)
{
	int l = s.Length();
	String s1 = String(wp,l);
	if(s == s1) return true;
    else return false;
}
//---------------------------------------------------------------------------



