// ---------------------------------------------------------------------------
#pragma hdrstop
#include "spine.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 建構式
__fastcall CSpine::CSpine()
{
	Files = new TStringList();         // 遍歷文件

	// 底下是 CBETA 特有的, 由 JuanLine 物件產生及讀取資料
	BookID = 0;       // 書, 例如 T
	VolNum = 0;     // 冊數, 例如 01
	Vol = 0;		// 冊, 例如 T01
	Sutra = 0;		// 經
	Juan = 0;		// 卷
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CSpine::~CSpine()
{
	if(Files) delete Files;
	if(BookID) delete BookID;
	if(VolNum) delete VolNum;
	if(Vol) delete Vol;
	if(Sutra) delete Sutra;
	if(Juan) delete Juan;
}
// ---------------------------------------------------------------------------
// 載入文件
void __fastcall CSpine::LoadSpineFile(String sFile)
{
	if(!TFile::Exists(sFile))
	{
		ShowMessage(u"遍歷文件不存在 : " + sFile);
		return;
	}

	Files->LoadFromFile(sFile);
}

// ---------------------------------------------------------------------------
// 由經卷去找 Spine 的 Index , 也可以不指定卷
int __fastcall CSpine::CBGetSpineIndexBySutraNumJuan(String sBookID,String sSutraNum,String sJuan)
{
	// 傳回標準的經號格式
	sSutraNum = CBGetSutraNumFormat(sSutraNum);

	// 這二組是為了使用者沒寫經號的 abc... 先自動幫他們加上的
	int iSutraLen = sSutraNum.Length();

	int iJuan = 0;
	if(!sJuan.IsEmpty()) iJuan = sJuan.ToInt(); // 先取得卷數的數字

	bool bFindBook = false;

	for(int i=0; i<BookID->Count; i++)
	{
		if(BookID->Strings[i] == sBookID)
		{
			bFindBook = true;

			String sThisSutra = Sutra->Strings[i];
			String sMySutra = sSutraNum;

			if(Sutra->Strings[i].Length() == 5)
			{
				// 標準經號是 5 位
				sThisSutra = Sutra->Strings[i].LowerCase();

				if(iSutraLen == 4)
					// 輸入只有 4 位, 例如 0123 , 但實際經號是 0123a
					// 所以比對前 4 位就好, 因為使用者可能不知道經號有 abc...
					sMySutra = sSutraNum + "a";
				else
					sMySutra = sSutraNum.LowerCase();
            }

			// 雙方都是 4 位數, 就不用管
			// 輸入是 5 位數, 目前是 4 數位, 肯定不合, 也不用管

			if(sThisSutra == sMySutra)
			{
				if(sJuan == "" || Juan->Strings[i].ToInt() == iJuan)
					return i;
			}
			else if(sThisSutra > sMySutra)
				// 經號超過了
				return -1;

		}
		else if(bFindBook)
			// 曾經找到此書, 但現在又不同書了, 表示找完了
			return -1;
	}

	return -1;
}

// ---------------------------------------------------------------------------
// 則由經卷去找檔名
String __fastcall CSpine::CBGetFileNameBySutraNumJuan(String sBookID,String sSutraNum,String sJuan)
{
	int iIndex = CBGetSpineIndexBySutraNumJuan(sBookID, sSutraNum, sJuan);
	return CBGetFileNameBySpineIndex(iIndex);
}
// ---------------------------------------------------------------------------
// 傳回標準的經號格式, "0001" 或 "0001a"
String __fastcall CSpine::CBGetSutraNumFormat(String sSutraNum)
{
	int iLen;
	wchar_t * w = sSutraNum.LastChar();
	if(*w >= '0' && *w <= '9')
		iLen = 4;	// 最後一個字是數字, 要填 0 直至四位數
	else
		iLen = 5;

	int iStrLen = sSutraNum.Length();
	for(int i=0; i<iLen-iStrLen; i++)
	{
		sSutraNum = "0" + sSutraNum;
	}
	return sSutraNum;
}
// ---------------------------------------------------------------------------
// 由 Spine 的 Index 去找 XML 檔名
String __fastcall CSpine::CBGetFileNameBySpineIndex(int iIndex)
{
    if(iIndex == -1)
		return "";
	else
		return Files->Strings[iIndex];
}
// ---------------------------------------------------------------------------

