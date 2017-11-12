//---------------------------------------------------------------------------

#pragma hdrstop

#include "juanline.h"
#include <vector>
#include <iterator>
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
	Spine->BookID = new TStringList();
	Spine->VolNum = new TStringList();
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
		mycoll = TRegEx::Matches(Spine->Files->Strings[i], "[\\/]([A-Z]+)(\\d+)n(.{4,5}?)_?(...)\\.xml");

		String sBookID = mycoll.Item[0].Groups.Item[1].Value;
		String sVolNum = mycoll.Item[0].Groups.Item[2].Value;
		String sVol = sBookID + sVolNum;
		String sSutra = mycoll.Item[0].Groups.Item[3].Value;
		String sJuan = mycoll.Item[0].Groups.Item[4].Value;

		// 記錄每一經的冊, 經, 卷
		Spine->BookID->Add(sBookID);
		Spine->VolNum->Add(sVolNum);
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
// 由冊頁欄行找 Spine 的 Index
int __fastcall CJuanLine::CBGetSpineIndexByVolPageFieldLine(String sBook, String sVol, String sPage, String sField, String sLine)
{
	SPageLineSerialNo * plPageLine;
	if(Vol.count(sVol))
		plPageLine = Vol[sVol];
	else
		return -1;

	// 要組合出標準的 頁欄行

	sPage = GetNormalPage(sPage);		// 處理頁
	sField = GetNormalField(sField);	// 欄
	sLine = GetNormalLine(sLine);		// 行
	String sPageLine = sPage + sField + sLine;

	// 比對方法
	// 因為頁碼有些是有 abc 在前面
	// 通常 abc 會在最前面, 類似序, xyz 在最後面, 類似跋
	// 所以 a001 改成 1a001
	//      0001 改成 20001
	//      z001 改成 2z001
	// 這樣就可以比較大小了

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
// 取得標準 4 位數的頁碼
String __fastcall CJuanLine::GetNormalPage(String sPage)
{
	if(sPage == "") return u"0001";
	int iPageLen = sPage.Length();
	if(iPageLen == 4) return sPage;

	String::iterator it = sPage.begin();

	if(*it >= '0' && *it <= '9')
	{
		// 全部都數字, 補上 0 直至 4 位數
		if(iPageLen < 4)
			sPage = UnicodeString().StringOfChar('0',4-iPageLen) + sPage;
		else
			// 長度原本就超過4了
			sPage = UnicodeString(it+iPageLen-4,4);
	}
	else
	{
		// 第一個字是英文字母
		String sHead = UnicodeString(it,1);

		if(iPageLen < 4)
		{
			String sTail = UnicodeString(it+1,iPageLen-1);
			sPage = sHead + UnicodeString().StringOfChar('0',4-iPageLen) + sTail;
		}
		else
		{
			// 長度原本就超過4了
			sPage = sHead + UnicodeString(it+iPageLen-3,3);
        }
	}
	return sPage;
}
// ---------------------------------------------------------------------------
// 取得標準 1 位數的欄
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
// 取得標準 2 位數的行數
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
// 新的行首, 最前面 a-m 則在字首加 "1" , 其他則加 "2"
String __fastcall CJuanLine::GetNewPageLine(String sPageLine)
{
	auto it = sPageLine.begin();
	if(*it >= 'a' && *it <= 'm') sPageLine = u"1" + sPageLine;
	else sPageLine = u"2" + sPageLine;

	return sPageLine;
}
// ---------------------------------------------------------------------------



