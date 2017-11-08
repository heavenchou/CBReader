//---------------------------------------------------------------------------

#pragma hdrstop

#include "bookdata.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 建構式
__fastcall CBookData::CBookData()
{
	ID = new TStringList();         // 藏經代碼
	VolCount = new TStringList();       // 冊數的位數
	BookName = new TStringList();      // 中文名稱
	BookEngName = new TStringList();        // 英文名稱

}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CBookData::~CBookData()
{
	if(ID) delete ID;
	if(VolCount) delete VolCount;
	if(BookName) delete BookName;
	if(BookEngName) delete BookEngName;

}
// ---------------------------------------------------------------------------
// 載入目錄文件
void __fastcall CBookData::LoadBookDataFile(String sFile)
{
	TStringList * slBookData = new TStringList();

	if(!TFile::Exists(sFile))
	{
		ShowMessage(u"BookData 文件不存在 : " + sFile);
		return;
	}

	slBookData->LoadFromFile(sFile);

	for(int i=0; i<slBookData->Count; i++)
	{
		TStringDynArray slItems = TRegEx::Split(slBookData->Strings[i], "\\s*,\\s*");

		if(slItems.get_length() == 4)
		{
			ID->Add(slItems[0]);
			VolCount->Add(slItems[1]);
			BookName->Add(slItems[2]);
			BookEngName->Add(slItems[3]);
		}
    }
}
// ---------------------------------------------------------------------------
