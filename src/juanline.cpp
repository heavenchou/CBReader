//---------------------------------------------------------------------------

#pragma hdrstop

#include "juanline.h"
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

        TMatchCollection mycoll;
		TGroupCollection mygrps;
		mycoll = TRegEx::Matches(da[0], "[\\/]([A-Z]+\d+)n(.{4,5}?)_?(...)\.xml");

		String sVol = mycoll.Item[0].Groups.Item[1].Value;
		String sSutra = mycoll.Item[0].Groups.Item[2].Value;
		String sJuan = mycoll.Item[0].Groups.Item[3].Value;

		// �O���C�@�g���U, �g, ��
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
