//---------------------------------------------------------------------------

#pragma hdrstop

#include "juanline.h"
#include <vector>
#include <iterator>
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// �غc��
__fastcall CJuanLine::CJuanLine()
{
}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CJuanLine::~CJuanLine()
{
}
// ---------------------------------------------------------------------------
// �ǤJ Spane ����
// �̭��O XML/T/T01/T01n0001_001.xml , 0001a01
// ��ܦ��g�����O�� 0001a01 �}�l��
void __fastcall CJuanLine::LoadFromSpine(CSpine * Spine)
{
	Spine->BookID = new TStringList();
	Spine->VolNum = new TStringList();
	Spine->Vol = new TStringList();
	Spine->Sutra = new TStringList();
	Spine->Juan = new TStringList();

	for(int i=0; i<Spine->Files->Count; i++)
	{
		// �ǤJ�����e����
		// XML/T01/T01n0001_001.xml , 0001a01

		String sLine = Spine->Files->Strings[i];

		TStringDynArray da = TRegEx::Split(sLine, "\\s*,\\s*");

		Spine->Files->Strings[i] = da[0];

		// ���W���ݨӤӺC�F, ���U�ۤv�g
		/*
		TMatchCollection mycoll;
		TGroupCollection mygrps;
		mycoll = TRegEx::Matches(Spine->Files->Strings[i], "[\\/]([A-Z]+)(\\d+)n(.{4,5}?)_?(...)\\.xml");

		String sBookID = mycoll.Item[0].Groups.Item[1].Value;
		String sVolNum = mycoll.Item[0].Groups.Item[2].Value;
		String sVol = sBookID + sVolNum;
		String sSutra = mycoll.Item[0].Groups.Item[3].Value;
		String sJuan = mycoll.Item[0].Groups.Item[4].Value;
		*/

		String sBookID;
		String sVolNum;
		String sSutra;
		String sJuan;
		GetBookVolSutraJuan(Spine->Files->Strings[i], sBookID,
							sVolNum, sSutra, sJuan);
		String sVol = sBookID + sVolNum;


		// �O���C�@�g���U, �g, ��
		Spine->BookID->Add(sBookID);
		Spine->VolNum->Add(sVolNum);
		Spine->Vol->Add(sVol);
		Spine->Sutra->Add(sSutra);
		Spine->Juan->Add(sJuan);

		// �p�G�O�s���@�U, �N�]�w�� map
		if(Vol.count(sVol) == 0)
		{
			SPageLineSerialNo * plsn = new SPageLineSerialNo();
			plsn->PageLine = new TStringList();
			Vol.insert(map<String, SPageLineSerialNo *>::value_type (sVol, plsn));
		}

        // �O���C�@�U�U�g�U���� �����
		Vol[sVol]->PageLine->Add(da[1]);
		Vol[sVol]->SerialNo.push_back(i);
	}
}
// ---------------------------------------------------------------------------
// �ѥU������ Spine �� Index
int __fastcall CJuanLine::CBGetSpineIndexByVolPageFieldLine(String sBook, String sVol, String sPage, String sField, String sLine)
{
	String sFullVol = sBook + sVol; // ���� sVol �n�O�зǪ���Ƥ~��
	SPageLineSerialNo * plPageLine;
	if(Vol.count(sFullVol))
		plPageLine = Vol[sFullVol];
	else
		return -1;

	// �n�զX�X�зǪ� �����

	sPage = GetNormalPage(sPage);		// �B�z��
	sField = GetNormalField(sField);	// ��
	sLine = GetNormalLine(sLine);		// ��
	String sPageLine = sPage + sField + sLine;

	// ����k
	// �]�����X���ǬO�� abc �b�e��
	// �q�` abc �|�b�̫e��, ������, xyz �b�̫᭱, �����[
	// �ҥH a001 �令 1a001
	//      0001 �令 20001
	//      z001 �令 2z001
	// �o�˴N�i�H����j�p�F

	String sNewPageLine = GetNewPageLine(sPageLine);

	int cCount = plPageLine->PageLine->Count;
	for(int i=0 ; i < cCount ; i++)
	{
		String sNowPageLine = GetNewPageLine(plPageLine->PageLine->Strings[i]);

		if(sNewPageLine < sNowPageLine)
			if(i == 0)
				return plPageLine->SerialNo[i];
			else
				return plPageLine->SerialNo[i-1];
    }

	return plPageLine->SerialNo[cCount - 1];
}
// ---------------------------------------------------------------------------
// ���o�з� 4 ��ƪ����X
String __fastcall CJuanLine::GetNormalPage(String sPage)
{
	if(sPage == "") return u"0001";
	int iPageLen = sPage.Length();
	if(iPageLen == 4) return sPage;

	String::iterator it = sPage.begin();

	if(*it >= '0' && *it <= '9')
	{
		// �������Ʀr, �ɤW 0 ���� 4 ���
		if(iPageLen < 4)
			sPage = UnicodeString().StringOfChar('0',4-iPageLen) + sPage;
		else
			// ���׭쥻�N�W�L4�F
			sPage = UnicodeString(it+iPageLen-4,4);
	}
	else
	{
		// �Ĥ@�Ӧr�O�^��r��
		String sHead = UnicodeString(it,1);

		if(iPageLen < 4)
		{
			String sTail = UnicodeString(it+1,iPageLen-1);
			sPage = sHead + UnicodeString().StringOfChar('0',4-iPageLen) + sTail;
		}
		else
		{
			// ���׭쥻�N�W�L4�F
			sPage = sHead + UnicodeString(it+iPageLen-3,3);
        }
	}
	return sPage;
}
// ---------------------------------------------------------------------------
// ���o�з� 1 ��ƪ���
String __fastcall CJuanLine::GetNormalField(String sField)
{
	if(sField == "") return u"a";

	int iFieldLen = sField.Length();

	String::iterator it = sField.end();
	if(iFieldLen > 1) sField = UnicodeString(it-1,1);
	sField = sField.LowerCase();
	it = sField.begin();
	if(*it >= '0' && *it <= '9') *it = *it - '1' + 'a';
	if(sField == "0") return u"a";
	return sField;
}
// ---------------------------------------------------------------------------
// ���o�з� 2 ��ƪ����
String __fastcall CJuanLine::GetNormalLine(String sLine)
{
	if(sLine == "") return u"01";

	int iLineLen = sLine.Length();

	if(iLineLen == 1) return "0" + sLine;
	if(iLineLen == 2) return sLine;
	//if(iLineLen > 2)
	{
		String::iterator it = sLine.end();
		return UnicodeString(it-2,2);
	}
}
// ---------------------------------------------------------------------------
// �s���歺, �̫e�� a-m �h�b�r���[ "1" , ��L�h�[ "2"
String __fastcall CJuanLine::GetNewPageLine(String sPageLine)
{
	String::iterator it = sPageLine.begin();
	if(*it >= 'a' && *it <= 'm') sPageLine = u"1" + sPageLine;
	else sPageLine = u"2" + sPageLine;

	return sPageLine;
}
// ---------------------------------------------------------------------------
// �ǤJ�ɦW, ��X��,�U,�g,��
void __fastcall CJuanLine::GetBookVolSutraJuan(String sFile, String &sBook, String &sVol, String &sSutra, String &sJuan)
{
	System::WideChar * iIndex;
	int i1;

    iIndex = sFile.FirstChar();
	i1 = sFile.LastDelimiter0(u"\\/");

	// ���
	iIndex = iIndex + i1 + 1;   // sBook ���_�l��m

	for(int i=1; i<10; i++)
	{
		if(*(iIndex+i) >= u'0' && *(iIndex+i) <= '9')
		{
			sBook = String(iIndex,i);
			iIndex += i;
			break;
		}
	}

	// ��U
	for(int i=2; i<10; i++)
	{
		if(*(iIndex+i) == u'n')
		{
			sVol = String(iIndex,i);
			iIndex = iIndex + i + 1;
			break;
		}
	}

	// ��g��
	if(*(iIndex+4) == u'_')
	{
		sSutra = String(iIndex,4);
		iIndex+=4;
	}
	else
	{
		sSutra = String(iIndex,5);
		iIndex+=5;
	}
	// ��U��
	if(*(iIndex) == u'_') iIndex++;

	sJuan = String(iIndex,3);
}
// ---------------------------------------------------------------------------

