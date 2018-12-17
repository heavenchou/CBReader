//---------------------------------------------------------------------------

#pragma hdrstop

#include "subutil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// �N�g�W�᭱�� (��X��-��X��) or (��X��) ����
String __fastcall CMyCBUtil::CutJuanAfterSutraName(String sName)
{
	if(sName.Pos0(u"(��") >= 0)
	{
		int iPos = sName.Pos0(u"(��");
		int iPos2 = sName.Pos0(u"��)");
		// (�� x ��) �����b�̫�
		if(iPos2 == sName.Length()-2)
		{
			sName = sName.SubString0(0,iPos);
		}
	}
	return sName;
}
//---------------------------------------------------------------------------
// �N�g�W�᭱�� �]�W���U�@�G�T......�Q�^����
String __fastcall CMyCBUtil::CutNumberAfterSutraName(String sName)
{
	if(sName.Pos0(u"�]") >= 0)
	{
		// �ثB���]�@�^... �O���P�g
		if(sName.Pos0(u"�ثB���]") >= 0) return sName;
		// ��O�]�K�^�]�t���ݡB�祾�B�k�y�B�s���ػy�^
		if(sName == u"��O�]�K�^�]�t���ݡB�祾�B�k�y�B�s���ػy�^") return u"��O";

		TRegEx *regex = new TRegEx();
		sName = regex->Replace(sName, "�][�W���U�@�G�T�|�����C�K�E�Q�B]+�^$", "");
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
// �ǤJ�r��, �Ǧ^�L�Ÿ������, �� StrToULong("FFFF",16) => 65535;
unsigned long __fastcall CMyStrUtil::StrToULong(String s, int iBase)
{
	unsigned long ul = std::strtoul(AnsiString(s).c_str(),NULL,iBase);
	return ul;
}
//---------------------------------------------------------------------------
// �h���Y�����w�r��
String __fastcall CMyStrUtil::Trim(String sStr, System::WideChar wChar)
{
	sStr = TrimLeft(sStr, wChar);
	sStr = TrimRight(sStr, wChar);
	return sStr;
}
//---------------------------------------------------------------------------
// �h��������w�r��
String __fastcall CMyStrUtil::TrimLeft(String sStr, System::WideChar wChar)
{
	while((sStr.Length() > 0) && (*(sStr.begin()) == wChar))
	{
		sStr.Delete0(0,1);
	}

	return sStr;
}
//---------------------------------------------------------------------------
// �h���k����w�r��
String __fastcall CMyStrUtil::TrimRight(String sStr, System::WideChar wChar)
{
	while((sStr.Length() > 0) && (*(sStr.LastChar()) == wChar))
	{
		sStr.Delete0(sStr.Length()-1,1);
	}

	return sStr;
}
//---------------------------------------------------------------------------
// �����w���ת��r��
bool __fastcall CMyStrUtil::StrnCmp(System::WideChar * p1, System::WideChar * p2, int len)
{
	String s1 = String(p1,len);
	String s2 = String(p2,len);
	if(s1 == s2) return true;
	else return false;
}
//---------------------------------------------------------------------------
// �Ĥ@�Ӧr��O�_�]�t�ĤG�Ӧr��
bool __fastcall CMyStrUtil::StrHas(System::WideChar * wp, String s)
{
	int l = s.Length();
	String s1 = String(wp,l);
	if(s == s1) return true;
    else return false;
}
//---------------------------------------------------------------------------



