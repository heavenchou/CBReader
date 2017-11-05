//---------------------------------------------------------------------------

#pragma hdrstop

#include "juanline.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 建構式
__fastcall CJuanLine::CJuanLine()
{
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CJuanLine::~CJuanLine()
{
}
// ---------------------------------------------------------------------------
// 傳入 Spane 物件
// 裡面是 XML/T/T01/T01n0001_001.xml , 0001a01
// 表示此經此卷是由 0001a01 開始的
void __fastcall CJuanLine::LoadFromSpine(CSpine * Spine)
{
	Spine->Vol = new TStringList();
	Spine->Sutra = new TStringList();
	Spine->Juan = new TStringList();

	for(int i=0; i<Spine->Files->Count; i++)
	{
		// 傳入的內容類似
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

		// 記錄每一經的冊, 經, 卷
		Spine->Vol->Add(sVol);
		Spine->Sutra->Add(sSutra);
		Spine->Juan->Add(sJuan);

		// 如果是新的一冊, 就設定其 map
		if(Vol.count(sVol) == 0)
		{
			SPageLineSerialNo * plsn = new SPageLineSerialNo();
			plsn->PageLine = new TStringList();
			Vol.insert(map<String, SPageLineSerialNo *>::value_type (sVol, plsn));
		}

        // 記錄每一冊各經各卷的 頁欄行
		Vol[sVol]->PageLine->Add(da[1]);
		Vol[sVol]->SerialNo.push_back(i);
	}
}
// ---------------------------------------------------------------------------
