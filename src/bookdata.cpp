//---------------------------------------------------------------------------

#pragma hdrstop

#include "bookdata.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 建構式
__fastcall CBookData::CBookData()
{
	ID = new TStringList();            // 藏經代碼
	VolCount = new TStringList();      // 冊數的位數
	VerName = new TStringList();       // 版本名稱, 例如大正藏是【大】
	BookName = new TStringList();      // 中文名稱
	BookEngName = new TStringList();   // 英文名稱
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CBookData::~CBookData()
{
	if(ID) delete ID;
	if(VolCount) delete VolCount;
	if(VerName) delete VerName;
	if(BookName) delete BookName;
	if(BookEngName) delete BookEngName;

	ID = 0;            // 藏經代碼
	VolCount = 0;      // 冊數的位數
	VerName = 0;       // 版本名稱, 例如大正藏是【大】
	BookName = 0;      // 中文名稱
	BookEngName = 0;   // 英文名稱

}
// ---------------------------------------------------------------------------
// 載入目錄文件
void __fastcall CBookData::LoadBookDataFile(String sFile)
{
	TStringList * slBookData = new TStringList();

	if(!TFile::Exists(sFile))
	{
		TDialogService::ShowMessage(u"BookData 文件不存在 : " + sFile);
		return;
	}

	slBookData->LoadFromFile(sFile);

	for(int i=0; i<slBookData->Count; i++)
	{
		TStringDynArray slItems = TRegEx::Split(slBookData->Strings[i], u"\\s*,\\s*");

		if(slItems.get_length() == 5)
		{
			ID->Add(slItems[0]);
			VolCount->Add(slItems[1]);
			VerName->Add(slItems[2]);
			BookName->Add(slItems[3]);
			BookEngName->Add(slItems[4]);
		}
    }
}
// ---------------------------------------------------------------------------
// 傳入 T, 傳回其在 TStringList 的 Index , 沒有則傳回 -1
int __fastcall CBookData::GetBookIndex(String sBook)
{
	for(int i=0; i<ID->Count; i++)
	{
		if(ID->Strings[i] == sBook)
			return i;
	}
	return -1;
}
// ---------------------------------------------------------------------------
// 傳入 T, 1 , 傳回 "T01" 這種標準的冊數
String __fastcall CBookData::GetFullVolString(String sBook, String sVol)
{
	sVol = GetNormalVolNumString(sBook, sVol);
	sVol = sBook + sVol;
	return sVol;
}
// ---------------------------------------------------------------------------
// 取得指定藏經的冊數位數, 例如大正藏是 2 位數
int __fastcall CBookData::GetVolLen(String sBook)
{
	int i = GetBookIndex(sBook);
	if(i >= 0)
		return VolCount->Strings[i].ToInt();
	else
		return 0; // 找不到傳回 0
}
// ---------------------------------------------------------------------------
// 傳入 T, 1 , 傳回 "01" 這種標準的冊數
String __fastcall CBookData::GetNormalVolNumString(String sBook, String sVol)
{
	int iVol = sVol.ToIntDef(0);
	//if(iVol == 0) return u"";
	int iVolLen = GetVolLen(sBook); // 取得指定藏經的冊數位數, 例如大正藏是 2 位數
	int iMyLen = sVol.Length();

	if(iMyLen < iVolLen)
	{
		sVol = String().StringOfChar(u'0',iVolLen-iMyLen) + sVol;
	}
	else if(iMyLen > iVolLen)
	{
		sVol.Delete0(0,iVolLen-iMyLen);
    }

	return sVol;
}
// ---------------------------------------------------------------------------
// 傳入 T, 傳回 "【大】" 版本名稱 , 找不到傳回空字串
String __fastcall CBookData::GetVerName(String sBook)
{
	int i = GetBookIndex(sBook);
	if(i >= 0)
		return VerName->Strings[i];
	else
		return u""; // 找不到傳回空字串
}
// ---------------------------------------------------------------------------
// 傳入 T, 傳回 "大正新脩大藏經" 版本名稱 , 找不到傳回空字串
String __fastcall CBookData::GetBookName(String sBook)
{
	int i = GetBookIndex(sBook);
	if(i >= 0)
		return BookName->Strings[i];
	else
		return u""; // 找不到傳回空字串
}
// ---------------------------------------------------------------------------
// 由 bookdata 做出 map , 傳入 "T" , 傳回 "【大】"
// 這是要傳給全文檢索用的
void __fastcall CBookData::BookToVerNameMap(map<String, String> & mpBookToVerName)
{
	for(int i=0; i<ID->Count; i++)
	{
		mpBookToVerName[ID->Strings[i]] = VerName->Strings[i];
    }
}
// ---------------------------------------------------------------------------
