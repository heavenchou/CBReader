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
		if(i >= iLen)   // �n�D����m�W�L��l�r�����
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
// ���o�l�r��
String __fastcall CMyStrUtil::SubString(String s, int i, int j)
{
	int iLen = s.Length();
	if(iLen == 0) return "";
	if(i >= iLen) return "";	// �n�D����m�W�L��l�r�����
	if(j == 0) return "";  		// �n�D�����׬� 0

	if((i*-1)>iLen) i = iLen * -1;  // i �˰h�Ӧh, �^��_�I�N�n

	if(i>=0 && j>0)
	{
		if(i+j > iLen) j = iLen - i;    // �n�D�����פӪ�, �u���̤j��
		return String (s.begin()+i, j);
	}
	else if(i<0 && j>0)
	{
		if(j > (i*-1)) j = i * -1;       // �n�D�����פӪ�, �u���̤j��
		return String (s.end()+i, j);
	}
	else if(i>=0 && j<0)
	{
		if((iLen-i ) <= (j*-1)) return "";  // j �˰h�����׶W�L�_�I�F
		return String (s.begin()+i, iLen-i+j);
	}
	else if(i<0 && j<0)
	{
		if(j <= i) return "";           // j �˰h�����׶W�L�_�I�F
		return String (s.end()+i, (i*-1)+j);
	}
    return "";
}
//---------------------------------------------------------------------------
// �ǤJ�Ʀr, �Ǧ^ utf16 �� utf32 ���r��
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
// �ǤJ�r��, �Ǧ^�L�Ÿ������, �� StrToULong("FFFF",16) => 65535;
unsigned long __fastcall CMyStrUtil::StrToULong(String s, int iBase)
{
	unsigned long ul = std::strtoul(AnsiString(s).c_str(),NULL,iBase);
	return ul;
}
//---------------------------------------------------------------------------
// �h���Y�����w�r��
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

