// ---------------------------------------------------------------------------
#pragma hdrstop
#include "catalog.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 建構式
__fastcall CCatalog::CCatalog()
{
	ID = new TStringList();         // 藏經代碼
	Bulei = new TStringList();      // 部類
	Part = new TStringList();       // 部別
	Vol = new TStringList();        // 冊數
	SutraNum = new TStringList();   // 經號
	JuanNum = new TStringList();    // 卷數
	SutraName = new TStringList();  // 經名
	Byline = new TStringList();     // 作譯者

	Field.push_back(ID);
	Field.push_back(Bulei);
	Field.push_back(Part);
	Field.push_back(Vol);
	Field.push_back(SutraNum);
	Field.push_back(JuanNum);
	Field.push_back(SutraName);
	Field.push_back(Byline);
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CCatalog::~CCatalog()
{
	if(ID) delete ID;
	if(Bulei) delete Bulei;
	if(Part) delete Part;
	if(Vol) delete Vol;
	if(SutraNum) delete SutraNum;
	if(JuanNum) delete JuanNum;
	if(SutraName) delete SutraName;
	if(Byline) delete Byline;

}
// ---------------------------------------------------------------------------
// 載入目錄文件
void __fastcall CCatalog::LoadCatalog(String sFile)
{
	TStringList * slCatalog = new TStringList();

	if(!TFile::Exists(sFile))
	{
		TDialogService::ShowMessage(u"目錄文件不存在 : " + sFile);
		return;
	}

	slCatalog->LoadFromFile(sFile,TEncoding::UTF8);

	for(int i=0; i<slCatalog->Count; i++)
	{
		TStringDynArray slItems = TRegEx::Split(slCatalog->Strings[i], "\\s*,\\s*");

		if(slItems.get_length() == 8)
		{
			for(int i =0; i<8; i++)
			{
				Field[i]->Add(slItems[i]);
            }
		}
    }
}
// ---------------------------------------------------------------------------
// 取得 Catalog 的編號, 傳回 -1 表示找不到
/*
int __fastcall CCatalog::FindIndexBySutraNum(String sBook, String sSutraNum)
{
	for(int i=0; i<ID->Count; i++)
	{
		if(ID->Strings[i] == sBook && SutraNum->Strings[i] == sSutraNum)
			return i;
	}
	return -1;
}
*/
// ---------------------------------------------------------------------------
// 取得 Catalog 的編號, 傳回 -1 表示找不到
int __fastcall CCatalog::FindIndexBySutraNum(String sBook, String sVol, String sSutraNum)
{
	return FindIndexBySutraNum(sBook, sVol.ToInt(), sSutraNum);
}
// ---------------------------------------------------------------------------
// 取得 Catalog 的編號, 傳回 -1 表示找不到
int __fastcall CCatalog::FindIndexBySutraNum(String sBook, int iVol, String sSutraNum)
{
	for(int i=0; i<ID->Count; i++)
	{
		if(ID->Strings[i] == sBook && SutraNum->Strings[i] == sSutraNum)
			if(Vol->Strings[i].ToInt() == iVol && iVol != 0)
				return i;
	}
	return -1;
}
// ---------------------------------------------------------------------------

