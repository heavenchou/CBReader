// ---------------------------------------------------------------------------

#pragma hdrstop

#include "bookcase.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CBookcase::CBookcase() // 建構式
{
	BookcaseDir = ""; // 書櫃的目錄
	Books = new TList;
}
// ---------------------------------------------------------------------------
__fastcall CBookcase::~CBookcase(void)
{
	// 逐一解構 Books
	for(int i=0; i < Books->Count; i++)
	{
		CSeries * s = (CSeries *) Books->Items[i];
		if(s) delete s;
    }
	if(Books) delete Books;
}
// ---------------------------------------------------------------------------
// Books 的數量
int __fastcall CBookcase::Count()
{
	return Books->Count;
}

// ---------------------------------------------------------------------------
// 載入所有的書櫃
void __fastcall CBookcase::LoadBookcase(String sDir)
{
	// 檢查書櫃目錄是否存在
	if(!TDirectory::Exists(sDir))
	{
		ShowMessage(u"書櫃目錄不存在 : " + sDir);
		return;
	}
	BookcaseDir = sDir; 	// 書櫃的目錄

	TStringDynArray list;
	list = TDirectory::GetDirectories(sDir, "*", TSearchOption::soTopDirectoryOnly);
	for(int i=0; i<list.Length; i++)
	{
		// 嘗試建立套書物件
		CSeries * s = new CSeries(list[i]);
		if(s->NavFile != "")
			Books->Add(s);   // 至少要找到導覽文件 NavFile
		else
			delete s;
	}
}
// ---------------------------------------------------------------------------

